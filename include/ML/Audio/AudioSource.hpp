#ifndef _ML_AUDIO_SOURCE_HPP_
#define _ML_AUDIO_SOURCE_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API AudioSource
		: public I_Newable
		, public I_Handle<uint32_t>
	{
	public:
		AudioSource();
		AudioSource(const AudioSource & copy);
		~AudioSource();

	public:
		AudioSource & clean();
		AudioSource & create(uint32_t count);

	private:
		uint32_t m_count;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_SOURCE_HPP_