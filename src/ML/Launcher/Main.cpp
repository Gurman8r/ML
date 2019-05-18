/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/StateMachine.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// this all needs to be relegated to a hotloaded DLL

# define MY_PROGRAM DEMO::Sandbox
# define MY_INCLUDE "../../../examples/Sandbox/Sandbox.hpp"
# define MY_LIBRARY ML_LIB_STR("Sandbox")

# include MY_INCLUDE
# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, MY_LIBRARY)
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Preferences
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (!ML_Prefs.loadFromFile("../../../ML.ini"))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Setup Control States
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> control =
	{ 
	{ State::Enter, []()
	{	// Enter State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::EnterEvent(__argc, __argv));
		return control.run(State::Load);
	} },
	
	{ State::Load, []()
	{	// Load State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::LoadEvent());
		return control.run(State::Start);
	} },
	
	{ State::Start, []()
	{	// Start State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::StartEvent());
		return control.run(State::Loop);
	} },
	
	{ State::Loop, []()
	{	// Loop State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Engine.runLoop([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(ML_Engine.frameTime()));
			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(ML_Engine.frameTime()));
			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(ML_Engine.frameTime()));
		});
		return control.run(State::Unload);
	} },
	
	{ State::Unload, []()
	{	// Unload State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::UnloadEvent());
		return control.run(State::Exit);
	} },
	
	{ State::Exit, []()
	{	// Exit State
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::ExitEvent());
		return control.run(State::None);
	} },
	};

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (ml::Application * app = ML_Engine.launchApp(new MY_PROGRAM()))
	{
		control.run(State::Enter);

		return ML_Engine.freeApp(app);
	}
	else
	{
		return ml::Debug::logError("Failed Launching Application")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */