#ifndef _ML_SCRIPT_EVENTS_HPP_
#define _ML_SCRIPT_EVENTS_HPP_

#include <ML/Core/IEvent.hpp>
#include <ML/Script/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API ScriptEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_SCRIPT_EVENT = IEvent::EV_SCRIPT + 1,

			EV_Command,

			MAX_SCRIPT_EVENT
		};

		ScriptEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API CommandEvent : public ScriptEvent
	{
		const String & cmd;
		CommandEvent(const String & cmd)
			: ScriptEvent(EV_Command)
			, cmd(cmd)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_EVENTS_HPP_