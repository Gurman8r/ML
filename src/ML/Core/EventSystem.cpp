#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventSystem::addListener(const int32_t type, EventListener * listener)
	{
		return ((listener)
			? m_listeners.insert({ type, listener })->second
			: nullptr
		);
	}
	
	bool EventSystem::fireEvent(Event const & value)
	{
		if (!value) { return false; }
		auto found{ m_listeners.equal_range(*value) };
		for (auto it = found.first; it != found.second; it++)
		{
			if (it->second) { it->second->onEvent(value); }
		}
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EventSystem::removeListener(int32_t const & type, EventListener * listener)
	{
		auto found { m_listeners.equal_range(type) };
		for (auto it = found.first; it != found.second; it++)
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
		bool done { false };
		while (!done)
		{
			done = true;
			for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
			{
				if (it->second == listener)
				{
					// break to prevent using invalidated iterator
					m_listeners.erase(it);
					done = false; 
					break;
				}
			}
		}
		return done;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}