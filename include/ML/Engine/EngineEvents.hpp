#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Event.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class GameTime;
	class Preferences;
	class RenderWindow;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EngineEvent final
	{
		enum : int32_t
		{
			MIN_ENGINE_EVENT = Event::EV_ENGINE,

			EV_Enter,
			EV_Load,
			EV_Start,
			EV_BeginFrame,
			EV_Update,
			EV_BeginDraw,
			EV_Draw,
			EV_EndDraw,
			EV_EndFrame,
			EV_Unload,
			EV_Exit,

			MAX_ENGINE_EVENT
		};
	};

	// Startup
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

	struct ML_ENGINE_API LoadEvent final : public IEvent<EngineEvent::EV_Load>
	{
		Preferences		& prefs;
		Resources		& resources;
		LoadEvent(Preferences & prefs, Resources & resources)
			: prefs		(prefs)
			, resources	(resources)
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

	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API BeginFrameEvent final : public IEvent<EngineEvent::EV_BeginFrame>
	{
		GameTime		& time;
		RenderWindow	& window;
		BeginFrameEvent(GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API BeginDrawEvent final : public IEvent<EngineEvent::EV_BeginDraw>
	{
		BeginDrawEvent() {}
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

	struct ML_ENGINE_API EndDrawEvent final : public IEvent<EngineEvent::EV_EndDraw>
	{
		EndDrawEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API EndFrameEvent final : public IEvent<EngineEvent::EV_EndFrame>
	{
		GameTime		& time;
		RenderWindow	& window;
		EndFrameEvent(GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public IEvent<EngineEvent::EV_Unload>
	{
		Resources		& resources;
		RenderWindow	& window;
		UnloadEvent(Resources & resources, RenderWindow & window)
			: resources(resources)
			, window(window)
		{
		}
	};

	struct ML_ENGINE_API ExitEvent final : public IEvent<EngineEvent::EV_Exit>
	{
		ExitEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_