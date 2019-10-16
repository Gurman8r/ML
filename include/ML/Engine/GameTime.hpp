#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API GameTime final : public Newable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime();
		~GameTime();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GameTime & beginLoop();
		GameTime & endLoop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto elapsed()		const -> const Duration & { return m_elapsed; }
		inline auto total()			const -> const Duration & { return m_main.elapsed(); }
		inline auto deltaTime()		const -> const float_t & { return (m_delta = m_elapsed.delta()); }
		inline auto frameCount()	const -> const int32_t & { return m_frame; }
		inline auto frameRate()		const -> const float_t & { return m_tracker.frameRate; }
		inline auto totalTime()		const -> const float_t & { return (m_total = total().delta()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer m_main;
		Timer m_loop;
		
		mutable float_t		m_delta		{ 0 };
		mutable Duration	m_elapsed	{ 0 };
		mutable int32_t		m_frame		{ 0 };
		mutable float_t		m_total		{ 0 };

		struct FPS_Tracker final
		{
			static constexpr time_t	size { 512 };
			size_t	index;
			float_t frameRate;
			float_t accum;
			float_t capture[size];
			FPS_Tracker() = default;
		} m_tracker;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_