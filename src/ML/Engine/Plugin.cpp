#include <ML/Engine/Plugin.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin(EventSystem & eventSystem)
		: IEventListener(eventSystem)
	{
		eventSystem.addListener(EngineEvent::EV_Enter,	this);
		eventSystem.addListener(EngineEvent::EV_Start,	this);
		eventSystem.addListener(EngineEvent::EV_Update,	this);
		eventSystem.addListener(EngineEvent::EV_Draw,	this);
		eventSystem.addListener(EngineEvent::EV_Exit,	this);
	}

	Plugin::~Plugin() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void Plugin::onEvent(const ml::Event * value)
	{
		switch (*value)
		{
		case EngineEvent::EV_Enter:	return onEnter	(*value->as<EnterEvent>());
		case EngineEvent::EV_Start:	return onStart	(*value->as<StartEvent>());
		case EngineEvent::EV_Update:return onUpdate	(*value->as<UpdateEvent>());
		case EngineEvent::EV_Draw:	return onDraw	(*value->as<DrawEvent>());
		case EngineEvent::EV_Exit:	return onExit	(*value->as<ExitEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}