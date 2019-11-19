#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <ML/Core/Duration.hpp>
#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Timer final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clock		= typename std::chrono::high_resolution_clock;
		using TimePoint = typename Clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer()
			: m_paused	{ false }
			, m_prev	{}
			, m_next	{}
			, m_elapsed	{ 0 }
		{
		}

		Timer(bool startMe) : Timer{}
		{
			if (startMe) { this->start(); }
		}

		Timer(Timer const & copy)
			: m_paused	{ copy.m_paused }
			, m_prev	{ copy.m_prev }
			, m_next	{ copy.m_next }
			, m_elapsed	{ copy.m_elapsed }
		{
		}

		~Timer() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool const & paused() const
		{
			return m_paused;
		}

		inline Duration const & elapsed() const
		{
			return (m_paused ? (m_elapsed) : (m_elapsed = (Clock::now() - m_prev)));
		}

		inline Timer & pause(bool value)
		{
			if (value && !m_paused)
			{
				m_paused = true;
				m_next = Clock::now();
				m_elapsed = (m_next - m_prev);
			}
			else if (!value && m_paused)
			{
				m_paused = false;
				m_prev = Clock::now();
			}
			return (*this);
		}

		inline Timer & reset()
		{
			return this->stop().start();
		}

		inline Timer & start()
		{
			m_prev = m_next = Clock::now();
			m_elapsed = 0.0;
			return this->pause(false);
		}

		inline Timer & stop()
		{
			m_next = Clock::now();
			m_elapsed = (m_next - m_prev);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_paused;
		TimePoint	m_prev;
		TimePoint	m_next;

		mutable Duration m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_