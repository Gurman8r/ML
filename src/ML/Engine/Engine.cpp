#include <ML/Engine/Engine.hpp>
#include <ML/Audio/Audio.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Engine/Application.hpp>
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

	Engine::Engine(
		EventSystem		& eventSystem, 
		Preferences		& prefs, 
		GameTime		& time,
		Resources		& resources,
		RenderWindow	& window)	
		: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_time		(time)
		, m_resources	(resources)
		, m_window		(window)
	{
		// Setup Events
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.addListener(EngineEvent::EV_Enter,		this);
		eventSystem.addListener(EngineEvent::EV_Load,		this);
		eventSystem.addListener(EngineEvent::EV_Start,		this);
		eventSystem.addListener(EngineEvent::EV_Update,		this);
		eventSystem.addListener(EngineEvent::EV_Draw,		this);
		eventSystem.addListener(EngineEvent::EV_Exit,	this);
		eventSystem.addListener(EngineEvent::EV_Shutdown,		this);

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (window.create(
			prefs.GetString	("Window", "title",			"Title"), { {
			prefs.GetUint	("Window", "width",			1280),
			prefs.GetUint	("Window", "height",		720) },
			prefs.GetUint	("Window", "colorDepth",	32) },
			prefs.GetUint	("Window", "style",			Window::Default), {
			prefs.GetUint	("Window", "majorVersion",	3),
			prefs.GetUint	("Window", "minorVersion",	3),
			prefs.GetUint	("Window", "profile",		Context::Compat),
			prefs.GetUint	("Window", "depthBits",		24),
			prefs.GetUint	("Window", "stencilBits",	8),
			prefs.GetBool	("Window", "multisample",	false),
			prefs.GetBool	("Window", "srgbCapable",	false)
		}))
		{
			window.maximize();
			window.seCursorMode(Cursor::Normal);
			window.setPosition((Screen::desktop().resolution - window.getSize()) / 2);
			window.setViewport(vec2i::Zero, window.getFrameSize());
		}
		else
		{
			Debug::fatal("Failed Creating Window");
		}

		// Setup Network
		/* * * * * * * * * * * * * * * * * * * * */
		//m_netType = m_prefs.GetInt("Engine", "net_mode", 0);
		//switch (m_netType)
		//{
		//case NetServer::ID:
		//	if (m_server.setup())
		//	{
		//		if (m_server.start({ ML_LOCALHOST, ML_DEFAULT_PORT }, ML_MAX_CLIENTS))
		//		{
		//			Debug::log("Server Started: {0}", m_server.getMyAddress());
		//		}
		//	}
		//	break;
		//case NetClient::ID:
		//	if (m_client.setup())
		//	{
		//		if (m_client.connect({ ML_LOCALHOST, ML_DEFAULT_PORT }))
		//		{
		//			Debug::log("Client Connected: {0}", m_client.getMyAddress());
		//		}
		//	}
		//	break;
		//}
	}

	Engine::~Engine() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const IEvent * value)
	{
		switch (*value)
		{
			// Enter Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Enter:
			if (auto ev = value->as<EnterEvent>())
			{
			}
			break;

			// Load Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Load: 
			if (auto ev = value->as<LoadEvent>())
			{
				// Load Default Meshes
				/* * * * * * * * * * * * * * * * * * * * */
				ev->resources.meshes.load("default_triangle")->loadFromMemory(
					Shapes::Triangle::Vertices,
					Shapes::Triangle::Indices
				);
				ev->resources.meshes.load("default_quad")->loadFromMemory(
					Shapes::Quad::Vertices,
					Shapes::Quad::Indices
				);
				ev->resources.meshes.load("default_cube")->loadFromMemory(
					Shapes::Cube::Vertices,
					Shapes::Cube::Indices
				);
				ev->resources.meshes.load("default_skybox")->loadFromMemory(
					Shapes::Sky::Vertices
				);

				// Load Default Models
				/* * * * * * * * * * * * * * * * * * * * */
				ev->resources.models.load("default_triangle")->loadFromMemory(
					*ev->resources.meshes.get("default_triangle")
				);
				ev->resources.models.load("default_quad")->loadFromMemory(
					*ev->resources.meshes.get("default_quad")
				);
				ev->resources.models.load("default_cube")->loadFromMemory(
					*ev->resources.meshes.get("default_cube")
				);
				ev->resources.models.load("default_skybox")->loadFromMemory(
					*ev->resources.meshes.get("default_skybox")
				);

				// Load Resource Manifest
				/* * * * * * * * * * * * * * * * * * * * */
				if (!ev->resources.loadFromFile(ML_FS.getPathTo(
					ev->prefs.GetString("Engine", "manifest", "../../../assets/manifest.txt")
				)))
				{
					Debug::logError("Failed Loading Manifest");
				}
			}
			break;

			// Start Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Start:
			if (auto ev = value->as<StartEvent>())
			{
			}
			break;

			// Update Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Update:
			if (auto ev = value->as<UpdateEvent>())
			{
			}
			break;

			// Draw Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Draw:
			if (auto ev = value->as<DrawEvent>())
			{
			}
			break;
		
			// Shutdown Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Exit:
			if (auto ev = value->as<ExitEvent>())
			{
				ev->resources.dispose();
			}
			break;

			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Shutdown:
			if (auto ev = value->as<ShutdownEvent>())
			{
				m_window.close();
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		// Start Loop Timer
		m_time.beginLoop();

		// Update Network
		//switch (m_netType)
		//{
		//case NetServer::ID: m_server.poll(); break;
		//case NetClient::ID: m_client.poll(); break;
		//}

		// Poll Window Events
		m_window.pollEvents();
	}

	void Engine::endFrame()
	{
		// Swap Buffers
		m_window.swapBuffers();

		// Stop Loop Timer
		m_time.endLoop();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventSystem		& Engine::eventSystem() const { return m_eventSystem; }
	Preferences		& Engine::prefs()		const { return m_prefs; }
	Resources		& Engine::resources()	const { return m_resources; }
	GameTime		& Engine::time()		const { return m_time; }
	RenderWindow	& Engine::window()		const { return m_window; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}