#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime::GameTime() : m_main {}, m_loop {} { m_main.start(); }

	GameTime::~GameTime() { m_main.stop(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime & GameTime::beginLoop()
	{
		m_loop.start();
		return (*this);
	}

	GameTime & GameTime::endLoop()
	{
		m_elapsed = m_loop.stop().elapsed();

		m_tracker.accum += deltaTime() - m_tracker.capture[m_tracker.index];
		
		m_tracker.capture[m_tracker.index] = deltaTime();
		
		m_tracker.index = (m_tracker.index + 1) % m_tracker.size;
		
		m_tracker.frameRate = ((m_tracker.accum > 0.0f)
			? (1.0f / (m_tracker.accum / (float_t)m_tracker.size))
			: FLT_MAX
		);

		m_frame++;

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}