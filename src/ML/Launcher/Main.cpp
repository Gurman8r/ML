/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/StateMachine.hpp>

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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::Preferences preferences;

	if (!preferences.loadFromFile("../../../ML.ini"))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Setup Control
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::Engine engine;

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> control =
	{ 
	{ State::Enter, []()
	{	// Enter
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::EnterEvent(
			__argc,
			__argv, 
			preferences,
			engine,
			ML_Resources
		));
		return control.run(State::Load);
	} },
	
	{ State::Load, []()
	{	// Load
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::LoadEvent(
			preferences,
			engine,
			ML_Resources
		));
		return control.run(State::Start);
	} },
	
	{ State::Start, []()
	{	// Start
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::StartEvent(
			preferences,
			engine,
			ML_Resources
		));
		return control.run(State::Loop);
	} },
	
	{ State::Loop, []()
	{	// Loop
		/* * * * * * * * * * * * * * * * * * * * */
		engine.loopFun([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(
				engine,
				ML_Resources
			));
			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(
				engine,
				ML_Resources
			));
			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(
				engine, 
				ML_Editor,
				ML_Resources
			));
		});
		return control.run(State::Unload);
	} },
	
	{ State::Unload, []()
	{	// Unload
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::UnloadEvent(
			engine,
			ML_Resources
		));
		return control.run(State::Exit);
	} },
	
	{ State::Exit, []()
	{	// Exit
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::ExitEvent(
			engine,
			ML_Resources
		));
		return control.run(State::None);
	} },
	};

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (ml::Application * app = engine.launchApp(new MY_PROGRAM(ML_EventSystem)))
	{
		control.run(State::Enter);

		return engine.freeApp(app);
	}
	else
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */