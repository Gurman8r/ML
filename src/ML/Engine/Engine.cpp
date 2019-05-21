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
		: IEventListener(eventSystem)
		, m_prefs		(prefs)
		, m_window		(window)
	{
		// Setup Events
		eventSystem.addListener(EngineEvent::EV_Enter, this);
		eventSystem.addListener(EngineEvent::EV_LoadContent, this);;
		eventSystem.addListener(EngineEvent::EV_Exit, this);
		eventSystem.addListener(EngineEvent::EV_Shutdown, this);
	}

	Engine::~Engine() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event * value)
	{
		switch (*value)
		{
			// Enter Event
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		case EnterEvent::ID:
			if (auto ev = value->as<EnterEvent>())
			{
				// Create Window
				/* * * * * * * * * * * * * * * * * * * * */

				if (ev->window.create(
					ev->prefs.GetString	("Window", "title",			"Title"), { 
					ev->prefs.GetUint	("Window", "width",			1280),
					ev->prefs.GetUint	("Window", "height",		720),
					ev->prefs.GetUint	("Window", "colorDepth",	32) }, {
					ev->prefs.GetBool	("Window", "resizable",		true),
					ev->prefs.GetBool	("Window", "visible",		false),
					ev->prefs.GetBool	("Window", "decorated",		true),
					ev->prefs.GetBool	("Window", "focused",		true),
					ev->prefs.GetBool	("Window", "autoIconify",	true),
					ev->prefs.GetBool	("Window", "floating",		false),
					ev->prefs.GetBool	("Window", "maximized",		true) }, {
					ev->prefs.GetUint	("Window", "majorVersion",	3),
					ev->prefs.GetUint	("Window", "minorVersion",	3),
					ev->prefs.GetUint	("Window", "profile",		ContextSettings::Compat),
					ev->prefs.GetUint	("Window", "depthBits",		24),
					ev->prefs.GetUint	("Window", "stencilBits",	8),
					ev->prefs.GetBool	("Window", "multisample",	false),
					ev->prefs.GetBool	("Window", "srgbCapable",	false)
				}))
				{
					if (ev->window.getStyle().maximized)
					{
						ev->window.maximize();
					}

					ev->window.seCursorMode(Cursor::Normal);
					
					ev->window.setPosition((VideoSettings::desktop().resolution - ev->window.getSize()) / 2);
					
					ev->window.setViewport(vec2i::Zero, ev->window.getFrameSize());
				}
				else
				{
					Debug::fatal("Failed Creating Window");
				}
			}
			break;

			// Load Content Event
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		case LoadContentEvent::ID:
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
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		case ExitEvent::ID:
			if (auto ev = value->as<ExitEvent>())
			{
				ev->resources.dispose();
			}
			break;

			// Shutdown Event
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		case ShutdownEvent::ID:
			m_window.close();
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}