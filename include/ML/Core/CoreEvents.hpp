#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CoreEvents final
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = Event::EV_CORE,

			EV_FileSystem,

			MAX_CORE_EVENT
		};

		static_assert(MAX_CORE_EVENT < (MIN_CORE_EVENT + (int32_t)Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FileSystemEvent final : public I_Event<CoreEvents::EV_FileSystem>
	{
		constexpr FileSystemEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_