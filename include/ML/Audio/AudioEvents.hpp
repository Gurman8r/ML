#ifndef _ML_AUDIO_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class AudioEventType
	{
		MIN_AUDIO_EVENT = Event::EV_AUDIO,

		// ...

		MAX_AUDIO_EVENT
	};

	static_assert(
		(int32_t)AudioEventType::MAX_AUDIO_EVENT <
		(int32_t)AudioEventType::MIN_AUDIO_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <AudioEventType ID> struct AudioEvent : public T_Event<AudioEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ...

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_EVENTS_HPP_
