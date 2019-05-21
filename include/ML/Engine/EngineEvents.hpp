#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IEvent.hpp>
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

	struct ML_ENGINE_API EngineEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_ENGINE_EVENT = IEvent::EV_ENGINE + 1,

			EV_Enter,
			EV_Load,
			EV_Start,
			EV_Update,
			EV_Draw,
			EV_Exit,

			EV_Shutdown,

			MAX_ENGINE_EVENT
		};

		EngineEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API EnterEvent final : public EngineEvent
	{
		Preferences		& prefs;
		Resources		& resources;
		RenderWindow	& window;
		EnterEvent(Preferences & prefs, Resources & resources, RenderWindow & window)
			: EngineEvent(EV_Enter)
			, prefs		(prefs)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API LoadEvent final : public EngineEvent
	{
		Preferences		& prefs;
		Resources		& resources;
		RenderWindow	& window;
		LoadEvent(Preferences & prefs, Resources & resources, RenderWindow & window)
			: EngineEvent(EV_Load)
			, prefs		(prefs)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API StartEvent final : public EngineEvent
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		StartEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: EngineEvent(EV_Start)
			, time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API UpdateEvent final : public EngineEvent
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		UpdateEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: EngineEvent(EV_Update)
			, time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API DrawEvent final : public EngineEvent
	{
		const GameTime	& time;
		Resources		& resources;
		RenderWindow	& window;
		DrawEvent(const GameTime & time, Resources & resources, RenderWindow & window)
			: EngineEvent(EV_Draw)
			, time		(time)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	struct ML_ENGINE_API ExitEvent final : public EngineEvent
	{
		Resources		& resources;
		RenderWindow	& window;
		ExitEvent(Resources	& resources, RenderWindow & window)
			: EngineEvent(EV_Exit)
			, resources	(resources)
			, window	(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ShutdownEvent final : public EngineEvent
	{
		ShutdownEvent() : EngineEvent(EV_Shutdown) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_