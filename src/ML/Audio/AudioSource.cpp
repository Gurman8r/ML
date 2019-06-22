#include <ML/Audio/AudioSource.hpp>
#include <ML/Audio/OpenAL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	AudioSource::AudioSource()
		: I_Handle(NULL)
		, m_count(0)
	{
	}

	AudioSource::AudioSource(const AudioSource & copy)
		: I_Handle(copy)
		, m_count(copy.m_count)
	{
	}

	AudioSource::~AudioSource()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AudioSource & AudioSource::clean()
	{
		if ((*this))
		{
			OpenAL::deleteBuffers(m_count, (*this));
		}
		return (*this);
	}

	AudioSource & AudioSource::create(uint32_t count)
	{
		if (set_handle(ML_AL.genSources(count)))
		{
			m_count = count;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}