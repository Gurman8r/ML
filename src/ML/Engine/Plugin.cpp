#include <ML/Engine/Plugin.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin(EventSystem & eventSystem)
		: I_EventListener { eventSystem }
	{
	}

	Plugin::~Plugin() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}