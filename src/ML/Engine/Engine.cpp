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
		RenderWindow	& window)	
		: m_app(NULL)
		, m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_time		(time)
		, m_resources	(resources)
		, m_window		(window)
	{
		/* Setup Events */
		eventSystem.addListener(EngineEvent::EV_Enter,		this);
		eventSystem.addListener(EngineEvent::EV_Load,		this);
		eventSystem.addListener(EngineEvent::EV_Start,		this);
		eventSystem.addListener(EngineEvent::EV_Update,		this);
		eventSystem.addListener(EngineEvent::EV_Draw,		this);
		eventSystem.addListener(EngineEvent::EV_Shutdown,	this);
		eventSystem.addListener(EngineEvent::EV_Exit,		this);

		/* Setup Network */ 
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case EngineEvent::EV_Enter:		break;
		case EngineEvent::EV_Load:		break;
		case EngineEvent::EV_Start:		break;
		case EngineEvent::EV_Update:	break;
		case EngineEvent::EV_Draw:		break;
		case EngineEvent::EV_Shutdown:	break;

			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Exit:
			if (this->isRunning())
			{
				m_window.close();
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