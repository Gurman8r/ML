#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Audio/Audio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine(
		EventSystem		& eventSystem, 
		Preferences		& prefs, 
		Resources		& resources,
		RenderWindow	& window,
		NetClient		& client,
		NetServer		& server,
		Audio			& audio
	)
		: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_resources	(resources)
		, m_window		(window)
		, m_client		(client)
		, m_server		(server)
		, m_audio		(audio)
		, m_app			(NULL)
		, m_mainTimer	()
		, m_loopTimer	()
		, m_frameTime	(0)
		, m_frameCounter(0)
		, m_frameRate	(0)
		, m_nextSecond	(0.0f)
		, m_prevSecond	(0.0f)
		, m_netMode		(0)
	{
		/* Setup Events */
		eventSystem.addListener(EngineEvent::EV_Enter,		this);
		eventSystem.addListener(EngineEvent::EV_Load,		this);
		eventSystem.addListener(EngineEvent::EV_Start,		this);
		eventSystem.addListener(EngineEvent::EV_Update,		this);
		eventSystem.addListener(EngineEvent::EV_Draw,		this);
		eventSystem.addListener(EngineEvent::EV_Unload,		this);
		eventSystem.addListener(EngineEvent::EV_Shutdown,	this);

		/* Start Main Timer */
		m_mainTimer.start();

		/* Load Preferences */
		m_netMode = m_prefs.GetInt("Engine", "net_mode", 0);
	}

	Engine::~Engine()
	{
		/* Stop Main Timer */
		m_mainTimer.stop();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case EngineEvent::EV_Enter	: break;
		case EngineEvent::EV_Load	: break;
		case EngineEvent::EV_Start	: break;
		case EngineEvent::EV_Update	: break;
		case EngineEvent::EV_Draw	: break;
		case EngineEvent::EV_Unload	: break;

			// Shutdown Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Shutdown:
			if (auto ev = value->as<ShutdownEvent>())
			{
				if (isRunning()) { m_window.close(); }
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Application * Engine::launchApp(Application * value)
	{
		return ((!isRunning()) ? (m_app = value) : (NULL));
	}

	int32_t Engine::freeApp(Application * value)
	{
		if (m_app && (m_app == value))
		{
			delete m_app;
		}
		return ((m_app) ? (EXIT_FAILURE) : (EXIT_SUCCESS));
	}

	bool Engine::isRunning() const
	{
		return ((m_app) && (m_window.isOpen()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		// Start Loop Timer
		m_loopTimer.start();

		// Poll Window Events
		m_window.pollEvents();
	}

	void Engine::endFrame()
	{
		// Swap Buffers
		m_window.swapBuffers();

		// Stop Loop Timer
		m_frameTime = m_loopTimer.stop().elapsed();
		m_frameCounter++;
		if (((m_nextSecond += m_frameTime.delta()) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			m_frameRate = m_frameCounter;
			m_frameCounter = 0;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Audio			& Engine::audio()		const { return m_audio; }
	NetClient		& Engine::client()		const { return m_client; }
	EventSystem		& Engine::eventSystem() const { return m_eventSystem; }
	Preferences		& Engine::prefs()		const { return m_prefs; }
	Resources		& Engine::resources()	const { return m_resources; }
	NetServer		& Engine::server()		const { return m_server; }
	RenderWindow	& Engine::window()		const { return m_window; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}