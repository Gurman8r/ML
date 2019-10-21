#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Script.hpp>
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
		: m_prefs{ ML_CONFIG_INI }
		, m_time{}
		, m_window{}
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
			ML_CommandRegistry.execute(value.as<CommandEvent>()->cmd);
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Initialize Python
		if (!ML_Py.init(ML_ARGV[0], ML_FS.pathTo(m_prefs.get_string(
			"Engine", "python_home", ""
		))))
		{
			Debug::fatal("Failed initializing Python");
		}

		// Run Boot Script
		if (Script boot { m_prefs.get_string("Engine", "boot_script", "") })
		{
			boot.execute();
		}

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (!m_window.create(
			m_prefs.get_string	("Window", "title",				"MemeLib"), { 
			m_prefs.get_uint	("Window", "width",				1280),
			m_prefs.get_uint	("Window", "height",			720),
			m_prefs.get_uint	("Window", "bits_per_pixel",	32) }, {
			m_prefs.get_bool	("Window", "resizable",			true),
			m_prefs.get_bool	("Window", "visible",			false),
			m_prefs.get_bool	("Window", "decorated",			true),
			m_prefs.get_bool	("Window", "focused",			true),
			m_prefs.get_bool	("Window", "auto_iconify",		true),
			m_prefs.get_bool	("Window", "floating",			false),
			m_prefs.get_bool	("Window", "maximized",			true) }, {
			m_prefs.get_uint	("Window", "major_version",		3),
			m_prefs.get_uint	("Window", "minor_version",		3),
			m_prefs.get_uint	("Window", "context_profile",	ContextSettings::Compat),
			m_prefs.get_uint	("Window", "depth_bits",		24),
			m_prefs.get_uint	("Window", "stencil_bits",		8),
			m_prefs.get_bool	("Window", "multisample",		false),
			m_prefs.get_bool	("Window", "srgb_capable",		false)
		}))
		{
			Debug::fatal("Failed Creating Window");
		}
		else if (m_prefs.get_bool("Window", "fullscreen", false))
		{
			m_window.setFullscreen(true);
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Defaults
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Texture>("default_texture")->loadFromImage
		(
			*ML_Content.create<Image>("default_image", Image::Default)
		);

		ML_Content.create<Model>("default_triangle")->loadFromMemory
		(
			geo::triangle_static::vertices,
			geo::triangle_static::indices
		);
		
		ML_Content.create<Model>("default_quad")->loadFromMemory
		(
			geo::quad_static::vertices,
			geo::quad_static::indices
		);
		
		ML_Content.create<Model>("default_cube")->loadFromMemory
		(
			geo::cube_static::vertices,
			geo::cube_static::indices
		);
		
		ML_Content.create<Model>("default_skybox")->loadFromMemory
		(
			geo::skybox_static::vertices
		);

		ML_Content.insert<Uniform>("u_cursor", new uni_vec2_clbk
		{ 
			"u_cursor", []() { return ML_Engine.window().getCursorPos(); }
		});
		
		ML_Content.insert<Uniform>("u_delta", new uni_float_clbk
		{ 
			"u_delta", []() { return ML_Engine.time().deltaTime(); }
		});
		
		ML_Content.insert<Uniform>("u_frame", new uni_int_clbk
		{ 
			"u_frame", []() { return ML_Engine.time().frameCount(); }
		});
		
		ML_Content.insert<Uniform>("u_fps", new uni_float_clbk
		{ 
			"u_fps", []() { return ML_Engine.time().frameRate(); }
		});
		
		ML_Content.insert<Uniform>("u_time", new uni_float_clbk
		{ 
			"u_time", []() { return ML_Engine.time().totalTime(); }
		});

		
		// Run Load Script
		/* * * * * * * * * * * * * * * * * * * * */
		if (Script load { m_prefs.get_string("Engine", "load_script", "") })
		{
			load.execute();
		}

		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (Ref<Image> icon { "_app_icon_" })
		{
			m_window.setIcon(icon->width(), icon->height(), icon->data());
		}
	}

	void Engine::onBeginStep(const BeginStepEvent & ev)
	{
		m_time.beginStep();
		m_window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Window Title
		static const String original_title { m_window.getTitle() };
		static Timer tm {};
		static float_t dt { 0 };
		if (tm.elapsed().count() > 0.5)
		{
			dt = m_time.deltaTime();
			tm.reset();
		}
		m_window.setTitle(String{ "{0} | {1} | {2} | {3}s/frame" }.format(
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
		m_window.makeContextCurrent();
		m_window.swapBuffers();
		m_time.endStep();
	}

	void Engine::onUnload(const UnloadEvent & ev)
	{
		ML_Content.dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
		m_window.dispose();
		ML_Lua.dispose();
		ML_Py.dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}