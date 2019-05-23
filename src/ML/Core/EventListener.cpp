#include <ML/Core/EventListener.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EventListener::EventListener(EventSystem & eventSystem)
		: m_eventSystem(eventSystem)
	{
	}

	EventListener::~EventListener()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	EventSystem & EventListener::eventSystem() const
	{
		return m_eventSystem;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}