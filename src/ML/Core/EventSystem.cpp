#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventSystem::EventSystem() {}

	EventSystem::~EventSystem() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EventSystem::addListener(const int32_t & type, IEventListener * listener)
	{
		m_listeners.insert({ type, listener });
	}
	
	void EventSystem::fireEvent(const IEvent & ev)
	{
		Pair<iterator, iterator> found = m_listeners.equal_range((int32_t)(ev));

		for (iterator it = found.first; it != found.second; ++it)
		{
			it->second->onEvent(&ev);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EventSystem::removeListener(const int32_t & type, IEventListener * listener)
	{
		Pair<iterator, iterator> found = m_listeners.equal_range(type);

		for (iterator it = found.first; it != found.second; ++it)
		{
			if (it->second == listener)
			{
				m_listeners.erase(it);

				break; // to prevent using invalidated iterator
			}
		}
	}
	
	void EventSystem::removeListenerFromAllEvents(IEventListener * listener)
	{
		bool allTheWayThrough = false;

		while (!allTheWayThrough)
		{
			allTheWayThrough = true;

			for (iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
			{
				if (it->second == listener)
				{
					m_listeners.erase(it);

					allTheWayThrough = false; // didn't make it the whole way through

					break; // to prevent using invalidated iterator
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}