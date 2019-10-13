#include <ML/Core/EventListener.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener::~EventListener()
	{
		ML_EventSystem.removeListenerFromAllEvents(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}