#include <ML/Launcher/Launcher.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Launcher::operator()(int32_t argc, char ** argv)
	{
		// Hello!
		if (running) return EXIT_FAILURE;
		running = true;

		// Startup
		ML_EventSystem.fireEvent<EnterEvent>(time, prefs, window);
		ML_EventSystem.fireEvent<LoadEvent>	(time, prefs, window);
		ML_EventSystem.fireEvent<StartEvent>(time, prefs, window);

		// Loop
		while (window.is_open())
		{
			ML_EventSystem.fireEvent<BeginLoopEvent>(time, window, engine);
			ML_EventSystem.fireEvent<UpdateEvent>	(time, window, engine);
			ML_EventSystem.fireEvent<BeginDrawEvent>(time, window, engine);
			ML_EventSystem.fireEvent<DrawEvent>		(time, window, engine);
			ML_EventSystem.fireEvent<EndDrawEvent>	(time, window, engine);
			ML_EventSystem.fireEvent<BeginGuiEvent>	(time, window, engine, editor);
			ML_EventSystem.fireEvent<GuiEvent>		(time, window, engine, editor);
			ML_EventSystem.fireEvent<EndGuiEvent>	(time, window, engine, editor);
			ML_EventSystem.fireEvent<EndLoopEvent>	(time, window, engine);
		}

		// Shutdown
		ML_EventSystem.fireEvent<UnloadEvent>(time, window, engine);
		ML_EventSystem.fireEvent<ExitEvent>	 (time, window);
		
		// Cleanup Plugins
		plugins.dispose();

		// Goodbye!
		running = false;
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}