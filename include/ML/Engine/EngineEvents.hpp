#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IEvent.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
			EV_Unload,
			EV_Exit,

			MAX_ENGINE_EVENT
		};

		EngineEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API EnterEvent final : public EngineEvent
	{
		int32_t argc;
		char ** argv;

		EnterEvent(int32_t argc, char ** argv)
			: EngineEvent(EV_Enter)
			, argc(argc)
			, argv(argv)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API LoadEvent final : public EngineEvent
	{
		LoadEvent()
			: EngineEvent(EV_Load)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API StartEvent final : public EngineEvent
	{
		StartEvent()
			: EngineEvent(EV_Start)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UpdateEvent final : public EngineEvent
	{
		const Duration & elapsed;

		UpdateEvent(const Duration & elapsed)
			: EngineEvent(EV_Update)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API DrawEvent final : public EngineEvent
	{
		const Duration & elapsed;

		DrawEvent(const Duration & elapsed)
			: EngineEvent(EV_Draw)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public EngineEvent
	{
		UnloadEvent()
			: EngineEvent(EV_Unload)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ExitEvent final : public EngineEvent
	{
		ExitEvent()
			: EngineEvent(EV_Exit)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_