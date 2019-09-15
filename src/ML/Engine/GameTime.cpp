#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime::GameTime()
		: m_mainTimer	{}
		, m_loopTimer	{}
		, m_delta		{ NULL }
		, m_elapsed		{ NULL }
		, m_fps			{ NULL }
	{
		m_mainTimer.start();
	}

	GameTime::~GameTime()
	{
		m_mainTimer.stop();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GameTime & GameTime::beginLoop()
	{
		m_loopTimer.start();
		return (*this);
	}

	GameTime & GameTime::endLoop()
	{
		m_delta = (m_elapsed = m_loopTimer.stop().elapsed()).delta();

		m_fps.accum += m_delta - m_fps.capture[m_fps.index];
		
		m_fps.capture[m_fps.index] = m_delta;
		
		m_fps.index = (m_fps.index + 1) % m_fps.size;
		
		m_fps.frameRate = ((m_fps.accum > 0.0f)
			? (1.0f / (m_fps.accum / (float_t)m_fps.size))
			: FLT_MAX
		);

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}