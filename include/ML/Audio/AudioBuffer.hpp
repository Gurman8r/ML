#ifndef _ML_AUDIO_BUFFER_HPP_
#define _ML_AUDIO_BUFFER_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioBuffer
		: public Trackable
		, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioBuffer();
		AudioBuffer(AudioBuffer const & copy);
		~AudioBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioBuffer & clean();
		AudioBuffer & create(uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t m_count;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_BUFFER_HPP_