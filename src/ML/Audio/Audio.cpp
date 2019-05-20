#include <ML/Audio/Audio.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Audio/OpenAL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Audio::Audio(EventSystem & eventSystem)
		: m_eventSystem(eventSystem)
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

	EventSystem & Audio::eventSystem() const { return m_eventSystem; }

	/* * * * * * * * * * * * * * * * * * * * */
}