#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime::GameTime() : m_main {}, m_step {} { m_main.start(); }

	GameTime::~GameTime() { m_main.stop(); }

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
			: FLT_MAX
		);

		m_frame.count++;

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}