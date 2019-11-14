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
		: m_time	{}
		, m_prefs	{ ML_CONFIG_INI }
		, m_plugins	{}
		, m_content	{}
		, m_commands{}
		, m_window	{}
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<BeginStepEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<BeginDrawEvent>(this);
		ML_EventSystem.addListener<DrawEvent>(this);
		ML_EventSystem.addListener<EndDrawEvent>(this);
		ML_EventSystem.addListener<EndStepEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<ExitEvent>(this);
		ML_EventSystem.addListener<CommandEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	Engine::~Engine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID		: return onEnter(*value.as<EnterEvent>());
		case LoadEvent::ID		: return onLoad(*value.as<LoadEvent>());
		case BeginStepEvent::ID	: return onBeginStep(*value.as<BeginStepEvent>());
		case UpdateEvent::ID	: return onUpdate(*value.as<UpdateEvent>());
		case BeginDrawEvent::ID	: return onBeginDraw(*value.as<BeginDrawEvent>());
		case DrawEvent::ID		: return onDraw(*value.as<DrawEvent>());
		case EndDrawEvent::ID	: return onEndDraw(*value.as<EndDrawEvent>());
		case EndStepEvent::ID	: return onEndStep(*value.as<EndStepEvent>());
		case UnloadEvent::ID	: return onUnload(*value.as<UnloadEvent>());
		case ExitEvent::ID		: return onExit(*value.as<ExitEvent>());

		case CommandEvent::ID:
			commands().execute(value.as<CommandEvent>()->cmd);
			break;

		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>())
			{
				static String secret { "" };
				if (ev->getPress(KeyCode::Up)) secret += "u";
				if (ev->getPress(KeyCode::Down)) secret += "d";
				if (ev->getPress(KeyCode::Left)) secret += "l";
				if (ev->getPress(KeyCode::Right)) secret += "r";
				if (ev->getPress(KeyCode::A)) secret += "a";
				if (ev->getPress(KeyCode::B)) secret += "b";
				if (ev->getPress(KeyCode::Enter))
				{
					if (secret == "uuddlrlrba")
					{
						ML_EventSystem.fireEvent<SecretEvent>();
					}
					secret.clear();
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Start Python
		ML_ASSERT(
			"Failed Initializing Python\n" &&
			ML_Py.init(__argv[0], ML_FS.pathTo(prefs().get_string(
				"Engine", "library_path", ""
			)))
		);

		// Start Lua
		ML_ASSERT(
			"Failed Initializing Lua\n" &&
			ML_Lua.init()
		);

		// Run Boot Script
		if (Script scr_enter { prefs().get_string("Engine", "enter_script", "") })
		{
			scr_enter.execute();
		}

		// Create Window
		ML_ASSERT(
			"Failed Initializing Window\n" &&
			window().create(
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
				prefs().get_bool	("Window", "maximized",		true) }, {
				prefs().get_uint	("Window", "major_version",	3),
				prefs().get_uint	("Window", "minor_version",	3),
				prefs().get_uint	("Window", "profile",		ContextSettings::Compat),
				prefs().get_uint	("Window", "depth_bits",	24),
				prefs().get_uint	("Window", "stencil_bits",	8),
				prefs().get_bool	("Window", "multisample",	false),
				prefs().get_bool	("Window", "srgb_capable",	false)
			})
		);
		
		if (prefs().get_bool("Window", "fullscreen", false))
		{
			window().setFullscreen(true);
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Defaults
		/* * * * * * * * * * * * * * * * * * * * */
		content().create<Texture>("tex/default")->loadFromImage
		(
			*content().create<Image>("img/default", Image::Default)
		);

		content().create<Model>("obj/default/triangle")->loadFromMemory
		(
			geo::triangle_static::vertices, geo::triangle_static::indices
		);
		
		content().create<Model>("obj/default/quad")->loadFromMemory
		(
			geo::quad_static::vertices, geo::quad_static::indices
		);
		
		content().create<Model>("obj/default/cube")->loadFromMemory
		(
			geo::cube_static::vertices, geo::cube_static::indices
		);
		
		content().create<Model>("obj/default/skybox")->loadFromMemory
		(
			geo::skybox_static::vertices
		);

		content().insert<Uniform>("u_delta", new uni_float_clbk {
			"u_delta", [&]() {
				return time().deltaTime();
			} });

		content().insert<Uniform>("u_fps", new uni_float_clbk {
			"u_fps", [&]() {
				return time().frameRate();
			} });

		content().insert<Uniform>("u_frame", new uni_int_clbk {
			"u_frame", [&]() {
				return (int32_t)time().frameCount();
			} });

		content().insert<Uniform>("u_mouse", new uni_vec4_clbk {
			"u_mouse", [&]() {
				return vec4 {
					window().getCursorPos()[0],
					window().getCursorPos()[1],
					(float_t)window().getMouseButton(MouseButton::Button0),
					(float_t)window().getMouseButton(MouseButton::Button1)
				};
			} });

		content().insert<Uniform>("u_scroll", new uni_vec2_clbk{
			"u_scroll", [&]() {
				return window().getScroll();
			} });

		content().insert<Uniform>("u_time", new uni_float_clbk {
			"u_time", [&]() {
				return time().totalTime();
			} });

		content().insert<Uniform>("u_resolution", new uni_vec2_clbk {
			"u_resolution", [&]() {
				if (auto c { Camera::mainCamera() }) { return (vec2)c->viewport().size(); }
				return vec2{};
			} });


		// Run Load Script
		/* * * * * * * * * * * * * * * * * * * * */
		if (Script scr_load { prefs().get_string("Engine", "load_script", "") })
		{
			scr_load.execute();
		}

		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const_ptr_t<Image> icon { content().get<Image>("_app_icon_") })
		{
			window().setIcon(icon->width(), icon->height(), icon->data());
		}
	}

	void Engine::onBeginStep(const BeginStepEvent & ev)
	{
		time().beginStep();

		window().pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Window Title
		static const String original_title { window().getTitle() };
		static Timer tm {};
		static float_t dt { 0 };
		if (tm.elapsed().count() > 0.25)
		{
			dt = time().deltaTime();
			tm.reset();
		}
		window().setTitle(String{ "{0} | {1} | {2} | {3}s/frame" }.format(
			original_title, 
			ML_CONFIGURATION, 
			ML_PLATFORM_TARGET, 
			dt
		));
	}

	void Engine::onBeginDraw(const BeginDrawEvent & ev)
	{
	}

	void Engine::onDraw(const DrawEvent & ev)
	{
	}

	void Engine::onEndDraw(const EndDrawEvent & ev)
	{
		window().swapBuffers();
	}

	void Engine::onEndStep(const EndStepEvent & ev)
	{
		time().endStep();
	}

	void Engine::onUnload(const UnloadEvent & ev)
	{
		content().dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
		window().dispose();

		ML_Lua.dispose();

		ML_Py.dispose();

		plugins().dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}