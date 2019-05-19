/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorApplication.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Prefs.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Script/DefaultCommands.hpp>
#include <ML/Script/Interpreter.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Preferences
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::Prefs prefs;
	if (!prefs.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}


	// Setup Instances
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ml::DefaultCommands::install();

	static ml::EventSystem	eventSystem;
	static ml::Engine		engine(eventSystem);
	static ml::Editor		editor(eventSystem);
	static ml::Resources	resources;

	eventSystem.addListener(ml::ScriptEvent::EV_Command, &ML_Interpreter);


	// Setup Control
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ControlState : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<ControlState> control =
	{ 
	{ ControlState::Enter, []()
	{	// Enter
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::EnterEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(ControlState::Load);
	} },
	
	{ ControlState::Load, []()
	{	// Load
		eventSystem.fireEvent(ml::LoadEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(ControlState::Start);
	} },
	
	{ ControlState::Start, []()
	{	// Start
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::StartEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(ControlState::Loop);
	} },
	
	{ ControlState::Loop, []()
	{	// Loop
		/* * * * * * * * * * * * * * * * * * * * */
		engine.loopFun([&]()
		{
			// Update
			eventSystem.fireEvent(ml::UpdateEvent(
				engine,
				*engine.app(),
				resources
			));
			// Draw
			eventSystem.fireEvent(ml::DrawEvent(
				engine,
				*engine.app(),
				resources
			));
			// Gui
			eventSystem.fireEvent(ml::GuiEvent(
				engine, 
				*engine.app(),
				editor,
				resources
			));
		});
		return control.run(ControlState::Unload);
	} },
	
	{ ControlState::Unload, []()
	{	// Unload
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::UnloadEvent(
			engine,
			resources
		));
		return control.run(ControlState::Exit);
	} },
	
	{ ControlState::Exit, []()
	{	// Exit
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::ExitEvent(
			engine,
			resources
		));
		return control.run(ControlState::None);
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
				control.run(ControlState::Enter);

				return engine.freeApp(app);
			}
			else
			{
				return ml::Debug::logError("Failed Launching Application")
					|| ml::Debug::pause(EXIT_FAILURE);
			}
		}
		else
		{
			return ml::Debug::logError("Failed Calling Plugin Main")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
	}
	else
	{
		return ml::Debug::logError("Failed Loading Library: \'{0}\'", lib.filename())
			|| ml::Debug::pause(EXIT_FAILURE);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}