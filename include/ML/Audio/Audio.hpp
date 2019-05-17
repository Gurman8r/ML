#ifndef _ML_AUDIO_HPP_
#define _ML_AUDIO_HPP_

#include <ML/Core/ISingleton.hpp>
#include <ML/Audio/OpenAL.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Audio/AudioBuffer.hpp>
#include <ML/Audio/AudioSource.hpp>

#define ML_Audio ml::Audio::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API Audio
		: public ITrackable
		, public ISingleton<Audio>
	{
		friend class ISingleton<Audio>;

	private:
		Audio();
		~Audio();

	public:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_HPP_