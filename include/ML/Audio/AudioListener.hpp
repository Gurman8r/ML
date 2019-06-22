#ifndef _ML_AUDIO_LISTENER_HPP_
#define _ML_AUDIO_LISTENER_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	struct ML_AUDIO_API AudioListener final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AudioListener();
		~AudioListener();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_AUDIO_LISTENER_HPP_