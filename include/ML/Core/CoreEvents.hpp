#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class CoreEventType
	{
		MIN_CORE_EVENT = Event::EV_CORE,

		// ...

		MAX_CORE_EVENT
	};

	static_assert(
		(int32_t)CoreEventType::MAX_CORE_EVENT < 
		(int32_t)CoreEventType::MIN_CORE_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <CoreEventType ID> struct CoreEvent : public T_Event<CoreEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_