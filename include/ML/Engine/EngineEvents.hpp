#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IEvent.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
		int32_t argc;
		char ** argv;
		const Preferences & prefs;
		Resources & res;

		EnterEvent(int32_t argc, char ** argv, const Preferences & prefs, Resources & res)
			: EngineEvent(EV_Enter)
			, argc(argc)
			, argv(argv)
			, prefs(prefs)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API LoadEvent final : public EngineEvent
	{
		const Preferences & prefs;
		Resources & res;

		LoadEvent(const Preferences & prefs, Resources & res)
			: EngineEvent(EV_Load)
			, prefs(prefs)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API StartEvent final : public EngineEvent
	{
		const Preferences & prefs;
		Resources & res;

		StartEvent(const Preferences & prefs, Resources & res)
			: EngineEvent(EV_Start)
			, prefs(prefs)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UpdateEvent final : public EngineEvent
	{
		const Duration & elapsed;
		Resources & res;

		UpdateEvent(const Duration & elapsed, Resources & res)
			: EngineEvent(EV_Update)
			, elapsed(elapsed)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API DrawEvent final : public EngineEvent
	{
		const Duration & elapsed;
		Resources & res;

		DrawEvent(const Duration & elapsed, Resources & res)
			: EngineEvent(EV_Draw)
			, elapsed(elapsed)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API UnloadEvent final : public EngineEvent
	{
		Resources & res;
		UnloadEvent(Resources & res)
			: EngineEvent(EV_Unload)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ExitEvent final : public EngineEvent
	{
		Resources & res;
		ExitEvent(Resources & res)
			: EngineEvent(EV_Exit)
			, res(res)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_