#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

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

		static_assert(MAX_ENGINE_EVENT < (MIN_ENGINE_EVENT + (int32_t)Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};


	// Startup
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public I_Event<EngineEvents::EV_Enter>
	{
		constexpr EnterEvent() {}
	};

	struct LoadEvent final : public I_Event<EngineEvents::EV_Load>
	{
		constexpr LoadEvent() {}
	};

	struct StartEvent final : public I_Event<EngineEvents::EV_Start>
	{
		constexpr StartEvent() {}
	};


	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginLoopEvent final : public I_Event<EngineEvents::EV_BeginLoop>
	{
		constexpr BeginLoopEvent() {}
	};

	struct UpdateEvent final : public I_Event<EngineEvents::EV_Update>
	{
		constexpr UpdateEvent() {}
	};

	struct BeginDrawEvent final : public I_Event<EngineEvents::EV_BeginDraw>
	{
		constexpr BeginDrawEvent() {}
	};

	struct DrawEvent final : public I_Event<EngineEvents::EV_Draw>
	{
		constexpr DrawEvent() {}
	};

	struct EndDrawEvent final : public I_Event<EngineEvents::EV_EndDraw>
	{
		constexpr EndDrawEvent() {}
	};

	struct EndLoopEvent final : public I_Event<EngineEvents::EV_EndLoop>
	{
		constexpr EndLoopEvent() {}
	};


	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UnloadEvent final : public I_Event<EngineEvents::EV_Unload>
	{
		constexpr UnloadEvent() {}
	};

	struct ExitEvent final : public I_Event<EngineEvents::EV_Exit>
	{
		constexpr ExitEvent() {}
	};


	// Utility
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CommandEvent final : public I_Event<EngineEvents::EV_Command>
	{
		C_String cmd;
		constexpr CommandEvent(C_String cmd) : cmd(cmd) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_