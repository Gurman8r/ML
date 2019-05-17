#include <ML/Audio/Audio.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
		OpenAL::closeDevice();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}