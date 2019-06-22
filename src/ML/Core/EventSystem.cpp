#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventSystem::EventSystem() {}

	EventSystem::~EventSystem() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventSystem::addListener(const int32_t type, EventListener * listener)
	{
		return (listener
			? m_listeners.insert({ type, listener })->second
			: nullptr
		);
	}
	
	bool EventSystem::fireEvent(const Event & ev)
	{
		if (ev > Event::EV_INVALID)
		{
			Pair<iterator, iterator> found = m_listeners.equal_range((int32_t)(ev));

			for (iterator it = found.first; it != found.second; ++it)
			{
				it->second->onEvent(&ev);
			}

			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EventSystem::removeListener(const int32_t & type, EventListener * listener)
	{
		Pair<iterator, iterator> found = m_listeners.equal_range(type);

		for (iterator it = found.first; it != found.second; ++it)
		{
			if (it->second == listener)
			{
				m_listeners.erase(it);

				return true;
			}
		}
		return false;
	}
	
	bool EventSystem::removeListenerFromAllEvents(EventListener * listener)
	{
		bool allTheWayThrough = false;

		while (!allTheWayThrough)
		{
			allTheWayThrough = true;

			for (iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
			{
				if (it->second == listener)
				{
					// didn't make it the whole way through.
					// break to prevent using invalidated iterator.
					m_listeners.erase(it);
					allTheWayThrough = false; 
					break;
				}
			}
		}
		return allTheWayThrough;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}