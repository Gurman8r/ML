#include <ML/Launcher/Launcher.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ML/Graphics/Model.hpp>

namespace py = pybind11;

namespace ml
{
	// Types
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
		py::class_<ml_py_config>(m, "config")
			.def_static("architecture",			[]() { return ML_ARCHITECTURE; })
			.def_static("compiler_name",		[]() { return ML_CC_NAME; })
			.def_static("compiler_version",		[]() { return ML_CC_VER; })
			.def_static("configuration",		[]() { return ML_CONFIGURATION; })
			.def_static("cplusplus_version",	[]() { return ML_CPLUSPLUS; })
			.def_static("is_debug",				[]() { return ML_DEBUG; })
			.def_static("platform_target",		[]() { return ML_PLATFORM_TARGET; })
			.def_static("project_author",		[]() { return ML_PROJECT_AUTH; })
			.def_static("project_date",			[]() { return ML_PROJECT_DATE; })
			.def_static("project_name",			[]() { return ML_PROJECT_NAME; })
			.def_static("project_time",			[]() { return ML_PROJECT_TIME; })
			.def_static("project_url",			[]() { return ML_PROJECT_URL; })
			.def_static("project_version",		[]() { return ML_PROJECT_VER; })
			.def_static("system_name",			[]() { return ML_SYSTEM_NAME; })
			;
		
		// Content
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_content { };
		py::class_<ml_py_content>(m, "content")
			.def_static("create",	[](str_t t, str_t n) { return (bool)ML_Content.generate(t, n); })
			.def_static("destroy",	[](str_t t, str_t n) { return ML_Content.destroy(String(t).hash(), n); })
			.def_static("exists",	[](str_t t, str_t n) { return ML_Content.exists(String(t).hash(), n); })
			.def_static("load",		[](const dict_t & d) { return MetadataParser::parseMetadata(Metadata { d }); })
			.def_static("load_all", [](const table_t & t) { return MetadataParser::parseMetadata(t); })
			;

		// Plugins
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_plugins {};
		py::class_<ml_py_plugins>(m, "plugins")
			.def_static("load",		[](str_t s) { return ML_Launcher.plugins.loadOneShot(s); })
			.def_static("load_all", [](const list_t & l) { return ML_Launcher.plugins.loadList(l); })
			;

		struct ml_py_io {};
		py::class_<ml_py_io>(m, "io")
			.def_static("clear",	[]() { Debug::clear(); })
			.def_static("command",	[](str_t s) { ML_EventSystem.fireEvent<CommandEvent>(s.c_str()); })
			.def_static("exit",		[]() { Debug::exit(0); })
			.def_static("fatal",	[](str_t s) { Debug::fatal(s); })
			.def_static("pause",	[]() { Debug::pause(0); })
			.def_static("print",	[](str_t s) { cout << s; })
			.def_static("printf",	[](str_t s, const list_t & l) { cout << alg::format(s, l); })
			.def_static("printl",	[](str_t s) { cout << s << endl; })
			.def_static("system",	[](str_t s) { return Debug::system(s.c_str()); })
			;


		// Prefs
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_prefs {};
		py::class_<ml_py_prefs>(m, "prefs")
			.def_static("load",		[](str_t s) { return ML_Launcher.prefs.loadFromFile(s); })
			.def_static("save",		[](str_t s) { return ML_Launcher.prefs.saveToFile(s); })
			.def_static("get",		[](str_t s, str_t n, str_t v) { return (str_t)ML_Launcher.prefs.get_string(s, n, v); })
			.def_static("set",		[](str_t s, str_t n, str_t v) { return ML_Launcher.prefs.set_string(s, n, v); })
			;


