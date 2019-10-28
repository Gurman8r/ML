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

		GameTime & beginStep();
		GameTime & endStep();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		inline float_t totalTime() const
		{
			return static_cast<float_t>(Timer::master().elapsed().count());
		}

		inline float_t deltaTime() const
		{
			return static_cast<float_t>(m_elapsed.count());
		}

		inline uint64_t frameCount() const
		{
			return m_frame.count;
		}

		inline float_t frameRate() const
		{
			return m_frame.rate;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer m_step;

		mutable Duration m_elapsed { 0 };

		struct FrameTracker final
		{
			FrameTracker() = default;
			size_t		index		{ 0 };
			float_t		rate		{ 0 };
			float_t		accum		{ 0 };
			float_t		buf[90]		{ 0 };
			uint64_t	count		{ 0 };
		} m_frame;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_