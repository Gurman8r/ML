#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <ML/Core/Duration.hpp>
#include <ML/Core/Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Timer final : public Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clock		= typename std::chrono::high_resolution_clock;
		using TimePoint = typename Clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer()
			: m_paused	{ true }
			, m_clock	{}
			, m_prev	{}
			, m_next	{}
			, m_elapsed	{}
		{
		}

		Timer(bool start)
			: m_paused	{ !start }
			, m_clock	{}
			, m_prev	{}
			, m_next	{}
			, m_elapsed	{}
		{
		}

		Timer(const Timer & copy)
			: m_paused	{ copy.m_paused }
			, m_clock	{ copy.m_clock }
			, m_prev	{ copy.m_prev }
			, m_next	{ copy.m_next }
			, m_elapsed	{ copy.m_elapsed }
		{
		}

		~Timer() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const bool & paused() const
		{
			return m_paused;
		}

		inline const Duration & elapsed() const
		{
			return (m_paused ? (m_elapsed) : (m_elapsed = (m_clock.now() - m_prev)));
		}

		inline Timer & pause(bool value)
		{
			if (value && !m_paused)
			{
				m_paused = true;
				m_next = m_clock.now();
				m_elapsed = (m_next - m_prev);
			}
			else if (!value && m_paused)
			{
				m_paused = false;
				m_prev = m_clock.now();
			}
			return (*this);
		}

		inline Timer & reset()
		{
			return stop().start();
		}

		inline Timer & start()
		{
			m_prev = m_next = m_clock.now();
			m_elapsed = Duration {};
			return pause(false);
		}

		inline Timer & stop()
		{
			m_next = m_clock.now();
			m_elapsed = (m_next - m_prev);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_paused;
		Clock		m_clock;
		TimePoint	m_prev;
		TimePoint	m_next;

		mutable Duration m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_