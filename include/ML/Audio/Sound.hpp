#ifndef _ML_SOUND_HPP_
#define _ML_SOUND_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API Sound
		: public IObject
		, public IDisposable
		, public IReadable
	{
	public:
		Sound();
		~Sound();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	private:
		void * m_device;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SOUND_HPP_