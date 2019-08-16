#include <ML/Launcher/Launcher.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Asset.hpp>
#include <ML/Engine/ContentLoader.hpp>

// Launcher
/* * * * * * * * * * * * * * * * * * * * */
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

// Scripting
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	// Config
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_config, m)
	{
		m.attr("architecture")		= ML_ARCHITECTURE;
		m.attr("compiler_name")		= ML_CC_NAME;
		m.attr("compiler_ver")		= ML_CC_VER;
		m.attr("configuration")		= ML_CONFIGURATION;
		m.attr("is_debug")			= ML_DEBUG;
		m.attr("platform_target")	= ML_PLATFORM_TARGET;
		m.attr("project_url")		= ML_PROJECT_URL;
		m.attr("project_ver")		= ML_PROJECT_VER;
	}

	// IO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_io, m)
	{
		m.def("pause", []() { Debug::pause(0); });
		m.def("print", [](const std::string & value) { cout << value; });
		m.def("printl", [](const std::string & value) { cout << value << endl; });
	}

	// Content
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_content, m)
	{
		m.def("load", [](const Tree<std::string, std::string> & data)
		{
			Metadata md;
			for (const auto & pair : data) md.setData(
				pair.first, pair.second
			);
			return (bool)ContentLoader::parseMetadata(md);
		});
	}

	// Window
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_window, m)
	{
		m.def("is_open", []() { return ML_Launcher.window.isOpen(); });
	}

	// Plugins
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_plugins, m)
	{
		m.def("load", [](const std::string & filename)
		{
			return ML_Launcher.plugins.loadOneShot(filename);
		});
	}

}