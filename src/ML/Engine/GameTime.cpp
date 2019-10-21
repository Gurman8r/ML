#include <ML/Engine/GameTime.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime::GameTime() 
		: m_main {}
		, m_step {}
	{
		m_main.start();
	}

	GameTime::~GameTime()
	{ 
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime & GameTime::beginStep()
	{
		m_step.start();

		return (*this);
	}

	GameTime & GameTime::endStep()
	{
		m_elapsed = m_step.stop().elapsed();

		m_frame.accum += deltaTime() - m_frame.buf[m_frame.index];
		
		m_frame.buf[m_frame.index] = deltaTime();
		
		m_frame.index = (m_frame.index + 1) % ML_ARRAYSIZE(m_frame.buf);
		
		m_frame.rate = ((m_frame.accum > 0.0f)
			? (1.0f / (m_frame.accum / (float_t)ML_ARRAYSIZE(m_frame.buf)))
			: 0.0f
		);

		m_frame.count++;

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const float_t & GameTime::totalTime() const
	{
		return (m_totalTime = (float_t)m_main.elapsed().count());
	}

	const float_t & GameTime::deltaTime() const
	{
		return (m_deltaTime = (float_t)m_elapsed.count());
	}

	const int32_t & GameTime::frameCount() const
	{
		return m_frame.count;
	}

	const float_t & GameTime::frameRate() const
	{
		return m_frame.rate;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}