		// Window
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_window {};
		py::class_<ml_py_window>(m, "window")
			.def_static("close",			[]() { ML_Launcher.window.close(); })
			.def_static("iconify",			[]() { ML_Launcher.window.iconify(); })
			.def_static("maximize",			[]() { ML_Launcher.window.maximize(); })
			.def_static("restore",			[]() { ML_Launcher.window.restore(); })
			.def_static("is_focused",		[]() { return ML_Launcher.window.is_focused(); })
			.def_static("is_fullscreen",	[]() { return ML_Launcher.window.is_fullscreen(); })
			.def_static("is_open",			[]() { return ML_Launcher.window.is_open(); })
			.def_static("get_clipboard",	[]() { return (str_t)ML_Launcher.window.getClipboardString(); })
			.def_static("get_cursor",		[]() { return (coord_t)(vec2)ML_Launcher.window.getCursorPos(); })
			.def_static("get_key",			[](int32_t i) { return ML_Launcher.window.getKey(i); })
			.def_static("get_title",		[]() { return (str_t)ML_Launcher.window.title(); })
			.def_static("get_size",			[]() { return (coord_t)(vec2)ML_Launcher.window.size(); })
			.def_static("get_position",		[]() { return (coord_t)(vec2)ML_Launcher.window.getPosition(); })
			.def_static("get_time",			[]() { return (float_t)ML_Launcher.window.getTime(); })
			.def_static("set_clipboard",	[](str_t s) { ML_Launcher.window.setClipboardString(s); })
			.def_static("set_cursor",		[](int32_t i) { ML_Launcher.window.setCursorMode(((Cursor::Mode)i)); })
			.def_static("set_fullscreen",	[](bool b) { ML_Launcher.window.setFullscreen(b); })
			.def_static("set_position",		[](const coord_t & c) { ML_Launcher.window.setPosition({ (int32_t)c[0], (int32_t)c[1] }); })
			.def_static("set_size",			[](const coord_t & c) { ML_Launcher.window.setSize({ (uint32_t)c[0], (uint32_t)c[1] }); })
			.def_static("set_title",		[](str_t s) { ML_Launcher.window.setTitle(s); })
			;


		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct ml_py_ecs {};
		py::class_<ml_py_ecs>(m, "ecs")
			.def_static("add_component", [](str_t n, str_t t) { Entity * e { ML_Content.get<Entity>(n) }; return (e && e->addByName(t)); })
			.def_static("get_component", [](str_t n, str_t t) { Entity * e { ML_Content.get<Entity>(n) }; return (e && e->getByName(t)); })
			.def_static("camera_attr", [](str_t name, str_t section, str_t key, str_t value)
			{
				auto * e { ML_Content.get<Entity>(name) }; if (!e) return false;
				auto * c { e->get<Camera>() }; if (!c) return false;
				switch (alg::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): c->setEnabled(input<bool>()(value)); break;
					case Hash("clearflags"): c->setClearFlags(input<Camera::ClearFlags>()(value)); break;
					case Hash("background"): c->setBackground(input<vec4>()(value)); break;
					case Hash("projection"): c->setProjection(input<Camera::Projection>()(value)); break;
					case Hash("position"):	c->setPosition(input<vec3>()(value)); break;
					case Hash("direction"):	c->setDirection(input<vec3>()(value)); break;
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
			.def_static("light_attr", [](str_t name, str_t section, str_t key, str_t value)
			{
				auto * e { ML_Content.get<Entity>(name) }; if (!e) return false;
				auto * c { e->get<Light>() }; if (!c) return false;
				switch (alg::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): c->setEnabled(input<bool>()(value)); break;
					case Hash("color"): c->setColor(input<vec4>()(value)); break;
					case Hash("intensity"): c->setIntensity(input<float_t>()(value)); break;
					case Hash("mode"): c->setMode(input<Light::Mode>()(value)); break;
					}
				}
				break;
				}
				return true;
			})
			.def_static("renderer_attr", [](str_t name, str_t section, str_t key, str_t value)
			{
				auto * e { ML_Content.get<Entity>(name) }; if (!e) return false;
				auto * c { e->get<Renderer>() }; if (!c) return false;
				switch (alg::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): c->setEnabled(input<bool>()(value)); break;
					case Hash("material"): c->setMaterial(ML_Content.get<Material>(value)); break;
					case Hash("shader"): c->setShader(ML_Content.get<Shader>(value)); break;
					case Hash("model"): c->setModel(ML_Content.get<Model>(value)); break;
					}
				}
				break;
				case Hash("alpha"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): 
						c->states().alpha().enabled = input<bool>()(value); 
						break;
					case Hash("predicate"): 
						c->states().alpha().predicate = GL::find_by_raw_name(value.c_str(), GL::Greater);
						break;
					case Hash("coeff"): 
						c->states().alpha().coeff = input<float_t>()(value);
						break;
					}
				}
				break;
				case Hash("blend"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): 
						c->states().blend().enabled = input<bool>()(value); 
						break;
					case Hash("srcRGB"):
						c->states().blend().srcRGB = GL::find_by_raw_name(value.c_str(), GL::SrcAlpha);
						break;
					case Hash("srcAlpha"):
						c->states().blend().srcAlpha = GL::find_by_raw_name(value.c_str(), GL::OneMinusSrcAlpha);
						break;
					case Hash("dstRGB"):
						c->states().blend().dstRGB = GL::find_by_raw_name(value.c_str(), GL::SrcAlpha);
						break;
					case Hash("dstAlpha"): 
						c->states().blend().dstAlpha = GL::find_by_raw_name(value.c_str(), GL::OneMinusSrcAlpha);
						break;
					}
				}
				break;
				case Hash("cull"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): 
						c->states().cull().enabled = input<bool>()(value); 
						break;
					case Hash("face"):
						c->states().cull().face = GL::find_by_raw_name(value.c_str(), GL::Back);
						break;
					}
				}
				break;
				case Hash("depth"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"): 
						c->states().depth().enabled = input<bool>()(value);
						break;
					case Hash("predicate"): 
						c->states().depth().predicate = GL::find_by_raw_name(value.c_str(), GL::Less);
						break;
					case Hash("mask"): 
						c->states().depth().mask = input<bool>()(value); 
						break;
					}
				}
				break;
				}
				return true;
			})
			.def_static("transform_attr", [](str_t name, str_t section, str_t key, str_t value)
			{
				auto * e { ML_Content.get<Entity>(name) }; if (!e) return false;
				auto * c { e->get<Transform>() }; if (!c) return false;
				switch (alg::to_lower(section).hash())
				{
				case Hash("self"):
				{
					switch (alg::to_lower(key).hash())
					{
					case Hash("enabled"):	c->setEnabled(input<bool>()(value)); break;
					case Hash("position"):	c->setPosition(input<vec3>()(value)); break;
					case Hash("scale"):		c->setScale(input<vec3>()(value)); break;
					case Hash("rotation"):	c->setRotation(input<vec3>()(value)); break;
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