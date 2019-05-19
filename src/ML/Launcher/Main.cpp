/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorApplication.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Script/Interpreter.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	// Load Instances
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::EventSystem	eventSystem;
	static ml::Preferences	prefs;
	static ml::Resources	resources;
	static ml::Engine		engine(eventSystem, prefs, resources);
	static ml::Editor		editor(engine);

	if (!prefs.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Preferences: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}


	// Setup Control
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> control =
	{ 
	{ State::Enter, []()
	{	// Enter
		eventSystem.fireEvent(ml::EnterEvent(engine));
		return control.run(State::Load);
	} },
	
	{ State::Load, []()
	{	// Load
		eventSystem.fireEvent(ml::LoadEvent(engine));
		return control.run(State::Start);
	} },
	
	{ State::Start, []()
	{	// Start
		eventSystem.fireEvent(ml::StartEvent(engine));
		return control.run(State::Loop);
	} },
	
	{ State::Loop, []()
	{	// Loop
		engine.loopFun([&]()
		{
			// Update
			eventSystem.fireEvent(ml::UpdateEvent(engine));
			// Draw
			eventSystem.fireEvent(ml::DrawEvent(engine));
			// Gui
			eventSystem.fireEvent(ml::GuiEvent(editor));
		});
		return control.run(State::Unload);
	} },
	
	{ State::Unload, []()
	{	// Unload
		eventSystem.fireEvent(ml::UnloadEvent(engine));
		return control.run(State::Exit);
	} },
	
	{ State::Exit, []()
	{	// Exit
		eventSystem.fireEvent(ml::ExitEvent(engine));
		return control.run(State::None);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (auto lib = ml::SharedLibrary(
		ML_FS.getPathTo(prefs.GetString("General", "application", "") + ML_DLL_STR(""))
	))
	{	if (auto app = lib.callFun<ml::Application *>(ML_str(ML_Plugin_Main), &eventSystem))
		{
			if (engine.launchApp(app))
			{
				control.run(State::Enter);
				return engine.freeApp(app);
			}
			else
			{
				return ml::Debug::logError("Failed Launching Application")
					|| ml::Debug::pause(EXIT_FAILURE);
			}
		}
		return ml::Debug::logError("Failed Calling Plugin Main")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
	else
	{
		return ml::Debug::logError("Failed Loading Library: \'{0}\'", lib.filename())
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */