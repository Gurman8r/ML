#ifndef _ML_AUDIO_EVENTS_HPP_
#define _ML_AUDIO_EVENTS_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_AUDIO_EVENT = IEvent::EV_AUDIO + 1,
			MAX_AUDIO_EVENT
		};

		AudioEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_EVENTS_HPP_