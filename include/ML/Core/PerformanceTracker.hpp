#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>
#include <ML/Core/Timer.hpp>

#define ML_PerformanceTracker ::ml::PerformanceTracker::getInstance()

namespace ml
{
	struct ML_CORE_API PerformanceTracker final : public Singleton<PerformanceTracker>
	{
		friend Singleton<PerformanceTracker>;

		inline void clear_traces()
		{
			m_traces.clear();
		}

		inline void push_trace(C_String name, Duration const & duration)
		{
			m_traces.push_back({ name, duration });
		}

		inline auto const & traces() const { return m_traces; }

	private:
		std::vector<std::pair<C_String, Duration>> m_traces;
		PerformanceTracker() : m_traces{} {}
		~PerformanceTracker() {}
	};

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
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_