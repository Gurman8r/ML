#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API CoreEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = IEvent::EV_CORE + 1,
			
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_