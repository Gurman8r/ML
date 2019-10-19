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

		inline const Timer & main() const
		{
			return m_main;
		}

		inline const Timer & step() const
		{
			return m_step;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const float_t & totalTime() const
		{ 
			return (m_totalTime = m_main.elapsed().delta());
		}

		inline const float_t & deltaTime()	const
		{ 
			return (m_deltaTime = m_elapsed.delta());
		}

		inline const int32_t & frameCount() const
		{ 
			return m_frame.count; 
		}

		inline const float_t & frameRate() const
		{ 
			return m_frame.rate; 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Timer m_main; // Master Timer
		Timer m_step; // Frame Timer

		mutable Duration	m_elapsed	{ 0 };
		mutable float_t		m_deltaTime	{ 0 };
		mutable float_t		m_totalTime	{ 0 };

		struct FrameTracker final
		{
			FrameTracker() = default;
			size_t	index		{ 0 };
			float_t rate		{ 0 };
			float_t accum		{ 0 };
			float_t buf[512]	{ 0 };
			int32_t count		{ 0 };
		} m_frame;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_