#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sound::Sound()
	{
		
	}

	Sound::~Sound()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sound::dispose()
	{
		return false;
	}

	bool Sound::loadFromFile(const String & filename)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}