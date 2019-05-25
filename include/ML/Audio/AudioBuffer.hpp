#ifndef _ML_AUDIO_BUFFER_HPP_
#define _ML_AUDIO_BUFFER_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API AudioBuffer
		: public IObject
		, public IHandle<uint32_t>
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