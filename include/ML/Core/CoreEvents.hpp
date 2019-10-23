#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class CoreEvents
	{
		MIN_CORE_EVENT = Event::EV_CORE,

		// 

		MAX_CORE_EVENT
	};

	static_assert(
		(int32_t)CoreEvents::MAX_CORE_EVENT < 
		(int32_t)CoreEvents::MIN_CORE_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <CoreEvents ID> struct CoreEvent : public T_Event<CoreEvents, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_