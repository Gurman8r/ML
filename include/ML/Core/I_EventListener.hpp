#ifndef _ML_I_EVENT_LISTENER_HPP_
#define _ML_I_EVENT_LISTENER_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;
	struct EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API I_EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit I_EventListener(EventSystem & eventSystem)
			: m_eventSystem { eventSystem }
		{
		}
		
		virtual ~I_EventListener() {}

		virtual void onEvent(const Event & value) = 0;

		inline auto eventSystem() const -> EventSystem & { return m_eventSystem; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: EventSystem & m_eventSystem;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_LISTENER_HPP_