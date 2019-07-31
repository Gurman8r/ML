#ifndef _ML_SCRIPT_EVENTS_HPP_
#define _ML_SCRIPT_EVENTS_HPP_

#include <ML/Core/I_Event.hpp>

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

		static_assert(MAX_SCRIPT_EVENT < (MIN_SCRIPT_EVENT + Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct CommandEvent final : public I_Event<ScriptEvent::EV_Command>
	{
		C_String cmd;
		constexpr CommandEvent(C_String cmd)
			: cmd(cmd)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_EVENTS_HPP_