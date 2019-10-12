#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Core/I_Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct	GameTime;
	struct	Preferences;
	struct	RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EngineEvents final
	{
		EngineEvents() = delete;

		enum : int32_t
		{
			MIN_ENGINE_EVENT = Event::EV_ENGINE,

			EV_Enter,		// 
			EV_Load,		// 
			EV_Start,		// 
			EV_BeginLoop,	// 
			EV_Update,		// 
			EV_BeginDraw,	// 
			EV_Draw,		// 
			EV_EndDraw,		// 
			EV_EndLoop,		// 
			EV_Unload,		// 
			EV_Exit,		// 
			EV_Command,		// 

			MAX_ENGINE_EVENT
		};

		static_assert(MAX_ENGINE_EVENT < (MIN_ENGINE_EVENT + Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	// Startup
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public I_Event<EngineEvents::EV_Enter>
	{
		const GameTime & time;
		Preferences & prefs;
		RenderWindow & window;
		constexpr EnterEvent(const GameTime & time, Preferences & prefs, RenderWindow & window)
			: time(time)
			, prefs(prefs)
			, window(window)
		{
		}
	};

	struct LoadEvent final : public I_Event<EngineEvents::EV_Load>
	{
		const GameTime & time;
		Preferences & prefs;
		RenderWindow & window;
		constexpr LoadEvent(const GameTime & time, Preferences & prefs, RenderWindow & window)
			: time(time)
			, prefs(prefs)
			, window(window)
		{
		}
	};

	struct StartEvent final : public I_Event<EngineEvents::EV_Start>
	{
		const GameTime & time;
		Preferences & prefs;
		RenderWindow & window;
		constexpr StartEvent(const GameTime & time, Preferences & prefs, RenderWindow & window)
			: time(time)
			, prefs(prefs)
			, window(window)
		{
		}
	};

	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginLoopEvent final : public I_Event<EngineEvents::EV_BeginLoop>
	{
		GameTime & time;
		RenderWindow & window;
		constexpr BeginLoopEvent(GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UpdateEvent final : public I_Event<EngineEvents::EV_Update>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr UpdateEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginDrawEvent final : public I_Event<EngineEvents::EV_BeginDraw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr BeginDrawEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	struct DrawEvent final : public I_Event<EngineEvents::EV_Draw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr DrawEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	struct EndDrawEvent final : public I_Event<EngineEvents::EV_EndDraw>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr EndDrawEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EndLoopEvent final : public I_Event<EngineEvents::EV_EndLoop>
	{
		GameTime & time;
		RenderWindow & window;
		constexpr EndLoopEvent(GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UnloadEvent final : public I_Event<EngineEvents::EV_Unload>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr UnloadEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};

	struct ExitEvent final : public I_Event<EngineEvents::EV_Exit>
	{
		const GameTime & time;
		RenderWindow & window;
		constexpr ExitEvent(const GameTime & time, RenderWindow & window)
			: time(time)
			, window(window)
		{
		}
	};


	// Utility
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CommandEvent final : public I_Event<EngineEvents::EV_Command>
	{
		C_String cmd;
		constexpr CommandEvent(C_String cmd)
			: cmd(cmd)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_