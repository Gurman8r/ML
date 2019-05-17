#include <ML/Core/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void Time::start()
	{
		m_mainTimer.start();
	}

	const Timer & Time::timer() const
	{
		return m_mainTimer;
	}

	const Duration & Time::elapsed() const
	{
		return timer().elapsed();
	}

	float Time::sin() const
	{
		return std::sinf(elapsed().delta());
	}

	float Time::cos() const
	{
		return std::cosf(elapsed().delta());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}