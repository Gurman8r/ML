#ifndef _ML_TIME_HPP_
#define _ML_TIME_HPP_

#include <ML/Core/Timer.hpp>

#define ML_Time ml::Time::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Time final
		: public ITrackable
		, public ISingleton<Time>
	{
		friend class ISingleton<Time>;

	private:
		Time() {}
		~Time() {}

	public:
		void start();
		
		const Timer & timer() const;

		const Duration & elapsed() const;

		float sin() const;

		float cos() const;

	private:
		Timer m_mainTimer;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIME_HPP_