#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Event.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class GameTime;
	class Prefs;
	class RenderWindow;

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
		const Prefs		& prefs;
		RenderWindow	& window;
		constexpr EnterEvent(Prefs & prefs, RenderWindow & window)
			: prefs	(prefs)
			, window(window)
		{
		}
	};

	struct ML_ENGINE_API LoadEvent final : public IEvent<EngineEvent::EV_Load>
	{
		const Prefs		& prefs;
		constexpr LoadEvent(Prefs & prefs)
			: prefs	(prefs)
		{
		}
	};

	struct ML_ENGINE_API StartEvent final : public IEvent<EngineEvent::EV_Start>
	{
		const GameTime	& time;
		RenderWindow	& window;
		constexpr StartEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API BeginFrameEvent final : public IEvent<EngineEvent::EV_BeginFrame>
	{
		GameTime		& time;
		RenderWindow	& window;
		constexpr BeginFrameEvent(GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UpdateEvent final : public IEvent<EngineEvent::EV_Update>
	{
		const GameTime	& time;
		RenderWindow	& window;
		constexpr UpdateEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API BeginDrawEvent final : public IEvent<EngineEvent::EV_BeginDraw>
	{
		constexpr BeginDrawEvent() {}
	};

	struct ML_ENGINE_API DrawEvent final : public IEvent<EngineEvent::EV_Draw>
	{
		const GameTime	& time;
		RenderWindow	& window;
		constexpr DrawEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	struct ML_ENGINE_API EndDrawEvent final : public IEvent<EngineEvent::EV_EndDraw>
	{
		constexpr EndDrawEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API EndFrameEvent final : public IEvent<EngineEvent::EV_EndFrame>
	{
		GameTime		& time;
		RenderWindow	& window;
		constexpr EndFrameEvent(GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public IEvent<EngineEvent::EV_Unload>
	{
		RenderWindow	& window;
		constexpr UnloadEvent(RenderWindow & window)
			: window(window)
		{
		}
	};

	struct ML_ENGINE_API ExitEvent final : public IEvent<EngineEvent::EV_Exit>
	{
		constexpr ExitEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_