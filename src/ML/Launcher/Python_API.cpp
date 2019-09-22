#include <ML/Launcher/Launcher.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/Entity.hpp>

namespace py = pybind11;

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using str_t		= typename std::string;
	using list_t	= typename std::vector<str_t>;
	using dict_t	= typename std::map<str_t, str_t>;
	using table_t	= typename std::vector<dict_t>;


	// Config
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_config, m)
	{
		m.attr("architecture")		= ML_ARCHITECTURE;
		m.attr("compiler_name")		= ML_CC_NAME;
		m.attr("compiler_ver")		= ML_CC_VER;
		m.attr("configuration")		= ML_CONFIGURATION;
		m.attr("cppversion")		= ML_CPLUSPLUS;
		m.attr("is_debug")			= ML_DEBUG;
		m.attr("platform_target")	= ML_PLATFORM_TARGET;
		m.attr("project_auth")		= ML_PROJECT_AUTH;
		m.attr("project_date")		= ML_PROJECT_DATE;
		m.attr("project_name")		= ML_PROJECT_NAME;
		m.attr("project_time")		= ML_PROJECT_TIME;
		m.attr("project_url")		= ML_PROJECT_URL;
		m.attr("project_ver")		= ML_PROJECT_VER;
		m.attr("system_name")		= ML_SYSTEM_NAME;
	}


	// Content
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_content, m)
	{
		m.def("load", [](const dict_t & data)
		{
			Metadata md;
			for (const auto & pair : data)
			{
				md.setData(pair.first, pair.second);
			}
			return MetadataParser::parseMetadata(md);
		});
		m.def("load_all", [](const table_t & value)
		{
			for (const dict_t & elem : value)
			{
				Metadata md;
				for (const auto & pair : elem)
				{
					md.setData(pair.first, pair.second);
				}
				MetadataParser::parseMetadata(md);
			}
			return true;
		});
		m.def("exists", [](const str_t & type, const str_t & name)
		{
			auto & data { ML_Content.at(String(type).hash()) };
			return (data.find(name) != data.end());
		});
		m.def("create", [](const str_t & type, const str_t & name)
		{
			if (const hash_t * code { ML_Registry.get_code(type) })
			{
				auto & data { ML_Content.at(*code) };
				auto it { data.find(name) };
				return (it == data.end()) && data.insert({
					name, (I_Newable *)ML_Registry.generate(*code)
				}).first->second;
			}
			return (bool)Debug::logError("Unknown Type: \'{0}\'", type);
		});
		m.def("destroy", [](const str_t & type, const str_t & name)
		{
			auto & data { ML_Content.at(String(type).hash()) };
			auto it { data.find(name) };
			if (it != data.end())
			{
				delete it->second;
				data.erase(it);
				return true;
			}
			return false;
		});
	}


	// ECS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_ecs, m)
	{
		m.def("add_component", [](const str_t & name, const str_t & type)
		{
			if (Entity * e = ML_Content.get<Entity>(name))
			{
				return (bool)e->addByName(type);
			}
			return false;
		});
	}


	// IO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_io, m)
	{
		m.def("clear", []() { Debug::clear(); });
		m.def("command", [](const str_t & cmd)
		{
			ML_Launcher.eventSystem.fireEvent(CommandEvent(cmd.c_str()));
		});
		m.def("exit", []() { Debug::exit(0); });
		m.def("fatal", [](const str_t & msg) { Debug::fatal(msg); });
		m.def("pause", []() { Debug::pause(0); });
		m.def("print", [](const str_t & msg) { cout << msg; });
		m.def("printf", [](const str_t & fmt, const list_t & args)
		{
			String str { fmt };
			for (size_t i = 0; i < args.size(); i++)
			{
				str.replaceAll(("{" + std::to_string(i) + "}"), args[i]);
			}
			cout << str;
		});
		m.def("printl", [](const str_t & msg) { cout << msg << endl; });
		m.def("system", [](const str_t & msg) { return Debug::system(msg.c_str()); });
	}


	// Plugins
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_plugins, m)
	{
		m.def("load", [](const str_t & filename)
		{
			return ML_Launcher.plugins.loadOneShot(filename);
		});
		m.def("load_all", [](const list_t & filenames) 
		{
			for (const str_t & file : filenames)
			{
				ML_Launcher.plugins.loadOneShot(file);
			}
		});
	}


	// Preferences
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_prefs, m)
	{
		m.def("load", [](const str_t & filename)
		{
			return ML_Launcher.prefs.loadFromFile(filename);
		});
		m.def("save", [](const str_t & filename)
		{
			return ML_Launcher.prefs.saveToFile(filename);
		});
		m.def("get", [](const str_t & section, const str_t & name, const str_t & dv)
		{
			return (str_t)ML_Launcher.prefs.get_string(section, name, dv);
		});
		m.def("set", [](const str_t & section, const str_t & name, const str_t & value)
		{
			return ML_Launcher.prefs.set(section, name, value);
		});
	}


	// Window
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PYBIND11_EMBEDDED_MODULE(memelib_window, m)
	{
		m.def("close", []() { ML_Launcher.window.close(); });
		m.def("destroy", []() { ML_Launcher.window.destroy(); });
		m.def("iconify", []() { ML_Launcher.window.iconify(); });
		m.def("make_context_current", []() { ML_Launcher.window.makeContextCurrent(); });
		m.def("maximize", []() { ML_Launcher.window.maximize(); });
		m.def("poll_events", []() { ML_Launcher.window.pollEvents(); });
		m.def("restore", []() { ML_Launcher.window.restore(); });
		m.def("swap_buffers", []() { ML_Launcher.window.swapBuffers(); });
		m.def("terminate", []() { ML_Launcher.window.terminate(); });
		m.def("is_focused", []() { return ML_Launcher.window.is_focused(); });
		m.def("is_open", []() { return ML_Launcher.window.is_open(); });
		m.def("get_cx", []() { return (int32_t)ML_Launcher.window.getCursorPos()[0]; });
		m.def("get_cy", []() { return (int32_t)ML_Launcher.window.getCursorPos()[1]; });
		m.def("get_key", [](int32_t k) { return ML_Launcher.window.getKey(k); });
		m.def("get_title", []() { return (str_t)ML_Launcher.window.title(); });
		m.def("get_w", []() { return (int32_t)ML_Launcher.window.width(); });
		m.def("get_h", []() { return (int32_t)ML_Launcher.window.height(); });
		m.def("get_x", []() { return ML_Launcher.window.getPosition()[0]; });
		m.def("get_y", []() { return ML_Launcher.window.getPosition()[1]; });
		m.def("set_clipboard", [](const str_t & s) { ML_Launcher.window.setClipboardString(s); });
		m.def("set_cursor", [](int32_t m) { ML_Launcher.window.setCursorMode(((Cursor::Mode)m)); });
		m.def("set_pos", [](int32_t x, int32_t y) { ML_Launcher.window.setPosition({ x, y }); });
		m.def("set_size", [](int32_t w, int32_t h) { ML_Launcher.window.setSize(vec2i { w, h }); });
		m.def("set_swap_interval", [](int32_t i) { ML_Launcher.window.swapInterval(i); });
		m.def("set_title", [](const str_t & s) { ML_Launcher.window.setTitle(s); });
	}

}