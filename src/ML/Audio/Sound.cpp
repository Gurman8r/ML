#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sound::Sound()
	{
	}

	Sound::Sound(String const & filename)
		: Sound{}
	{
		loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sound::dispose()
	{
		return false;
	}

	bool Sound::loadFromFile(String const & filename)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}