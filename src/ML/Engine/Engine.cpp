#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
		: m_app			(NULL)
		, m_mainTimer	()
		, m_loopTimer	()
		, m_frameTime	(0)
		, m_frameCount	(0)
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

		m_frameTime = m_loopTimer.stop().elapsed();

		m_frameCount++;

		if (((m_nextSecond += m_frameTime.delta()) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			
			m_frameRate = m_frameCount;
			
			m_frameCount = 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}