#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Timer::Timer()
		: m_paused	(true)
		, m_clock	()
		, m_prev	()
		, m_next	()
		, m_elapsed	()
	{
	}

	Timer::Timer(const Timer & copy)
		: m_paused	(copy.m_paused)
		, m_clock	(copy.m_clock)
		, m_prev	(copy.m_prev)
		, m_next	(copy.m_next)
		, m_elapsed	(copy.m_elapsed)
	{
	}

	Timer::~Timer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Timer & Timer::pause(bool paused)
	{
		if (paused && !m_paused)
		{
			m_paused = true;
			m_next = m_clock.now();
			m_elapsed = (m_next - m_prev);
		}
		else if (!paused && m_paused)
		{
			m_paused = false;
			m_prev = m_clock.now();
		}
		return (*this);
	}

	Timer & Timer::reset()
	{
		return stop().start();
	}

	Timer & Timer::start()
	{
		m_prev = m_next = m_clock.now();
		m_elapsed = Duration();
		return pause(false);
	}

	Timer & Timer::stop()
	{
		m_next = m_clock.now();
		m_elapsed = (m_next - m_prev);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Duration & Timer::elapsed() const
	{
		return ((m_paused)
			? (m_elapsed)
			: (m_elapsed = (m_clock.now() - m_prev))
		);
	}

	const bool & Timer::paused() const
	{
		return m_paused;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}
