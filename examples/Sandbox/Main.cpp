/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/StateMachine.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML_Config.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Settings
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (!SETTINGS.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
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

	static ml::StateMachine<State> control =
	{
	{ State::Enter, []()
	{	// Enter
		ML_EventSystem.fireEvent(ml::EnterEvent(__argc, __argv));
		return control.run(State::Load);
	} },
	{ State::Load, []()
	{	// Load
		ML_EventSystem.fireEvent(ml::LoadEvent());
		return control.run(State::Start);
	} },
	{ State::Start, []()
	{	// Start
		ML_EventSystem.fireEvent(ml::StartEvent());
		return control.run(State::Loop);
	} },
	{ State::Loop, []()
	{	// Loop
		ML_Engine.loop([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(ML_Engine.elapsed()));
			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(ML_Engine.elapsed()));
			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(ML_Engine.elapsed()));
		});
		return control.run(State::Unload);
	} },
	{ State::Unload, []()
	{	// Unload
		ML_EventSystem.fireEvent(ml::UnloadEvent());
		return control.run(State::Exit);
	} },
	{ State::Exit, []()
	{	// Exit
		ML_EventSystem.fireEvent(ml::ExitEvent());
		return control.run(State::None);
	} },
	};

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (ml::Application * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		control.run(State::Enter);
		return ML_Engine.freeApp(app);
	}
	else
	{
		return ml::Debug::logError("Failed Launching Application")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */