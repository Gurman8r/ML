#ifndef _ML_I_EVENT_LISTENER_HPP_
#define _ML_I_EVENT_LISTENER_HPP_

#include <ML/Core/CoreEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API IEventListener
	{
	public:
		virtual void onEvent(const IEvent * ev) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_LISTENER_HPP_