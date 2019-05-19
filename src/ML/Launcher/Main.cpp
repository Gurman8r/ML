/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
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

// this must be relegated to a plugin

# define MY_PROGRAM DEMO::Sandbox
# define MY_INCLUDE "../../../proj/examples/Sandbox/Sandbox.hpp"
# define MY_LIBRARY ML_LIB_STR("Sandbox")

# if defined(ML_SYSTEM_WINDOWS)
#	include MY_INCLUDE
#	pragma comment(lib, MY_LIBRARY)
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Preferences
	/* * * * * * * * * * * * * * * * * * * * */

	static ml::Prefs prefs;
	if (!prefs.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}


	// Setup Instances
	/* * * * * * * * * * * * * * * * * * * * */

	ml::DefaultCommands::install();

	static ml::EventSystem	eventSystem;
	static ml::Engine		engine(eventSystem);
	static ml::Editor		editor(eventSystem);
	static ml::Resources	resources;

	eventSystem.addListener(ml::ScriptEvent::EV_Command, &ML_Interpreter);


	// Setup Control
	/* * * * * * * * * * * * * * * * * * * * */

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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::EnterEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(State::Load);
	} },
	
	{ State::Load, []()
	{	// Load
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::LoadEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(State::Start);
	} },
	
	{ State::Start, []()
	{	// Start
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::StartEvent(
			prefs,
			engine,
			*engine.app(),
			resources
		));
		return control.run(State::Loop);
	} },
	
	{ State::Loop, []()
	{	// Loop
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
		return control.run(State::Unload);
	} },
	
	{ State::Unload, []()
	{	// Unload
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::UnloadEvent(
			engine,
			resources
		));
		return control.run(State::Exit);
	} },
	
	{ State::Exit, []()
	{	// Exit
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		eventSystem.fireEvent(ml::ExitEvent(
			engine,
			resources
		));
		return control.run(State::None);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * */
	if (ml::Application * app = engine.launchApp(new MY_PROGRAM(eventSystem)))
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