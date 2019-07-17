#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct GameTime;
	struct Preferences;
	struct RenderWindow;

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

	struct EnterEvent final : public I_Event<EngineEvent::EV_Enter>
	{
		const GameTime & time;
		const Preferences & prefs;
		RenderWindow & window;
		constexpr EnterEvent(const GameTime & time, Preferences & prefs, RenderWindow & window)
			: time	(time)
			, prefs	(prefs)
			, window(window)
		{
		}
	};

	struct LoadEvent final : public I_Event<EngineEvent::EV_Load>
	{
		const GameTime & time;
		const Preferences & prefs;
		constexpr LoadEvent(const GameTime & time, Preferences & prefs)
			: time	(time)
			, prefs	(prefs)
		{
		}
	};

	struct StartEvent final : public I_Event<EngineEvent::EV_Start>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr StartEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginFrameEvent final : public I_Event<EngineEvent::EV_BeginFrame>
	{
		GameTime & time;
		RenderWindow & window;
		constexpr BeginFrameEvent(GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UpdateEvent final : public I_Event<EngineEvent::EV_Update>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr UpdateEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginDrawEvent final : public I_Event<EngineEvent::EV_BeginDraw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr BeginDrawEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	struct DrawEvent final : public I_Event<EngineEvent::EV_Draw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr DrawEvent(const GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	struct EndDrawEvent final : public I_Event<EngineEvent::EV_EndDraw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr EndDrawEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EndFrameEvent final : public I_Event<EngineEvent::EV_EndFrame>
	{
		GameTime & time;
		RenderWindow & window;
		constexpr EndFrameEvent(GameTime & time, RenderWindow & window)
			: time	(time)
			, window(window)
		{
		}
	};

	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UnloadEvent final : public I_Event<EngineEvent::EV_Unload>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr UnloadEvent(const GameTime & time, RenderWindow & window)
			: time (time)
			, window(window)
		{
		}
	};

	struct ExitEvent final : public I_Event<EngineEvent::EV_Exit>
	{
		const GameTime & time;
		constexpr ExitEvent(const GameTime & time) 
			: time (time)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_