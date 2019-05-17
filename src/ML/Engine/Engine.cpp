#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Core/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
		: m_app			(NULL)
		, m_timer		(m_timer)
		, m_elapsed	(m_elapsed)	
		, m_frameCount	(m_frameCount)
		, m_frameRate	(m_frameRate)	
		, m_nextSecond	(m_nextSecond)
		, m_prevSecond	(m_prevSecond)
	{
	}

	Engine::~Engine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

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
		return ((m_app) && (m_app->isOpen()));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		m_timer.start();

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

		m_elapsed = m_timer.stop().elapsed();

		m_frameCount++;

		if (((m_nextSecond += m_elapsed.delta()) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			
			m_frameRate = m_frameCount;
			
			m_frameCount = 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}