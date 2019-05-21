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
		RenderWindow	& window)	
		: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_window		(window)
	{
		// Setup Events
		/* * * * * * * * * * * * * * * * * * * * */
		eventSystem.addListener(EngineEvent::EV_Enter, this);
		eventSystem.addListener(EngineEvent::EV_LoadContent, this);;
		eventSystem.addListener(EngineEvent::EV_Exit, this);
		eventSystem.addListener(EngineEvent::EV_Shutdown, this);

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

			// Load Content Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_LoadContent: 
			if (auto ev = value->as<LoadContentEvent>())
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
					m_prefs.GetString("Engine", "manifest", "../../../assets/manifest.txt")
				)))
				{
					Debug::logError("Failed Loading Manifest");
				}
			}
			break;

			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Exit:
			if (auto ev = value->as<ExitEvent>())
			{
				ev->resources.dispose();
			}
			break;

			// Shutdown Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Shutdown:
			m_window.close();
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}