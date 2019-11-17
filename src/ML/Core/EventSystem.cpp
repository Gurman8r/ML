#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ptr_t<EventListener> EventSystem::addListener(const int32_t type, ptr_t<EventListener> listener)
	{
		return ((listener)
			? m_listeners.insert({ type, listener })->second
			: nullptr
		);
	}
	
	bool EventSystem::fireEvent(const Event & value)
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

	bool EventSystem::removeListener(const int32_t & type, ptr_t<EventListener> listener)
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
	
	bool EventSystem::removeListenerFromAllEvents(ptr_t<EventListener> listener)
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