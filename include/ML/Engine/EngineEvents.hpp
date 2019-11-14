#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct	GameTime;
	struct	Preferences;
	struct	RenderTarget;
	struct	RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class EngineEventType
	{
		MIN_ENGINE_EVENT = Event::EV_ENGINE,

		EV_Enter,		// 
		EV_Load,		// 
		EV_Start,		// 
		EV_BeginStep,	// 
		EV_Update,		// 
		EV_BeginDraw,	// 
		EV_Draw,		// 
		EV_EndDraw,		// 
		EV_EndStep,		// 
		EV_Unload,		// 
		EV_Exit,		// 
		EV_Command,		// 

		MAX_ENGINE_EVENT
	};

	static_assert(
		(int32_t)EngineEventType::MAX_ENGINE_EVENT < 
		(int32_t)EngineEventType::MIN_ENGINE_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <EngineEventType ID> struct EngineEvent : public T_Event<EngineEventType, ID> 
	{
	};


	// Startup
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public EngineEvent<EngineEventType::EV_Enter>
	{
		int32_t argc;
		const C_String * argv;
		constexpr EnterEvent(int32_t argc, const C_String * argv)
			: argc { argc }
			, argv { argv }
		{
		}
	};

	struct LoadEvent final : public EngineEvent<EngineEventType::EV_Load>
	{
		constexpr LoadEvent() {}
	};

	struct StartEvent final : public EngineEvent<EngineEventType::EV_Start>
	{
		constexpr StartEvent() {}
	};


	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginStepEvent final : public EngineEvent<EngineEventType::EV_BeginStep>
	{
		constexpr BeginStepEvent() {}
	};

	struct UpdateEvent final : public EngineEvent<EngineEventType::EV_Update>
	{
		constexpr UpdateEvent() {}
	};

	struct BeginDrawEvent final : public EngineEvent<EngineEventType::EV_BeginDraw>
	{
		constexpr BeginDrawEvent() {}
	};

	struct DrawEvent final : public EngineEvent<EngineEventType::EV_Draw>
	{
		constexpr DrawEvent() {}
	};

	struct EndDrawEvent final : public EngineEvent<EngineEventType::EV_EndDraw>
	{
		constexpr EndDrawEvent() {}
	};

	struct EndStepEvent final : public EngineEvent<EngineEventType::EV_EndStep>
	{
		constexpr EndStepEvent() {}
	};


	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UnloadEvent final : public EngineEvent<EngineEventType::EV_Unload>
	{
		constexpr UnloadEvent() {}
	};

	struct ExitEvent final : public EngineEvent<EngineEventType::EV_Exit>
	{
		constexpr ExitEvent() {}
	};


	// Utility
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CommandEvent final : public EngineEvent<EngineEventType::EV_Command>
	{
		C_String cmd;
		constexpr CommandEvent(C_String cmd) : cmd(cmd) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_