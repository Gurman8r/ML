#ifndef _ML_EVENT_LISTENER_HPP_
#define _ML_EVENT_LISTENER_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API EventListener
	{
		virtual ~EventListener();

		virtual void onEvent(Event const & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_LISTENER_HPP_