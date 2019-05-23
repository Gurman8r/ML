#include <ML/Audio/Audio.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Audio/OpenAL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Audio::Audio(EventSystem & eventSystem)
		: EventListener(eventSystem)
	{
		ML_AL.init(eventSystem);
	}

	Audio::~Audio()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Audio::onEvent(const Event * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}