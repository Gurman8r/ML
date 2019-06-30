#ifndef _ML_EVENT_LISTENER_HPP_
#define _ML_EVENT_LISTENER_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;
	struct EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API EventListener
	{
	public:
		explicit EventListener(EventSystem & eventSystem);
		virtual ~EventListener();

	public:
		virtual void onEvent(const Event * value) = 0;

	public:
		EventSystem & eventSystem() const;

	private:
		EventSystem & m_eventSystem;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_LISTENER_HPP_