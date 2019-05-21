#ifndef _ML_I_EVENT_LISTENER_HPP_
#define _ML_I_EVENT_LISTENER_HPP_

#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API IEventListener
	{
	public:
		explicit IEventListener(EventSystem & eventSystem)
			: m_eventSystem(eventSystem)
		{
		}
		
		virtual ~IEventListener() {}

	public:
		virtual void onEvent(const IEvent * ev) = 0;

	public:
		inline EventSystem & eventSystem() const { return m_eventSystem; }

	private:
		EventSystem & m_eventSystem;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_LISTENER_HPP_