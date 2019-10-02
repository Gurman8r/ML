#ifndef _ML_I_EVENT_LISTENER_HPP_
#define _ML_I_EVENT_LISTENER_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API I_EventListener
	{
		virtual ~I_EventListener() {}

		virtual void onEvent(const Event & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_LISTENER_HPP_