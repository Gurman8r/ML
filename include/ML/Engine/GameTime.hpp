#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API GameTime final
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime();
		~GameTime();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime & beginLoop();
		GameTime & endLoop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto elapsed()	const -> const Duration &	{ return m_elapsed; }
		inline auto frameRate()	const -> const uint32_t		{ return m_frameRate; }
		inline auto total()		const -> const Duration &	{ return m_mainTimer.elapsed(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer		m_mainTimer;
		Timer		m_loopTimer;
		Duration	m_elapsed;
		uint32_t	m_frameRate;
		uint32_t	m_frameCount;
		float_t		m_nextSecond;
		float_t		m_prevSecond;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_