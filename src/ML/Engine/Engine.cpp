#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/CoreEvents.hpp>
#include <ML/Engine/Ref.hpp>
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
	{
		m_time		= new GameTime {};
		m_prefs		= new Preferences { ML_CONFIG_INI };
		m_content	= new ContentManager {};
		m_commands	= new CommandRegistry {};
		m_window	= new RenderWindow {};

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
		delete m_window;
		delete m_commands;
		delete m_content;
		delete m_prefs;
		delete m_time;
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
			m_commands->execute(value.as<CommandEvent>()->cmd);
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
			ML_Py.init(ML_ARGV[0], ML_FS.pathTo(m_prefs->get_string(
				"Engine", "library_path", ""
			)))
		);

		// Run Boot Script
		if (Script scr_enter { m_prefs->get_string("Engine", "enter_script", "") })
		{
			scr_enter.execute();
		}

		// Create Window
		ML_ASSERT(
			"Failed Initializing Window\n" &&
			m_window->create(
				m_prefs->get_string	("Window", "title",			"MemeLib"), { 
				m_prefs->get_uint	("Window", "width",			1280),
				m_prefs->get_uint	("Window", "height",		720),
				m_prefs->get_uint	("Window", "color_depth",	32) }, {
				m_prefs->get_bool	("Window", "resizable",		true),
				m_prefs->get_bool	("Window", "visible",		false),
				m_prefs->get_bool	("Window", "decorated",		true),
				m_prefs->get_bool	("Window", "focused",		true),
				m_prefs->get_bool	("Window", "auto_iconify",	true),
				m_prefs->get_bool	("Window", "floating",		false),
				m_prefs->get_bool	("Window", "maximized",		true) }, {
				m_prefs->get_uint	("Window", "major_version",	3),
				m_prefs->get_uint	("Window", "minor_version",	3),
				m_prefs->get_uint	("Window", "profile",		ContextSettings::Compat),
				m_prefs->get_uint	("Window", "depth_bits",	24),
				m_prefs->get_uint	("Window", "stencil_bits",	8),
				m_prefs->get_bool	("Window", "multisample",	false),
				m_prefs->get_bool	("Window", "srgb_capable",	false)
			})
		);
		
		if (m_prefs->get_bool("Window", "fullscreen", false))
		{
			m_window->setFullscreen(true);
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Defaults
		/* * * * * * * * * * * * * * * * * * * * */
		m_content->create<Texture>("tex/default")->loadFromImage
		(
			*m_content->create<Image>("img/default", Image::Default)
		);

		m_content->create<Model>("obj/default/triangle")->loadFromMemory
		(
			geo::triangle_static::vertices, geo::triangle_static::indices
		);
		
		m_content->create<Model>("obj/default/quad")->loadFromMemory
		(
			geo::quad_static::vertices, geo::quad_static::indices
		);
		
		m_content->create<Model>("obj/default/cube")->loadFromMemory
		(
			geo::cube_static::vertices, geo::cube_static::indices
		);
		
		m_content->create<Model>("obj/default/skybox")->loadFromMemory
		(
			geo::skybox_static::vertices
		);

		m_content->insert<Uniform>("u_delta", new uni_float_clbk {
			"u_delta", [&]() {
				return m_time->deltaTime();
			} });

		m_content->insert<Uniform>("u_fps", new uni_float_clbk {
			"u_fps", [&]() {
				return m_time->frameRate();
			} });

		m_content->insert<Uniform>("u_frame", new uni_int_clbk {
			"u_frame", [&]() {
				return (int32_t)m_time->frameCount();
			} });

		m_content->insert<Uniform>("u_mouse", new uni_vec4_clbk {
			"u_mouse", [&]() {
				return vec4 {
					m_window->getCursorPos()[0],
					m_window->getCursorPos()[1],
					(float_t)m_window->getMouseButton(MouseButton::Button0),
					(float_t)m_window->getMouseButton(MouseButton::Button1)
				};
			} });

		m_content->insert<Uniform>("u_time", new uni_float_clbk {
			"u_time", [&]() {
				return m_time->totalTime();
			} });

		m_content->insert<Uniform>("u_resolution", new uni_vec2_clbk {
			"u_resolution", [&]() {
				if (auto c { Camera::mainCamera() }) { return (vec2)c->viewport().size(); }
				return vec2{};
			} });


		// Run Load Script
		/* * * * * * * * * * * * * * * * * * * * */
		if (Script scr_load { m_prefs->get_string("Engine", "load_script", "") })
		{
			scr_load.execute();
		}

		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (Ref<Image> icon { "_app_icon_" })
		{
			m_window->setIcon(icon->width(), icon->height(), icon->data());
		}
	}

	void Engine::onBeginStep(const BeginStepEvent & ev)
	{
		m_time->beginStep();

		m_window->pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Window Title
		static const String original_title { m_window->getTitle() };
		static Timer tm {};
		static float_t dt { 0 };
		if (tm.elapsed().count() > 0.25)
		{
			dt = m_time->deltaTime();
			tm.reset();
		}
		m_window->setTitle(String{ "{0} | {1} | {2} | {3}s/frame" }.format(
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
	}

	void Engine::onEndStep(const EndStepEvent & ev)
	{
		m_window->swapBuffers();

		m_time->endStep();
	}

	void Engine::onUnload(const UnloadEvent & ev)
	{
		m_content->dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
		m_window->dispose();
		
		ML_Lua.dispose();
		
		ML_Py.dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}