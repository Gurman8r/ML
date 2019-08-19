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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime();
		~GameTime();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime & beginLoop();
		GameTime & endLoop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto deltaTime() const -> float_t			{ return m_delta; }
		inline auto elapsed()	const -> const Duration &	{ return m_elapsed; }
		inline auto frameRate()	const -> float_t			{ return m_fps.frameRate; }
		inline auto total()		const -> const Duration &	{ return m_mainTimer.elapsed(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer		m_mainTimer;
		Timer		m_loopTimer;
		float_t		m_delta;
		Duration	m_elapsed;

		struct FPS_Tracker final
		{
			static constexpr time_t		size	{ (2_min).seconds() };
			static constexpr float_t	scale	{ (float_t)size };

			size_t	index;
			float_t frameRate;
			float_t accum;
			float_t capture[size];
		} m_fps;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_