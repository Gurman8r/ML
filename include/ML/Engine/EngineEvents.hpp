#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Event.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Engine;
	class GameTime;
	class Preferences;
	class RenderWindow;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EngineEvent final
	{
		enum : int32_t
		{
			MIN_ENGINE_EVENT = Event::EV_ENGINE + 1,

			EV_Enter,
			EV_LoadContent,
			EV_Start,
			EV_Update,
			EV_Draw,
			EV_Exit,

			EV_Shutdown,

			MAX_ENGINE_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API EnterEvent final : public IEvent<EngineEvent::EV_Enter>
	{
		Preferences		& prefs;
		RenderWindow	& window;
		EnterEvent(Preferences & prefs, RenderWindow & window)
			: prefs		(prefs)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API LoadContentEvent final : public IEvent<EngineEvent::EV_LoadContent>
	{
		Resources & resources;
		LoadContentEvent(Resources & resources)
			: resources	(resources)
		{
		}
	};

	struct ML_ENGINE_API StartEvent final : public IEvent<EngineEvent::EV_Start>
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		StartEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API UpdateEvent final : public IEvent<EngineEvent::EV_Update>
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		UpdateEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API DrawEvent final : public IEvent<EngineEvent::EV_Draw>
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		DrawEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API ExitEvent final : public IEvent<EngineEvent::EV_Exit>
	{
		Resources		& resources;
		RenderWindow	& window;
		ExitEvent(Resources	& resources, RenderWindow & window)
			: resources	(resources)
			, window	(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ShutdownEvent final : public IEvent<EngineEvent::EV_Shutdown>
	{
		ShutdownEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_