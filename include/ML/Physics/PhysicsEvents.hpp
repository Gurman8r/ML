#ifndef _ML_PHYSICS_EVENTS_HPP_
#define _ML_PHYSICS_EVENTS_HPP_

#include <ML/Core/IEvent.hpp>
#include <ML/Physics/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PHYSICS_API PhysicsEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_PHYSICS_EVENT = IEvent::EV_PHYSICS + 1,

			MAX_PHYSICS_EVENT
		};

		PhysicsEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_EVENTS_HPP_