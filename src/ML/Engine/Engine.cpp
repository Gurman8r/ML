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
#include <ML/Engine/PluginLoader.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/GLM.hpp>
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

	Engine::Engine(EventSystem & eventSystem)	
		: I_EventListener { eventSystem }
	{
		eventSystem.addListener(EnterEvent		::ID, this);
		eventSystem.addListener(LoadEvent		::ID, this);
		eventSystem.addListener(StartEvent		::ID, this);
		eventSystem.addListener(BeginLoopEvent	::ID, this);
		eventSystem.addListener(UpdateEvent		::ID, this);
		eventSystem.addListener(BeginDrawEvent	::ID, this);
		eventSystem.addListener(DrawEvent		::ID, this);
		eventSystem.addListener(EndDrawEvent	::ID, this);
		eventSystem.addListener(EndLoopEvent	::ID, this);
		eventSystem.addListener(UnloadEvent		::ID, this);
		eventSystem.addListener(ExitEvent		::ID, this);
		eventSystem.addListener(CommandEvent	::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID		: if (auto ev = value.as<EnterEvent>())		return onEnter(*ev);
		case LoadEvent::ID		: if (auto ev = value.as<LoadEvent>())		return onLoad(*ev);
		case StartEvent::ID		: if (auto ev = value.as<StartEvent>())		return onStart(*ev);
		case BeginLoopEvent::ID	: if (auto ev = value.as<BeginLoopEvent>())	return onBeginFrame(*ev);
		case UpdateEvent::ID	: if (auto ev = value.as<UpdateEvent>())	return onUpdate(*ev);
		case BeginDrawEvent::ID	: if (auto ev = value.as<BeginDrawEvent>())	return onBeginDraw(*ev);
		case DrawEvent::ID		: if (auto ev = value.as<DrawEvent>())		return onDraw(*ev);
		case EndDrawEvent::ID	: if (auto ev = value.as<EndDrawEvent>())	return onEndDraw(*ev);
		case EndLoopEvent::ID	: if (auto ev = value.as<EndLoopEvent>())	return onEndFrame(*ev);
		case UnloadEvent::ID	: if (auto ev = value.as<UnloadEvent>())	return onUnload(*ev);
		case ExitEvent::ID		: if (auto ev = value.as<ExitEvent>())		return onExit(*ev);

		case CommandEvent::ID:
			if (auto ev = value.as<CommandEvent>()) { ML_CommandRegistry.execute(ev->cmd); }
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Initialize Python
		if (!ML_Py.init(ML_ARGV[0], ML_FS.pathTo(ev.prefs.get_string(
			"Engine", "python_home", ""
		))))
		{
			Debug::logError("Failed initializing Python");
		}

		// Run Boot Script
		if (Script boot { ev.prefs.get_string("Engine", "boot_script", "") })
		{
			boot.execute();
		}

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ev.window.create(
			ev.prefs.get_string	("Window", "title",				""), { 
			ev.prefs.get_uint	("Window", "width",				1280),
			ev.prefs.get_uint	("Window", "height",			720),
			ev.prefs.get_uint	("Window", "bits_per_pixel",	32) }, {
			ev.prefs.get_bool	("Window", "resizable",			true),
			ev.prefs.get_bool	("Window", "visible",			false),
			ev.prefs.get_bool	("Window", "decorated",			true),
			ev.prefs.get_bool	("Window", "focused",			true),
			ev.prefs.get_bool	("Window", "auto_iconify",		true),
			ev.prefs.get_bool	("Window", "floating",			false),
			ev.prefs.get_bool	("Window", "maximized",			true) }, {
			ev.prefs.get_uint	("Window", "major_version",		3),
			ev.prefs.get_uint	("Window", "minor_version",		3),
			ev.prefs.get_uint	("Window", "context_profile",	ContextSettings::Compat),
			ev.prefs.get_uint	("Window", "depth_bits",		24),
			ev.prefs.get_uint	("Window", "stencil_bits",		8),
			ev.prefs.get_bool	("Window", "multisample",		false),
			ev.prefs.get_bool	("Window", "srgb_capable",		false)
		}))
		{
			Debug::fatal("Failed Creating Window");
		}
		else if (ev.prefs.get_bool("Window", "fullscreen", false))
		{
			ev.window.setFullscreen(true);
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Defaults
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Texture>("default_texture")->loadFromImage(
			ML_Content.create<Image>("default_image")->update(
				{ 512, 512 }, 4, Color32::magenta
			)
		);

		ML_Content.create<Model>("default_triangle")->loadFromMemory(
			geo::tri::vertices,
			geo::tri::indices
		);
		ML_Content.create<Model>("default_quad")->loadFromMemory(
			geo::quad::vertices,
			geo::quad::indices
		);
		ML_Content.create<Model>("default_cube")->loadFromMemory(
			geo::cube::vertices,
			geo::cube::indices
		);
		ML_Content.create<Model>("default_skybox")->loadFromMemory(
			geo::sky::vertices
		);

		ML_Content.insert<Uniform>("u_cursor",	new uni_vec2_ptr ("u_cursor",	&m_cursorPos));
		ML_Content.insert<Uniform>("u_delta",	new uni_float_ptr("u_delta",	&m_deltaTime));
		ML_Content.insert<Uniform>("u_frame",	new uni_int_ptr	 ("u_frame",	&m_frameCount));
		ML_Content.insert<Uniform>("u_fps",	new uni_float_ptr("u_fps",	&m_frameRate));
		ML_Content.insert<Uniform>("u_time",	new uni_float_ptr("u_time",	&m_totalTime));
		ML_Content.insert<Uniform>("u_viewport",	new uni_vec2_ptr ("u_viewport",		&m_viewport));

		
		// Run Load Script
		/* * * * * * * * * * * * * * * * * * * * */
		if (Script load { ev.prefs.get_string("Engine", "load_script", "") })
		{
			load.execute();
		}

		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (Ref<Image> icon { "icon" })
		{
			ev.window.setIcon(icon->width(), icon->height(), icon->data());
		}
	}

	void Engine::onStart(const StartEvent & ev)
	{
	}

	void Engine::onBeginFrame(const BeginLoopEvent & ev)
	{
		ev.time.beginLoop();
		ev.window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Default Uniforms
		m_cursorPos		= (vec2)ev.window.getCursorPos();
		m_deltaTime		= ev.time.elapsed().delta();
		m_frameCount	= m_frameCount + 1;
		m_frameRate		= (float_t)ev.time.frameRate();
		m_viewport		= (vec2)ev.window.size();
		m_totalTime		= ev.time.total().delta();

		// Update Window Title
		static const String original_title { ev.window.title() };
		ev.window.setTitle(String("{0} | {1} | {2} | {3} ms/frame").format(
			original_title,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ev.time.elapsed().delta()
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

	void Engine::onEndFrame(const EndLoopEvent & ev)
	{
		ev.window.makeContextCurrent();
		ev.window.swapBuffers();
		ev.time.endLoop();
	}

	void Engine::onUnload(const UnloadEvent & ev)
	{
		ML_Content.dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
		ev.window.dispose();
		ML_Lua.dispose();
		ML_Py.dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}