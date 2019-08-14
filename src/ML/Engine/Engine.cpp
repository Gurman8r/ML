#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Worker.hpp>
#include <ML/Engine/Asset.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/ContentLoader.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/PluginLoader.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Script/ScriptEvents.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine(EventSystem & eventSystem)	
		: I_EventListener { eventSystem }
	{
		eventSystem.addListener(EnterEvent		::ID, this);
		eventSystem.addListener(LoadEvent		::ID, this);
		eventSystem.addListener(StartEvent		::ID, this);
		eventSystem.addListener(BeginFrameEvent	::ID, this);
		eventSystem.addListener(UpdateEvent		::ID, this);
		eventSystem.addListener(BeginDrawEvent	::ID, this);
		eventSystem.addListener(DrawEvent		::ID, this);
		eventSystem.addListener(EndDrawEvent	::ID, this);
		eventSystem.addListener(EndFrameEvent	::ID, this);
		eventSystem.addListener(UnloadEvent		::ID, this);
		eventSystem.addListener(ExitEvent		::ID, this);
		eventSystem.addListener(CommandEvent	::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID		: if (auto ev = value.as<EnterEvent>())		return onEnter(*ev);
		case LoadEvent::ID		: if (auto ev = value.as<LoadEvent>())		return onLoad(*ev);
		case StartEvent::ID		: if (auto ev = value.as<StartEvent>())		return onStart(*ev);
		case BeginFrameEvent::ID: if (auto ev = value.as<BeginFrameEvent>())return onBeginFrame(*ev);
		case UpdateEvent::ID	: if (auto ev = value.as<UpdateEvent>())	return onUpdate(*ev);
		case BeginDrawEvent::ID	: if (auto ev = value.as<BeginDrawEvent>())	return onBeginDraw(*ev);
		case DrawEvent::ID		: if (auto ev = value.as<DrawEvent>())		return onDraw(*ev);
		case EndDrawEvent::ID	: if (auto ev = value.as<EndDrawEvent>())	return onEndDraw(*ev);
		case EndFrameEvent::ID	: if (auto ev = value.as<EndFrameEvent>())	return onEndFrame(*ev);
		case UnloadEvent::ID	: if (auto ev = value.as<UnloadEvent>())	return onUnload(*ev);
		case ExitEvent::ID		: if (auto ev = value.as<ExitEvent>())		return onExit(*ev);

		case CommandEvent::ID:
			if (auto ev = value.as<CommandEvent>())
			{
				if (auto args = alg::tokenize(ev->cmd, " "))
				{
					if (auto cmd = ML_CommandRegistry.find_by_name(args.front()))
					{
						cmd->execute(args);
						break;
					}
				}

				Var v { ML_Interpreter.execCommand(ev->cmd) };
				if (v.isErrorType()) { cout << v.errorValue() << endl; }
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (ev.window.create(
			ev.prefs.get_string	("Window", "window_title",		"MemeLib"), { 
			ev.prefs.get_uint	("Window", "width",				1280),
			ev.prefs.get_uint	("Window", "height",			720),
			ev.prefs.get_uint	("Window", "color_depth",		32) }, {
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
			ev.window.seCursorMode(Cursor::Mode::Normal);

			ev.window.setViewport(vec2i { 0, 0 }, ev.window.getFrameSize());

			if (ev.window.getStyle().maximized)
			{
				ev.window.maximize();
			}
			else
			{
				// centered
				ev.window.setPosition(
					(vec2i)(VideoMode::get_desktop_mode().resolution - ev.window.getSize()) / 2
				);
			}
		}
		else
		{
			Debug::fatal("Failed Creating Window");
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Default Models
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Model>("default_triangle")->loadFromMemory(
			geo::tri::contiguous,
			geo::tri::indices
		);
		ML_Content.create<Model>("default_quad")->loadFromMemory(
			geo::quad::contiguous,
			geo::quad::indices
		);
		ML_Content.create<Model>("default_cube")->loadFromMemory(
			geo::cube::contiguous,
			geo::cube::indices
		);
		ML_Content.create<Model>("default_skybox")->loadFromMemory(
			geo::sky::contiguous
		);


		// Load Default Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<uni_vec2_ptr>	("CURSOR_POS",	"u_cursorPos",  &m_cursorPos);
		ML_Content.create<uni_float_ptr>("DELTA_TIME",	"u_deltaTime",  &m_deltaTime);
		ML_Content.create<uni_int_ptr>	("FRAME_COUNT",	"u_frameCount", &m_frameCount);
		ML_Content.create<uni_float_ptr>("FRAME_RATE",	"u_frameRate",	&m_frameRate);
		ML_Content.create<uni_float_ptr>("TOTAL_TIME",	"u_totalTime",  &m_totalTime);
		ML_Content.create<uni_vec2_ptr>	("VIEWPORT",	"u_viewport",	&m_viewport);

		// Load Asset Lists
		/* * * * * * * * * * * * * * * * * * * * */
		static ContentLoader loader;
		if (loader.loadFromFile(ML_FS.pathTo(ev.prefs.get_string(
			"Engine", "asset_lists", ""
		))))
		{
			loader.loadAll(true);
		}
		else
		{
			Debug::logError("Failed Loading Manifest");
		}
	}

	void Engine::onStart(const StartEvent & ev)
	{
		// Set Window Icon
		if (m_icon) { ev.window.setIcons({ (*m_icon) }); }
	}

	void Engine::onBeginFrame(const BeginFrameEvent & ev)
	{
		ev.time.beginLoop();
		ev.window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Default Uniforms
		m_cursorPos		= (vec2)ev.window.getCursorPos();
		m_deltaTime		= ev.time.elapsed().delta<Millisec>();
		m_frameCount	= m_frameCount + 1;
		m_frameRate		= (float_t)ev.time.frameRate();
		m_viewport		= (vec2)ev.window.getSize();
		m_totalTime		= ev.time.total().delta<Millisec>();

		// Update Window Title
		static const String original_title { ev.window.getTitle() };
		ev.window.setTitle(String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
			original_title,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ev.time.elapsed().delta(),
			ev.time.frameRate()
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

	void Engine::onEndFrame(const EndFrameEvent & ev)
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
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}