#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <ML/Core/Timer.hpp>

#define ML_TRACKER(var, name, clbk) PerformanceTracker var { name, clbk };

namespace ml
{
	struct PerformanceTracker final : public Newable, public NonCopyable
	{
		using Callback = typename std::function<void(const_ref_t<C_String>, const_ref_t<Duration>)>;

		Callback	callback;
		C_String	name;
		Timer		timer;

		explicit PerformanceTracker(C_String name, Callback && callback)
			: name { name }
			, timer { true }
			, callback { callback }
		{
		}

		~PerformanceTracker()
		{
			callback(name, timer.stop().elapsed());
		}
	};
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_