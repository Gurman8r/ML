#include <ML/Launcher/Launcher.hpp>

namespace ml
{
	Launcher::Launcher()
		: prefs			{ "../../../ML.ini" }
		, time			{ }
		, eventSystem	{ }
		, window		{ eventSystem }
		, engine		{ eventSystem }
		, editor		{ eventSystem }
		, plugins		{ eventSystem }
	{
	}

	int32_t Launcher::run()
	{
		if (m_running) return EXIT_FAILURE;
		else m_running = true;

		// Startup
		eventSystem.fireEvent(EnterEvent	{ time, prefs, window });
		eventSystem.fireEvent(LoadEvent		{ time, prefs, window });
		eventSystem.fireEvent(StartEvent	{ time, window });

		// Loop
		while (window.isOpen())
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
		eventSystem.fireEvent(ExitEvent		{ time });
		
		// Cleanup Plugins
		plugins.dispose();

		// Goodbye!
		m_running = false;
		return EXIT_SUCCESS;
	}
}