#include <ML/Engine/Plugin.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin(EventSystem & eventSystem)
		: I_EventListener { eventSystem }
	{
		eventSystem.addListener(StartEvent::ID, this);
		eventSystem.addListener(UpdateEvent::ID, this);
		eventSystem.addListener(DrawEvent::ID, this);
		eventSystem.addListener(ExitEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Plugin::onEvent(const Event & value)
	{
		switch (*value)
		{
		case StartEvent::ID:
			if (auto ev = value.as<StartEvent>()) return onStart(*ev);

		case UpdateEvent::ID:
			if (auto ev = value.as<UpdateEvent>()) return onUpdate(*ev);

		case DrawEvent::ID:
			if (auto ev = value.as<DrawEvent>()) return onDraw(*ev);

		case ExitEvent::ID:
			if (auto ev = value.as<ExitEvent>()) return onExit(*ev);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}