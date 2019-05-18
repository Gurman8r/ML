/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Engine/Preferences.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// this needs to be relegated to a DLL
# include "../../../examples/Sandbox/Sandbox.hpp"
# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "Sandbox_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Prefs
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (!ML_Prefs.loadFromFile("../../../ML_Config.ini"))
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

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State, ml::Application *> control =
	{ 
	{ State::Enter, [](auto data)
	{	// Enter
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::EnterEvent(__argc, __argv));
		return control.run(State::Load, data);
	} },
	
	{ State::Load, [](auto data)
	{	// Load
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::LoadEvent());
		return control.run(State::Start, data);
	} },
	
	{ State::Start, [](auto data)
	{	// Start
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::StartEvent());
		return control.run(State::Loop, data);
	} },
	
	{ State::Loop, [](auto data)
	{	// Loop
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Engine.loop([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(ML_Engine.elapsed()));

			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(ML_Engine.elapsed()));

			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(ML_Engine.elapsed()));
		});
		return control.run(State::Unload, data);
	} },
	
	{ State::Unload, [](auto data)
	{	// Unload
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::UnloadEvent());
		return control.run(State::Exit, data);
	} },
	
	{ State::Exit, [](auto data)
	{	// Exit
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EventSystem.fireEvent(ml::ExitEvent());
		return control.run(State::None, data);
	} },
	};

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (ml::Application * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		control.run(State::Enter, app);
		return ML_Engine.freeApp(app);
	}
	else
	{
		return ml::Debug::logError("Failed Launching Application")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */