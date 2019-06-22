#ifndef _ML_AUDIO_BUFFER_HPP_
#define _ML_AUDIO_BUFFER_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API AudioBuffer
		: public I_Newable
		, public I_Handle<uint32_t>
	{
	public:
		AudioBuffer();
		AudioBuffer(const AudioBuffer & copy);
		~AudioBuffer();

	public:
		AudioBuffer & clean();
		AudioBuffer & create(uint32_t count);

	private:
		uint32_t m_count;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_BUFFER_HPP_