#include <ML/Launcher/Launcher.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Asset.hpp>
#include <ML/Engine/ContentLoader.hpp>

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using str_t = std::string;
	using args_t = std::vector<str_t>;


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
		m.def("print", [](const str_t & value) { cout << value; });
		m.def("printl", [](const str_t & value) { cout << value << endl; });

		m.def("command", [](const str_t & cmd)
		{
			ML_Launcher.eventSystem.fireEvent(CommandEvent(cmd.c_str()));
		});

		m.def("format", [](const str_t & fmt, const args_t & args)
		{
			String str(fmt);
			for (size_t i = 0; i < args.size(); i++)
			{
				str.replaceAll(("{" + std::to_string(i) + "}"), args[i]);
			}
			return (str_t)str;
		});
	}


	// Content
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_content, m)
	{
		m.def("load", [](const Tree<str_t, str_t> & data)
		{
			Metadata md;
			for (const auto & pair : data) 
				md.setData(pair.first, pair.second);
			return (bool)ContentLoader::parseMetadata(md);
		});
	}


	// Window
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_window, m)
	{
		m.def("is_focused", []() { return ML_Launcher.window.isFocused(); });
		m.def("is_open", []() { return ML_Launcher.window.isOpen(); });
		m.def("get_key", [](int32_t value) { return ML_Launcher.window.getKey(value); });
		m.def("get_title", []() { return (str_t)ML_Launcher.window.getTitle(); });
		m.def("get_width", []() { return (int32_t)ML_Launcher.window.getWidth(); });
		m.def("get_height", []() { return (int32_t)ML_Launcher.window.getHeight(); });
		m.def("get_x", []() { return ML_Launcher.window.getPosition()[0]; });
		m.def("get_y", []() { return ML_Launcher.window.getPosition()[1]; });
		m.def("get_cursor_x", []() { return (int32_t)ML_Launcher.window.getCursorPos()[0]; });
		m.def("get_cursor_y", []() { return (int32_t)ML_Launcher.window.getCursorPos()[1]; });
		// etc...
	}


	// Plugins
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_plugins, m)
	{
		m.def("load", [](const str_t & filename)
		{
			return ML_Launcher.plugins.loadOneShot(filename);
		});
	}

}