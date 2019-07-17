#ifndef _ML_EVENT_LISTENER_HPP_
#define _ML_EVENT_LISTENER_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;
	struct EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit EventListener(EventSystem & eventSystem);
		
		virtual ~EventListener();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onEvent(const Event * value) = 0;

		EventSystem & eventSystem() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: EventSystem & m_eventSystem;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_LISTENER_HPP_