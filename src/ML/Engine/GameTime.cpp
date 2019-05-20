#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime::GameTime()
		: m_mainTimer	()
		, m_loopTimer	()
		, m_frameTime	()
		, m_frameCount	()
		, m_frameRate	()
		, m_nextSecond	()
		, m_prevSecond	()
	{
		m_mainTimer.start();
	}

	GameTime::~GameTime()
	{
		m_mainTimer.stop();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime & GameTime::beginLoop()
	{
		m_loopTimer.start();

		return (*this);
	}

	GameTime & GameTime::endLoop()
	{
		m_frameTime = m_loopTimer.stop().elapsed();
		
		m_frameCount++;
		
		if (((m_nextSecond += m_frameTime.delta()) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			
			m_frameRate = m_frameCount;
			
			m_frameCount = 0;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}