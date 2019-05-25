#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API GameTime final
		: public IObject
		, public INonCopyable
	{
	public:
		GameTime();
		~GameTime();

	public:
		GameTime & beginLoop();
		GameTime & endLoop();

	public:
		inline const Duration & elapsed()	const { return m_elapsed;	}
		inline const uint32_t	frameRate()	const { return m_frameRate; }
		inline const Duration & total()		const { return m_mainTimer.elapsed(); }

	private:
		Timer		m_mainTimer;
		Timer		m_loopTimer;
		Duration	m_elapsed;
		uint32_t	m_frameRate;
		uint32_t	m_frameCount;
		float		m_nextSecond;
		float		m_prevSecond;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_