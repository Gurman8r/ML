#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API GameTime final
		: public Newable
		, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime();
		~GameTime();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime & beginLoop();
		GameTime & endLoop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto elapsed()	const -> const Duration &	{ return m_elapsed; }
		inline auto total()		const -> const Duration &	{ return m_mainTimer.elapsed(); }
		inline auto deltaTime() const -> const float_t		{ return m_delta; }
		inline auto frameRate()	const -> const float_t		{ return m_fps.frameRate; }
		inline auto totalTime()	const -> const float_t		{ return total().delta(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer		m_mainTimer;
		Timer		m_loopTimer;
		float_t		m_delta;
		Duration	m_elapsed;

		struct FPS_Tracker final
		{
			static constexpr time_t	size { 512 };

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