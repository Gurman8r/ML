#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Core/EventSystem.hpp>
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
		eventSystem.addListener(KeyEvent::ID,			this);
	}

	Engine::~Engine() {}

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
					Debug::logError(v.errorValue());
				}
			}
			break;

			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				// Exit (Escape)
				if (ev->getPress(KeyCode::Escape))
				{
					eventSystem().fireEvent(WindowKillEvent());
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Get Boot Script
		/* * * * * * * * * * * * * * * * * * * * */
		m_bootScript = ev.prefs.GetString("Engine", "boot_script", "");

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

		// Load Resource Manifest
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_Content.loadFromFile(ML_FS.getPathTo(ev.prefs.GetString(
			"Engine",
			"import_list",
			"../../../assets/data/manifest.txt"
		))))
		{
			Debug::logError("Failed Loading Manifest");
		}
	}

	void Engine::onStart(const StartEvent & ev)
	{
		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const Image * icon = ML_Content.get<Image>("icon"))
		{
			const Image temp = Image(*icon).flipVertically();

			ev.window.setIcons({ temp });
		}

		// Run Boot Script
		/* * * * * * * * * * * * * * * * * * * * */
		if (Script * scr = ML_Content.get<Script>(m_bootScript))
		{
			if (!(scr->buildAndRun(Arguments(__argc, __argv))))
			{
				Debug::logError("Failed Running \'{0}\'", scr->path());
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