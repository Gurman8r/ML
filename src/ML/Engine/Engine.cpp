#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Graphics/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Engine::Engine(EventSystem & eventSystem, Preferences & prefs, Resources & resources)
		: m_eventSystem	(eventSystem)
		, m_prefs		(prefs)
		, m_resources	(resources)
		, m_app			(NULL)
		, m_mainTimer	()
		, m_loopTimer	()
		, m_elapsed		(0)
		, m_frameCounter(0)
		, m_frameRate	(0)
		, m_nextSecond	(0.0f)
		, m_prevSecond	(0.0f)
	{
		m_mainTimer.start();
	}

	Engine::~Engine()
	{
		m_mainTimer.stop();
	}

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		m_loopTimer.start();

		if (m_app)
		{
			m_app->pollEvents();
		}
	}

	void Engine::endFrame()
	{
		if (m_app)
		{
			m_app->swapBuffers();
		}

		m_elapsed = m_loopTimer.stop().elapsed();

		m_frameCounter++;

		if (((m_nextSecond += m_elapsed.delta()) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			
			m_frameRate = m_frameCounter;
			
			m_frameCounter = 0;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	EventSystem		& Engine::eventSystem() const { return m_eventSystem; }
	Preferences		& Engine::prefs()		const { return m_prefs; }
	Resources		& Engine::resources()	const { return m_resources; }
	RenderWindow	& Engine::window()		const { return (RenderWindow &)(*m_app); }

	/* * * * * * * * * * * * * * * * * * * * */
}