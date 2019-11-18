#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/ContentImporter.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ML/Graphics/Model.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Py::init(const String & name, const String & home)
	{
		if (!m_init && name && ML_FS.dirExists(home))
		{
			Py_SetProgramName(util::widen(name).c_str());
			Py_SetPythonHome(util::widen(home).c_str());
			Py_InitializeEx(Py_DontWriteBytecodeFlag);
			m_init = true;
			return true;
		}
		return false;
	}

	bool Py::restart()
	{
		return dispose() && init(m_name, m_home);
	}

	bool Py::dispose()
	{
		if (m_init)
		{
			Py_Finalize();
			m_init = false;
			return true;
		}
		return false;
	}

	int32_t Py::doString(const String & value) const
	{
		return ((value && m_init) ? PyRun_SimpleString(value.c_str()) : 0);
	}

	int32_t Py::doFile(const String & filename) const
	{
		return doString(ML_FS.getFileContents(filename));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	// In / Out Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using str_t		= typename std::string;
	using list_t	= typename std::vector<str_t>;
	using dict_t	= typename std::map<str_t, str_t>;
	using table_t	= typename std::vector<dict_t>;
	using coord_t	= typename std::array<float_t, 2>;

	PYBIND11_EMBEDDED_MODULE(MEMELIB, m)
	{
		// Config
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_config {};
		pybind11::class_<ml_py_config>(m, "config")
			.def_static("architecture", []() { return ML_ARCHITECTURE; })
			.def_static("compiler_name", []() { return ML_CC_NAME; })
			.def_static("compiler_version", []() { return ML_CC_VER; })
			.def_static("configuration", []() { return ML_CONFIGURATION; })
			.def_static("cplusplus_version", []() { return ML_CPP; })
			.def_static("is_debug", []() { return ML_DEBUG; })
			.def_static("platform_target", []() { return ML_PLATFORM_TARGET; })
			.def_static("project_author", []() { return ML_PROJECT_AUTH; })
			.def_static("project_date", []() { return ML_PROJECT_DATE; })
			.def_static("project_name", []() { return ML_PROJECT_NAME; })
			.def_static("project_time", []() { return ML_PROJECT_TIME; })
			.def_static("project_url", []() { return ML_PROJECT_URL; })
			.def_static("project_version", []() { return ML_PROJECT_VER; })
			.def_static("system_name", []() { return ML_SYSTEM_NAME; })
			;
		
		// Content
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_content { };
		pybind11::class_<ml_py_content>(m, "content")
			.def_static("create", [](str_t t, str_t n) { return (bool)ML_Engine.content().generate(t, n); })
			.def_static("destroy", [](str_t t, str_t n) { return ML_Engine.content().destroy(String(t).hash(), n); })
			.def_static("exists", [](str_t t, str_t n) { return ML_Engine.content().exists(String(t).hash(), n); })
			.def_static("load", [](const dict_t & d) { return ContentImporter<>::loadMetadata(Metadata{ d }); })
			.def_static("load_all", [](const table_t & t) { return ContentImporter<>::loadMetadata(t); })
			;

		// IO
		struct ml_py_io {};
		pybind11::class_<ml_py_io>(m, "io")
			.def_static("clear", []() { Debug::clear(); })
			.def_static("command", [](str_t s) { ML_EventSystem.fireEvent<CommandEvent>(s.c_str()); })
			.def_static("exit", []() { Debug::exit(0); })
			.def_static("pause", []() { Debug::pause(0); })
			.def_static("print", [](str_t s) { std::cout << s; })
			.def_static("printf", [](str_t s, const list_t & l) { std::cout << util::format(s, l); })
			.def_static("printl", [](str_t s) { std::cout << s << std::endl; })
			.def_static("log", [](str_t s) { Debug::logInfo(s); })
			.def_static("warning", [](str_t s) { Debug::logWarning(s); })
			.def_static("error", [](str_t s) { Debug::logError(s); })
			;

		// Plugins
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_plugins {};
		pybind11::class_<ml_py_plugins>(m, "plugins")
			.def_static("load", [](str_t s) { return ML_Engine.plugins().loadOneShot(s); })
			.def_static("load_all", [](const list_t & l) { return ML_Engine.plugins().loadList(l); })
			;

		// Prefs
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_prefs {};
		pybind11::class_<ml_py_prefs>(m, "prefs")
			.def_static("load", [](str_t s) { return ML_Engine.prefs().loadFromFile(s); })
			//.def_static("save", [](str_t s) { return ML_Engine.prefs().saveToFile(s); })
			.def_static("get", [](str_t s, str_t n, str_t v) { return (str_t)ML_Engine.prefs().get_string(s, n, v); })
			.def_static("set", [](str_t s, str_t n, str_t v) { return ML_Engine.prefs().set_string(s, n, v); })
			;

		// Window
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_window {};
		pybind11::class_<ml_py_window>(m, "window")
			.def_static("close", []() { ML_Engine.window().close(); })
			.def_static("iconify", []() { ML_Engine.window().iconify(); })
			.def_static("maximize", []() { ML_Engine.window().maximize(); })
			.def_static("restore", []() { ML_Engine.window().restore(); })
			.def_static("is_focused", []() { return ML_Engine.window().isFocused(); })
			.def_static("is_fullscreen", []() { return ML_Engine.window().isFullscreen(); })
			.def_static("is_open", []() { return ML_Engine.window().isOpen(); })
			.def_static("get_clipboard", []() { return (str_t)ML_Engine.window().getClipboardString(); })
			.def_static("get_cursor", []() { return (coord_t)(vec2)ML_Engine.window().getCursorPos(); })
			.def_static("get_key", [](int32_t i) { return ML_Engine.window().getKey(i); })
			.def_static("get_title", []() { return (str_t)ML_Engine.window().getTitle(); })
			.def_static("get_size", []() { return (coord_t)(vec2)ML_Engine.window().getSize(); })
			.def_static("get_position", []() { return (coord_t)(vec2)ML_Engine.window().getPosition(); })
			.def_static("get_time", []() { return (float_t)ML_Engine.window().getTime(); })
			.def_static("set_clipboard", [](str_t s) { ML_Engine.window().setClipboardString(s); })
			.def_static("set_cursor", [](int32_t i) { ML_Engine.window().setCursorMode(((Cursor::Mode)i)); })
			.def_static("set_fullscreen", [](bool b) { ML_Engine.window().setFullscreen(b); })
			.def_static("set_position", [](const coord_t & c) { ML_Engine.window().setPosition({ (int32_t)c[0], (int32_t)c[1] }); })
			.def_static("set_size", [](const coord_t & c) { ML_Engine.window().setSize({ (uint32_t)c[0], (uint32_t)c[1] }); })
			.def_static("set_title", [](str_t s) { ML_Engine.window().setTitle(s); })
			;

		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_ecs {}; 
		pybind11::class_<ml_py_ecs>(m, "ecs")
			.def_static("add_component", [](str_t n, str_t t) {
				auto e { ML_Engine.content().get<Entity>(n) }; 
				return (e && e->addByName(t)); 
			})
			.def_static("get_component", [](str_t n, str_t t) { 
				auto e { ML_Engine.content().get<Entity>(n) }; 
				return (e && e->getByName(t)); 
			})
			.def_static("camera_attr", [](str_t name, str_t section, str_t key, str_t value) {
				auto e { ML_Engine.content().get<Entity>(name) }; if (!e) { return false; }
				auto c { e->get<Camera>() }; if (!c) { return false; }
				switch (util::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): c->setEnabled(input<bool>()(value)); break;
					case Hash("clearflags"): c->setClearFlags(input<Camera::ClearFlags>()(value)); break;
					case Hash("background"): c->setBackground(input<vec4>()(value)); break;
					case Hash("projection"): c->setProjection(input<Camera::Projection>()(value)); break;
					case Hash("position"): c->setPosition(input<vec3>()(value)); break;
					case Hash("direction"): c->setDirection(input<vec3>()(value)); break;
					case Hash("fov"): c->setFieldOfView(input<float_t>()(value)); break;
					case Hash("near"): c->setClipNear(input<float_t>()(value)); break;
					case Hash("far"): c->setClipFar(input<float_t>()(value)); break;
					case Hash("viewport"): c->setViewport(input<IntRect>()(value)); break;
					}
				}
				break;
				}
				return true;
			})
			.def_static("light_attr", [](str_t name, str_t section, str_t key, str_t value) {
				auto e { ML_Engine.content().get<Entity>(name) }; if (!e) { return false; }
				auto l { e->get<Light>() }; if (!l) { return false; }
				switch (util::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): l->setEnabled(input<bool>()(value)); break;
					case Hash("color"): l->setColor(input<vec4>()(value)); break;
					case Hash("intensity"): l->setIntensity(input<float_t>()(value)); break;
					case Hash("mode"): l->setMode(input<Light::Mode>()(value)); break;
					}
				}
				break;
				}
				return true;
			})
			.def_static("renderer_attr", [](str_t name, str_t section, str_t key, str_t value) {
				auto e { ML_Engine.content().get<Entity>(name) }; if (!e) { return false; }
				auto r { e->get<Renderer>() }; if (!r) { return false; }
				switch (util::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): r->setEnabled(input<bool>()(value)); break;
					case Hash("material"): r->setMaterial(ML_Engine.content().get<Material>(value)); break;
					case Hash("shader"): r->setShader(ML_Engine.content().get<Shader>(value)); break;
					case Hash("model"): r->setModel(ML_Engine.content().get<Model>(value)); break;
					}
				}
				break;
				case Hash("alpha"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): r->states().alpha().enabled = input<bool>()(value); break;
					case Hash("func"): r->states().alpha().func = GL::find_by_raw_name(value.c_str(), GL::Greater); break;
					case Hash("coeff"): r->states().alpha().coeff = input<float_t>()(value); break;
					}
				}
				break;
				case Hash("blend"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): r->states().blend().enabled = input<bool>()(value); break;
					case Hash("sfactorRGB"): r->states().blend().sfactorRGB = GL::find_by_raw_name(value.c_str(), GL::SrcAlpha); break;
					case Hash("sfactorAlpha"): r->states().blend().sfactorAlpha = GL::find_by_raw_name(value.c_str(), GL::OneMinusSrcAlpha); break;
					case Hash("dfactorRGB"): r->states().blend().dfactorRGB = GL::find_by_raw_name(value.c_str(), GL::SrcAlpha); break;
					case Hash("dfactorAlpha"):  r->states().blend().dfactorAlpha = GL::find_by_raw_name(value.c_str(), GL::OneMinusSrcAlpha); break;
					}
				}
				break;
				case Hash("cull"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): r->states().cull().enabled = input<bool>()(value); break;
					case Hash("mode"): r->states().cull().mode = GL::find_by_raw_name(value.c_str(), GL::Back); break;
					}
				}
				break;
				case Hash("depth"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): r->states().depth().enabled = input<bool>()(value); break;
					case Hash("func"): r->states().depth().func = GL::find_by_raw_name(value.c_str(), GL::Less); break;
					case Hash("mask"): r->states().depth().mask = input<bool>()(value); break;
					}
				}
				break;
				}
				return true;
			})
			.def_static("transform_attr", [](str_t name, str_t section, str_t key, str_t value) {
				auto e { ML_Engine.content().get<Entity>(name) }; if (!e) { return false; }
				auto t { e->get<Transform>() }; if (!t) { return false; }
				switch (util::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (util::to_lower(key).hash())
					{
					case Hash("enabled"): t->setEnabled(input<bool>()(value)); break;
					case Hash("position"): t->setPosition(input<vec3>()(value)); break;
					case Hash("scale"): t->setScale(input<vec3>()(value)); break;
					case Hash("rotation"): t->setRotation(input<vec4>()(value)); break;
					}
				}
				break;
				}
				return true;
			})
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}