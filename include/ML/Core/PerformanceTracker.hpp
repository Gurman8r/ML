#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>
#include <ML/Core/Timer.hpp>

#define ML_PerformanceTracker ::ml::PerformanceTracker::getInstance()

#ifndef ML_DISABLE_BENCHMARKS
#	define ML_BENCH(...) ML_ANON_T(ScopeTimer, ##__VA_ARGS__)
#else
#	define ML_BENCH(...)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API PerformanceTracker final : public Singleton<PerformanceTracker>
	{
		friend Singleton<PerformanceTracker>;

		std::vector<std::pair<C_String, Duration>> m_curr, m_prev;
		
		PerformanceTracker() : m_curr{}, m_prev{}
		{
		}
		
		~PerformanceTracker() {}

	public:
		inline void endFrame()
		{
			m_prev = m_curr;

			m_curr.clear();
		}

		inline void push_trace(C_String name, Duration const & duration)
		{
			m_curr.push_back({ name, duration });
		}

		inline auto const & lastFrame() const
		{
			return m_prev;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API ScopeTimer final
	{
		C_String name; Timer timer;

		ScopeTimer(C_String name) : name{ name }, timer { true }
		{
		}

		~ScopeTimer()
		{
			ML_PerformanceTracker.push_trace(name, timer.stop().elapsed());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_