#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/CoreEvents.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<BeginStepEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<EndStepEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<ExitEvent>(this);
		ML_EventSystem.addListener<CommandEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
		
		m_prefs		= new Preferences{ ML_INI_FILENAME };
		m_time		= new GameTime{};
		m_content	= new ContentManager{};
		m_commands	= new CommandRegistry{};
		m_window	= new RenderWindow{};
		m_plugins	= new PluginManager{};
	}

	Engine::~Engine() 
	{
		delete m_plugins;
		delete m_window;
		delete m_commands;
		delete m_content;
		delete m_time;
		delete m_prefs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(Event const & value)
	{
		switch (*value)
		{
		case EnterEvent::ID: if (auto ev{ value.as<EnterEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_ASSERT(ML_Lua.init( // Init Lua
			));

			ML_ASSERT(ML_Py.init( // Init Python
				ev.argv[0],
				ML_FS.pathTo(prefs().get_string("Engine", "library_path", ""))
			));

			// Run Enter Script
			if (const Script scr{ prefs().get_string("Engine", "enter_script", "") })
			{
				scr.execute();
			}

			// Init Window
			ML_ASSERT(window().create(
				prefs().get_string	("Window", "title",			"MemeLib"), {
				prefs().get_uint	("Window", "width",			1280),
				prefs().get_uint	("Window", "height",		720),
				prefs().get_uint	("Window", "color_depth",	32) }, {
				prefs().get_bool	("Window", "resizable",		true),
				prefs().get_bool	("Window", "visible",		false),
				prefs().get_bool	("Window", "decorated",		true),
				prefs().get_bool	("Window", "focused",		true),
				prefs().get_bool	("Window", "auto_iconify",	true),
				prefs().get_bool	("Window", "floating",		false),
				prefs().get_bool	("Window", "maximized",		true),
				prefs().get_bool	("Window", "double_buffer",	true) }, {
				prefs().get_uint	("Window", "major_version",	3),
				prefs().get_uint	("Window", "minor_version",	3),
				prefs().get_uint	("Window", "profile",		ContextSettings::Compat),
				prefs().get_uint	("Window", "depth_bits",	24),
				prefs().get_uint	("Window", "stencil_bits",	8),
				prefs().get_bool	("Window", "multisample",	false),
				prefs().get_bool	("Window", "srgb_capable",	false)
			}));
			
			window().setFullscreen(prefs().get_bool("Window", "fullscreen", false));

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			// Load Defaults
			content().create<Texture>("tex/default")->loadFromImage(
				*content().create<Image>("img/default", Image::Default)
			);
			content().create<Model>("obj/default/triangle")->loadFromMemory(
				geo::triangle_static::vertices, geo::triangle_static::indices
			);
			content().create<Model>("obj/default/quad")->loadFromMemory(
				geo::quad_static::vertices, geo::quad_static::indices
			);
			content().create<Model>("obj/default/cube")->loadFromMemory(
				geo::cube_static::vertices, geo::cube_static::indices
			);
			content().create<Model>("obj/default/skybox")->loadFromMemory(
				geo::skybox_static::vertices
			);
			content().insert<Uniform>("u_delta", new uni_function<float_t>{ "u_delta", [&]() {
				return time().deltaTime();
			} });
			content().insert<Uniform>("u_fps", new uni_function<float_t>{ "u_fps", [&]() {
				return time().frameRate();
			} });
			content().insert<Uniform>("u_frame", new uni_function<int32_t>{ "u_frame", [&]() {
				return (int32_t)time().frameCount();
			} });
			content().insert<Uniform>("u_mouse", new uni_function<vec4>{ "u_mouse", [&]() {
				return vec4 {
					window().getCursorPos()[0],
					window().getCursorPos()[1],
					(float_t)window().getMouseButton(MouseButton::Button0),
					(float_t)window().getMouseButton(MouseButton::Button1)
				};
			} });
			content().insert<Uniform>("u_time", new uni_function<float_t>{ "u_time", [&]() {
				return time().totalTime();
			} });
			content().insert<Uniform>("u_resolution", new uni_function<vec2>{ "u_resolution", [&]() {
				const auto c { Camera::mainCamera() };
				return c ? (vec2)c->viewport().size() : vec2::one();
			} });

			// Run Load Script
			if (const Script scr{ prefs().get_string("Engine", "load_script", "") })
			{
				scr.execute();
			}

			// Set Window Icon
			if (auto icon{ content().get<Image>("_app_icon_") })
			{
				window().setIcon(icon->width(), icon->height(), icon->data());
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case BeginStepEvent::ID: if (auto ev{ value.as<BeginStepEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			window().pollEvents();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			// Update Window Title
			static const String original_title{ window().getTitle() };
			static Timer tm{};
			static float_t dt{ 0 };
			if (tm.elapsed().count() > 0.25)
			{
				dt = time().deltaTime();
				tm.reset();
			}
			window().setTitle(String("{0} | {1} | {2} | {3}s/frame").format(
				original_title, ML_CONFIGURATION, ML_PLATFORM_TARGET, dt
			));

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case EndStepEvent::ID: if (auto ev{ value.as<EndStepEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_GL.flush();

			if (window().getStyle().doubleBuffered())
			{
				window().swapBuffers();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			content().dispose();

			commands().dispose();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case ExitEvent::ID: if (auto ev{ value.as<ExitEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			plugins().dispose();
			
			window().dispose();
			
			ML_Lua.dispose();
			
			ML_Py.dispose();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case CommandEvent::ID: if (auto ev{ value.as<CommandEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			commands().execute(ev.cmd);

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
		{
			/* * * * * * * * * * * * * * * * * * * * */

			static String secret{ "" };
			if (ev.getPress(KeyCode::Up)) secret += "u";
			if (ev.getPress(KeyCode::Down)) secret += "d";
			if (ev.getPress(KeyCode::Left)) secret += "l";
			if (ev.getPress(KeyCode::Right)) secret += "r";
			if (ev.getPress(KeyCode::A)) secret += "a";
			if (ev.getPress(KeyCode::B)) secret += "b";
			if (ev.getPress(KeyCode::Enter))
			{
				if (secret == "uuddlrlrba")
				{
					Debug::execute("open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ");
				}
				secret.clear();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}