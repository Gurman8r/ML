#include <ML/Engine/Plugin.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin(EventSystem & eventSystem)
		: EventListener(eventSystem)
	{
		eventSystem.addListener(EnterEvent::ID, this);
		eventSystem.addListener(StartEvent::ID, this);
		eventSystem.addListener(UpdateEvent::ID, this);
		eventSystem.addListener(DrawEvent::ID, this);
		eventSystem.addListener(ExitEvent::ID, this);
	}

	Plugin::~Plugin() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void Plugin::onEvent(const Event * value)
	{
		switch (*value)
		{
		case EnterEvent::ID:	return onEnter	(*value->as<EnterEvent>());
		case StartEvent::ID:	return onStart	(*value->as<StartEvent>());
		case UpdateEvent::ID:	return onUpdate	(*value->as<UpdateEvent>());
		case DrawEvent::ID:		return onDraw	(*value->as<DrawEvent>());
		case ExitEvent::ID:		return onExit	(*value->as<ExitEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}