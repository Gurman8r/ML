#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CoreEvents final
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = Event::EV_CORE,

			EV_FileSystem,

			MAX_CORE_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FileSystemEvent final : public IEvent<CoreEvents::EV_FileSystem>
	{
		constexpr FileSystemEvent()
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_