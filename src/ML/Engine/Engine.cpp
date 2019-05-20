#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/GameTime.hpp>
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
		GameTime		& time,
		Resources		& resources,
		RenderWindow	& window,
		NetClient		& client,
		NetServer		& server,
		Audio			& audio
	)	: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_time		(time)
		, m_resources	(resources)
		, m_window		(window)
		, m_client		(client)
		, m_server		(server)
		, m_audio		(audio)
		, m_app			(NULL)
		, m_networkMode	(0)
	{
		/* Setup Events */
		eventSystem.addListener(EngineEvent::EV_Exit,		this);

		/* Setup Network */ 
		m_networkMode = m_prefs.GetInt("Engine", "net_mode", 0);
		switch (m_networkMode)
		{
		case ml::NetServer::Mode:
			if (m_server.setup())
			{
				if (m_server.start({ ML_LOCALHOST, ML_DEFAULT_PORT }, ML_MAX_CLIENTS))
				{
					ml::Debug::log("Server Started: {0}", m_server.getMyAddress());
				}
			}
			break;
		case ml::NetClient::Mode:
			if (m_client.setup())
			{
				if (m_client.connect({ ML_LOCALHOST, ML_DEFAULT_PORT }))
				{
					ml::Debug::log("Client Connected: {0}", m_client.getMyAddress());
				}
			}
			break;
		}
	}

	Engine::~Engine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const IEvent * value)
	{
		switch (*value)
		{
			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Exit:
			if (auto ev = value->as<ExitEvent>())
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Engine::isRunning() const
	{
		return ((m_app) && (m_window.isOpen()));
	}

	void Engine::beginFrame()
	{
		// Start Loop Timer
		m_time.beginLoop();

		// Update Network
		switch (m_networkMode)
		{
		case ml::NetServer::Mode: m_server.poll(); break;
		case ml::NetClient::Mode: m_client.poll(); break;
		}

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

	Audio			& Engine::audio()		const { return m_audio; }
	NetClient		& Engine::client()		const { return m_client; }
	EventSystem		& Engine::eventSystem() const { return m_eventSystem; }
	Preferences		& Engine::prefs()		const { return m_prefs; }
	Resources		& Engine::resources()	const { return m_resources; }
	NetServer		& Engine::server()		const { return m_server; }
	RenderWindow	& Engine::window()		const { return m_window; }
	GameTime		& Engine::time()		const { return m_time;	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}