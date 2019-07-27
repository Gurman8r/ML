#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Worker.hpp>
#include <ML/Engine/Asset.hpp>
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
		: EventListener(eventSystem)
	{
		eventSystem.addListener(EnterEvent::ID,			this);
		eventSystem.addListener(LoadEvent::ID,			this);
		eventSystem.addListener(StartEvent::ID,			this);
		eventSystem.addListener(BeginFrameEvent::ID,	this);
		eventSystem.addListener(UpdateEvent::ID,		this);
		eventSystem.addListener(BeginDrawEvent::ID,		this);
		eventSystem.addListener(DrawEvent::ID,			this);
		eventSystem.addListener(EndDrawEvent::ID,		this);
		eventSystem.addListener(EndFrameEvent::ID,		this);
		eventSystem.addListener(UnloadEvent::ID,		this);
		eventSystem.addListener(ExitEvent::ID,			this);
		eventSystem.addListener(CommandEvent::ID,		this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event * value)
	{
		switch (*value)
		{
		case EnterEvent::ID		:	return onEnter		(*value->as<EnterEvent>());
		case LoadEvent::ID		:	return onLoad		(*value->as<LoadEvent>());
		case StartEvent::ID		:	return onStart		(*value->as<StartEvent>());
		case BeginFrameEvent::ID:	return onBeginFrame	(*value->as<BeginFrameEvent>());
		case UpdateEvent::ID	:	return onUpdate		(*value->as<UpdateEvent>());
		case BeginDrawEvent::ID	:	return onBeginDraw	(*value->as<BeginDrawEvent>());
		case DrawEvent::ID		:	return onDraw		(*value->as<DrawEvent>());
		case EndDrawEvent::ID	:	return onEndDraw	(*value->as<EndDrawEvent>());
		case EndFrameEvent::ID	:	return onEndFrame	(*value->as<EndFrameEvent>());
		case UnloadEvent::ID	:	return onUnload		(*value->as<UnloadEvent>());
		case ExitEvent::ID		:	return onExit		(*value->as<ExitEvent>());

			// Command Event
			/* * * * * * * * * * * * * * * * * * * * */
		case CommandEvent::ID:
			if (auto ev = value->as<CommandEvent>())
			{
				Var v;
				if ((v = ML_Interpreter.execCommand(ev->cmd)).isErrorType())
				{
					cout << v.errorValue() << endl;
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Get Boot Script Name
		m_script.name = ev.prefs.GetString("Engine", "boot_script", "");

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (ev.window.create(
			ev.prefs.GetString	("Window", "title",			"Title"), { 
			ev.prefs.GetUint	("Window", "width",			1280),
			ev.prefs.GetUint	("Window", "height",		720),
			ev.prefs.GetUint	("Window", "colorDepth",	32) }, {
			ev.prefs.GetBool	("Window", "resizable",		true),
			ev.prefs.GetBool	("Window", "visible",		false),
			ev.prefs.GetBool	("Window", "decorated",		true),
			ev.prefs.GetBool	("Window", "focused",		true),
			ev.prefs.GetBool	("Window", "autoIconify",	true),
			ev.prefs.GetBool	("Window", "floating",		false),
			ev.prefs.GetBool	("Window", "maximized",		true) }, {
			ev.prefs.GetUint	("Window", "majorVersion",	3),
			ev.prefs.GetUint	("Window", "minorVersion",	3),
			ev.prefs.GetUint	("Window", "profile",		ContextSettings::Compat),
			ev.prefs.GetUint	("Window", "depthBits",		24),
			ev.prefs.GetUint	("Window", "stencilBits",	8),
			ev.prefs.GetBool	("Window", "multisample",	false),
			ev.prefs.GetBool	("Window", "srgbCapable",	false)
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
					(vec2i)(VideoSettings::desktop().resolution - ev.window.getSize()) / 2
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
		// Load Default Meshes
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Mesh>("default_triangle")->loadFromMemory(
			geo::tri::contiguous,
			geo::tri::indices
		);
		ML_Content.create<Mesh>("default_quad")->loadFromMemory(
			geo::quad::contiguous,
			geo::quad::indices
		);
		ML_Content.create<Mesh>("default_cube")->loadFromMemory(
			geo::cube::contiguous,
			geo::cube::indices
		);
		ML_Content.create<Mesh>("default_skybox")->loadFromMemory(
			geo::sky::contiguous
		);

		// Load Default Models
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Model>("default_triangle")->loadFromMemory(
			*ML_Content.get<Mesh>("default_triangle")
		);
		ML_Content.create<Model>("default_quad")->loadFromMemory(
			*ML_Content.get<Mesh>("default_quad")
		);
		ML_Content.create<Model>("default_cube")->loadFromMemory(
			*ML_Content.get<Mesh>("default_cube")
		);
		ML_Content.create<Model>("default_skybox")->loadFromMemory(
			*ML_Content.get<Mesh>("default_skybox")
		);

		// Load Default Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<uni_vec2_ptr>	("CURSOR_POS",	"u_cursorPos",  &m_cursorPos);
		ML_Content.create<uni_float_ptr>("DELTA_TIME",	"u_deltaTime",  &m_deltaTime);
		ML_Content.create<uni_int_ptr>	("FRAME_COUNT",	"u_frameCount", &m_frameCount);
		ML_Content.create<uni_float_ptr>("FRAME_RATE", "u_frameRate",	&m_frameRate);
		ML_Content.create<uni_float_ptr>("TOTAL_TIME",	"u_totalTime",  &m_totalTime);
		ML_Content.create<uni_vec2_ptr>	("VIEWPORT",	"u_viewport",	&m_viewport);

		// Load Content
		/* * * * * * * * * * * * * * * * * * * * */
		static ContentLoader loader;
		if (loader.loadFromFile(ML_FS.pathTo(ev.prefs.GetString(
			"Engine", "asset_lists", String()
		))))
		{
#if 1
			loader.loadAll(true);
#else
			// FIXME:
			// not working because there's only one opengl context
			static Worker worker;
			worker.launch(loader.lists().size(), [&]()
			{
				for (size_t i = 0; worker.working(); i++)
				{
					worker.process([&]() { return loader.loadElement(i); });
				}
				loader.dispose();
				worker.finalize();
				eventSystem().fireEvent(StartEvent(ev.time, ev.window));
			});
#endif
		}
		else
		{
			Debug::logError("Failed Loading Manifest");
		}
	}

	void Engine::onStart(const StartEvent & ev)
	{
		// Set Window Icon
		if (m_icon)
		{
			const Image temp = Image(*m_icon).flipVertically();
			ev.window.setIcons({ temp });
		}

		// Run Boot Script
		if (m_script)
		{
			if (!(m_script->buildAndRun(Arguments(__argc, __argv))))
			{
				Debug::logError("Failed Running \'{0}\'", m_script->path());
			}
		}
	}

	void Engine::onBeginFrame(const BeginFrameEvent & ev)
	{
		ev.time.beginLoop();
		ev.window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Uniforms
		m_cursorPos = (vec2)ev.window.getCursorPos();
		m_deltaTime = ev.time.elapsed().delta<Millisec>();
		m_frameCount++;
		m_frameRate = (float_t)ev.time.frameRate();
		m_viewport = (vec2)ev.window.getSize();
		m_totalTime = ev.time.total().delta<Millisec>();

		// Update Window Title
		static const String title(ev.window.getTitle());
		ev.window.setTitle(String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
			title,
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