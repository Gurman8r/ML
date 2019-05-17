#include <ML/Engine/Application.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Application::Application()
		: RenderWindow()
	{
		ML_EventSystem.addListener(EngineEvent::EV_Enter,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Load,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Start,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Update,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Draw,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Unload,	this);
		ML_EventSystem.addListener(EngineEvent::EV_Exit,	this);
	}

	Application::~Application() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void Application::onEvent(const ml::IEvent * value)
	{
		RenderWindow::onEvent(value);

		switch (*value)
		{
		case EngineEvent::EV_Enter:	return onEnter	(value->as<EnterEvent>());
		case EngineEvent::EV_Load:	return onLoad	(value->as<LoadEvent>());
		case EngineEvent::EV_Start:	return onStart	(value->as<StartEvent>());
		case EngineEvent::EV_Update:return onUpdate	(value->as<UpdateEvent>());
		case EngineEvent::EV_Draw:	return onDraw	(value->as<DrawEvent>());
		case EngineEvent::EV_Unload:return onUnload	(value->as<UnloadEvent>());
		case EngineEvent::EV_Exit:	return onExit	(value->as<ExitEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}