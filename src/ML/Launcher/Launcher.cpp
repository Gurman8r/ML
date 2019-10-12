#include <ML/Launcher/Launcher.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Launcher::operator()(int32_t argc, char ** argv)
	{
		// Initialize Instances
		ML_Engine; ML_Editor; ML_Plugins;

		// Pre-Loop
		ML_EventSystem.fireEvent<EnterEvent>(time, prefs, window);
		ML_EventSystem.fireEvent<LoadEvent>	(time, prefs, window);
		ML_EventSystem.fireEvent<StartEvent>(time, prefs, window);

		// Loop
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

		// Shutdown
		ML_EventSystem.fireEvent<UnloadEvent>(time, window);
		ML_EventSystem.fireEvent<ExitEvent>	 (time, window);
		
		// Cleanup Plugins
		ML_Plugins.dispose();

		// Goodbye!
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}