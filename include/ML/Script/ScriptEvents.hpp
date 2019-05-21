#ifndef _ML_SCRIPT_EVENTS_HPP_
#define _ML_SCRIPT_EVENTS_HPP_

#include <ML/Core/Event.hpp>
#include <ML/Script/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ScriptEvent final
	{
		enum : int32_t
		{
			MIN_SCRIPT_EVENT = Event::EV_SCRIPT + 1,

			EV_Command,

			MAX_SCRIPT_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API CommandEvent : public IEvent<ScriptEvent::EV_Command>
	{
		const String & cmd;
		CommandEvent(const String & cmd)
			: cmd(cmd)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_EVENTS_HPP_