#ifndef _ML_AUDIO_HPP_
#define _ML_AUDIO_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API Audio final
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		explicit Audio(EventSystem & eventSystem);
		~Audio();

	public:
		void onEvent(const Event * value) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_HPP_
