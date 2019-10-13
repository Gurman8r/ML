#ifndef _ML_AUDIO_SOURCE_HPP_
#define _ML_AUDIO_SOURCE_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioSource
		: public Newable
		, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioSource();
		AudioSource(const AudioSource & copy);
		~AudioSource();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioSource & clean();
		AudioSource & create(uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioSource & play();
		AudioSource & stop();
		AudioSource & rewind();
		AudioSource & pause();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t m_count;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_SOURCE_HPP_