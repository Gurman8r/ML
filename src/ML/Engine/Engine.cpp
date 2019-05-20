#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine(
		EventSystem & eventSystem, 
		Preferences & prefs, 
		Resources	& resources,
		NetClient	& client,
		NetServer	& server
	)
		: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_resources	(resources)
		, m_client		(client)
		, m_server		(server)
		, m_app			(NULL)
		, m_mainTimer	()
		, m_loopTimer	()
		, m_frameTime	(0)
		, m_frameCounter(0)
		, m_frameRate	(0)
		, m_nextSecond	(0.0f)
		, m_prevSecond	(0.0f)
		, m_isClient	(false)
		, m_isServer	(false)
	{
		m_mainTimer.start();
	}

	Engine::~Engine()
	{
		m_mainTimer.stop();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Application * Engine::launchApp(Application * value)
	{
		return ((!isRunning()) 
			? (m_app = value) 
			: (NULL)
		);
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
		return ((m_app) && (m_app->isOpen()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		// Start Loop Timer
		m_loopTimer.start();

		// Update Network
		if (m_isServer)
		{
			m_server.poll();
		}
		else if (m_isClient)
		{
			m_client.poll();
		}

		// Poll Window Events
		if (m_app)
		{
			m_app->pollEvents();
		}
	}

	void Engine::endFrame()
	{
		// Swap Buffers
		if (m_app)
		{
			m_app->swapBuffers();
		}

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

	EventSystem		& Engine::eventSystem() const { return m_eventSystem; }
	NetClient		& Engine::client()		const { return m_client; }
	Preferences		& Engine::prefs()		const { return m_prefs; }
	Resources		& Engine::resources()	const { return m_resources; }
	NetServer		& Engine::server()		const { return m_server; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}