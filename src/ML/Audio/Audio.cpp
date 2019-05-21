#include <ML/Audio/Audio.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Audio/OpenAL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Audio::Audio(EventSystem & eventSystem)
		: IEventListener(eventSystem)
	{
		ML_AL.init(eventSystem);
	}

	Audio::~Audio()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Audio::onEvent(const IEvent * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}