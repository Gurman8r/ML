#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <ML/Core/IEventListener.hpp>
#include <ML/Core/ISingleton.hpp>

#define ML_EventSystem ml::EventSystem::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API EventSystem final
		: public ITrackable
		, public ISingleton<EventSystem>
	{
		friend class ISingleton<EventSystem>;

	public:
		using ListenerMap	= MultiMap<int32_t, IEventListener *>;
		using iterator		= typename ListenerMap::iterator;

	public:
		void addListener(const int32_t & type, IEventListener * listener);
		void fireEvent(const IEvent & ev);
		void removeListener(const int32_t & type, IEventListener * listener);
		void removeListenerFromAllEvents(IEventListener * listener);

	private:
		EventSystem() {}
		~EventSystem() {}

		ListenerMap m_listeners;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_