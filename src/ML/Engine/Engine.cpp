#include <ML/Engine/Engine.hpp>
#include <ML/Audio/Audio.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Shapes.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Core/EventSystem.hpp>

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
	}

	Engine::~Engine() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event * value)
	{
		switch (*value)
		{
		case EnterEvent::ID:		return onEnter		(*value->as<EnterEvent>());
		case LoadEvent::ID:			return onLoad		(*value->as<LoadEvent>());
		case StartEvent::ID:		return onStart		(*value->as<StartEvent>());
		case BeginFrameEvent::ID:	return onBeginFrame	(*value->as<BeginFrameEvent>());
		case UpdateEvent::ID:		return onUpdate		(*value->as<UpdateEvent>());
		case BeginDrawEvent::ID:	return onBeginDraw	(*value->as<BeginDrawEvent>());
		case DrawEvent::ID:			return onDraw		(*value->as<DrawEvent>());
		case EndDrawEvent::ID:		return onEndDraw	(*value->as<EndDrawEvent>());
		case EndFrameEvent::ID:		return onEndFrame	(*value->as<EndFrameEvent>());
		case UnloadEvent::ID:		return onUnload		(*value->as<UnloadEvent>());
		case ExitEvent::ID:			return onExit		(*value->as<ExitEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{	
		// Create Window
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
			if (ev.window.getStyle().maximized)
			{
				ev.window.maximize();
			}
			ev.window.seCursorMode(Cursor::Normal);
			ev.window.setPosition((VideoSettings::desktop().resolution - ev.window.getSize()) / 2);
			ev.window.setViewport(vec2i::Zero, ev.window.getFrameSize());
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
		ev.resources.meshes.load("default_triangle")->loadFromMemory(
			Shapes::Triangle::Vertices,
			Shapes::Triangle::Indices
		);
		ev.resources.meshes.load("default_quad")->loadFromMemory(
			Shapes::Quad::Vertices,
			Shapes::Quad::Indices
		);
		ev.resources.meshes.load("default_cube")->loadFromMemory(
			Shapes::Cube::Vertices,
			Shapes::Cube::Indices
		);
		ev.resources.meshes.load("default_skybox")->loadFromMemory(
			Shapes::Sky::Vertices
		);

		// Load Default Models
		/* * * * * * * * * * * * * * * * * * * * */
		ev.resources.models.load("default_triangle")->loadFromMemory(
			*ev.resources.meshes.get("default_triangle")
		);
		ev.resources.models.load("default_quad")->loadFromMemory(
			*ev.resources.meshes.get("default_quad")
		);
		ev.resources.models.load("default_cube")->loadFromMemory(
			*ev.resources.meshes.get("default_cube")
		);
		ev.resources.models.load("default_skybox")->loadFromMemory(
			*ev.resources.meshes.get("default_skybox")
		);

		// Load Resource Manifest
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ev.resources.loadFromFile(ML_FS.getPathTo(ev.prefs.GetString(
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
	}

	void Engine::onBeginFrame(const BeginFrameEvent & ev)
	{
		ev.time.beginLoop();
		ev.window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
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
		ev.resources.dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}