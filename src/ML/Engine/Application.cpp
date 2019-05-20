#include <ML/Engine/Application.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Application::Application(EventSystem & eventSystem)
		: m_eventSystem(eventSystem)
	{
		eventSystem.addListener(EngineEvent::EV_Enter,	this);
		eventSystem.addListener(EngineEvent::EV_Load,	this);
		eventSystem.addListener(EngineEvent::EV_Start,	this);
		eventSystem.addListener(EngineEvent::EV_Update,	this);
		eventSystem.addListener(EngineEvent::EV_Draw,	this);
		eventSystem.addListener(EngineEvent::EV_Unload,	this);
	}

	Application::~Application() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void Application::onEvent(const ml::IEvent * value)
	{
		switch (*value)
		{
		case EngineEvent::EV_Enter:	return onEnter	(*value->as<EnterEvent>());
		case EngineEvent::EV_Load:	return onLoad	(*value->as<LoadEvent>());
		case EngineEvent::EV_Start:	return onStart	(*value->as<StartEvent>());
		case EngineEvent::EV_Update:return onUpdate	(*value->as<UpdateEvent>());
		case EngineEvent::EV_Draw:	return onDraw	(*value->as<DrawEvent>());
		case EngineEvent::EV_Unload:return onUnload	(*value->as<UnloadEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	EventSystem & Application::eventSystem() const
	{
		return m_eventSystem;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}