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
		eventSystem.fireEvent(EnterEvent	{ time, prefs, window });
		eventSystem.fireEvent(LoadEvent		{ time, prefs, window });
		eventSystem.fireEvent(StartEvent	{ time, window });

		// Loop
		while (window.is_open())
		{
			eventSystem.fireEvent(BeginFrameEvent	{ time, window, engine });
			eventSystem.fireEvent(UpdateEvent		{ time, window, engine });
			eventSystem.fireEvent(BeginDrawEvent	{ time, window, engine });
			eventSystem.fireEvent(DrawEvent			{ time, window, engine });
			eventSystem.fireEvent(EndDrawEvent		{ time, window, engine });
			eventSystem.fireEvent(BeginGuiEvent		{ time, window, editor });
			eventSystem.fireEvent(GuiEvent			{ time, window, editor });
			eventSystem.fireEvent(EndGuiEvent		{ time, window, editor });
			eventSystem.fireEvent(EndFrameEvent		{ time, window, engine });
		}

		// Shutdown
		eventSystem.fireEvent(UnloadEvent	{ time, window, engine });
		eventSystem.fireEvent(ExitEvent		{ time, window });
		
		// Cleanup Plugins
		plugins.dispose();

		// Goodbye!
		running = false;
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}