#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IEvent.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Engine;
	class Preferences;
	class Resources;

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
		int32_t				argc;
		char **				argv;
		const Preferences & prefs;
		Engine &			engine;
		Resources &			resources;

		EnterEvent(int32_t argc, char ** argv, const Preferences & prefs, Engine & engine, Resources & resources)
			: EngineEvent(EV_Enter)
			, argc(argc)
			, argv(argv)
			, prefs(prefs)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API LoadEvent final : public EngineEvent
	{
		const Preferences & prefs;
		Engine &			engine;
		Resources &			resources;

		LoadEvent(const Preferences & prefs, Engine & engine, Resources & resources)
			: EngineEvent(EV_Load)
			, prefs(prefs)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API StartEvent final : public EngineEvent
	{
		const Preferences & prefs;
		Engine &			engine;
		Resources &			resources;

		StartEvent(const Preferences & prefs, Engine & engine, Resources & resources)
			: EngineEvent(EV_Start)
			, prefs(prefs)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UpdateEvent final : public EngineEvent
	{
		const Duration	& elapsed;
		Engine			& engine;
		Resources		& resources;

		UpdateEvent(const Duration & elapsed, Engine & engine, Resources & resources)
			: EngineEvent(EV_Update)
			, elapsed(elapsed)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API DrawEvent final : public EngineEvent
	{
		const Duration	& elapsed;
		Engine			& engine;
		Resources		& resources;

		DrawEvent(const Duration & elapsed, Engine & engine, Resources & resources)
			: EngineEvent(EV_Draw)
			, elapsed(elapsed)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public EngineEvent
	{
		const Duration	& elapsed;
		Engine			& engine;
		Resources		& resources;

		UnloadEvent(Engine & engine, Resources & resources)
			: EngineEvent(EV_Unload)
			, elapsed(elapsed)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ExitEvent final : public EngineEvent
	{
		Engine		& engine;
		Resources	& resources;

		ExitEvent(Engine & engine, Resources & resources)
			: EngineEvent(EV_Exit)
			, engine(engine)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_