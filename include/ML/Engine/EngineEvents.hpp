#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Engine;

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
			EV_Unload,

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
		int32_t argc;
		char ** argv;
		Engine & engine;

		EnterEvent(int32_t argc, char ** argv, Engine & engine)
			: EngineEvent(EV_Enter)
			, argc	(argc)
			, argv	(argv)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API LoadEvent final : public EngineEvent
	{
		Engine & engine;

		LoadEvent(Engine & engine)
			: EngineEvent(EV_Load)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API StartEvent final : public EngineEvent
	{
		Engine & engine;

		StartEvent(Engine & engine)
			: EngineEvent(EV_Start)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UpdateEvent final : public EngineEvent
	{
		Engine & engine;

		UpdateEvent(Engine & engine)
			: EngineEvent(EV_Update)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API DrawEvent final : public EngineEvent
	{
		Engine & engine;

		DrawEvent(Engine & engine)
			: EngineEvent(EV_Draw)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public EngineEvent
	{
		Engine & engine;

		UnloadEvent(Engine & engine)
			: EngineEvent(EV_Unload)
			, engine(engine)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ShutdownEvent final : public EngineEvent
	{
		ShutdownEvent()
			: EngineEvent(EV_Shutdown)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_