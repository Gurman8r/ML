#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API GameTime final
		: public ITrackable
		, public INonCopyable
	{
		Timer		m_mainTimer;
		Timer		m_loopTimer;
		Duration	m_frameTime;
		uint32_t	m_frameRate;
		uint32_t	m_frameCount;
		float		m_nextSecond; 
		float		m_prevSecond; 

	public:
		GameTime();
		~GameTime();

	public:
		GameTime & beginLoop();
		GameTime & endLoop();

	public:
		inline const uint32_t	frameRate()	const { return m_frameRate; }
		inline const Duration & frameTime()	const { return m_frameTime;	}
		inline const Duration & totalTime()	const { return m_mainTimer.elapsed(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_