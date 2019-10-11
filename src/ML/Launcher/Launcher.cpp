#include <ML/Launcher/Launcher.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Launcher::operator()(int32_t argc, char ** argv)
	{
		// Hello!
		if (running) { return EXIT_FAILURE; } running = true;

		// Initialize Instances
		ML_Engine; ML_Editor; ML_Plugins;

		// Startup Events
		ML_EventSystem.fireEvent<EnterEvent>(time, prefs, window);
		ML_EventSystem.fireEvent<LoadEvent>	(time, prefs, window);
		ML_EventSystem.fireEvent<StartEvent>(time, prefs, window);

		// Loop Events
		while (window.is_open())
		{
			ML_EventSystem.fireEvent<BeginLoopEvent>(time, window);
			ML_EventSystem.fireEvent<UpdateEvent>	(time, window);
			ML_EventSystem.fireEvent<BeginDrawEvent>(time, window);
			ML_EventSystem.fireEvent<DrawEvent>		(time, window);
			ML_EventSystem.fireEvent<EndDrawEvent>	(time, window);
			ML_EventSystem.fireEvent<BeginGuiEvent>	(time, window);
			ML_EventSystem.fireEvent<GuiEvent>		(time, window);
			ML_EventSystem.fireEvent<EndGuiEvent>	(time, window);
			ML_EventSystem.fireEvent<EndLoopEvent>	(time, window);
		}

		// Shutdown Events
		ML_EventSystem.fireEvent<UnloadEvent>(time, window);
		ML_EventSystem.fireEvent<ExitEvent>	 (time, window);
		
		// Cleanup Plugins
		ML_Plugins.dispose();

		// Goodbye!
		running = false; return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}