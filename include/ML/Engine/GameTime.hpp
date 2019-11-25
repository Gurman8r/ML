#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>
#include <ML/Core/Array.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API GameTime final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline float_t totalTime() const { return static_cast<float_t>(m_main.elapsed().count()); }

		inline float_t deltaTime() const { return static_cast<float_t>(m_elapsed.count()); }

		inline uint64_t frameCount() const { return m_frame.count; }

		inline float_t frameRate() const { return m_frame.rate; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline GameTime & beginStep()
		{
			m_step.start();
			return (*this);
		}

		inline GameTime & endStep()
		{
			m_elapsed = m_step.stop().elapsed();
			m_frame.accum += deltaTime() - m_frame.buf[m_frame.index];
			m_frame.buf[m_frame.index] = deltaTime();
			m_frame.index = (m_frame.index + 1) % m_frame.buf.size();
			m_frame.rate = ((m_frame.accum > 0.0f)
				? (1.0f / (m_frame.accum / (float_t)m_frame.buf.size()))
				: limits<float_t>::min
			);
			m_frame.count++;
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class Engine;

		GameTime() : m_step{}, m_main{ true }{}

		~GameTime() {}

		Timer m_main;
		Timer m_step;

		mutable Duration m_elapsed { 0 };

		struct FrameTracker final
		{
			using Samples = Array<float_t, 90>;

			size_t		index	{ 0 };
			float_t		rate	{ 0 };
			float_t		accum	{ 0 };
			uint64_t	count	{ 0 };
			Samples		buf		{ 0 };

			FrameTracker() = default;
		} m_frame;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_TIME_HPP_