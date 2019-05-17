#ifndef _ML_AL_HPP_
#define _ML_AL_HPP_

#include <ML/Audio/Export.hpp>
#include <ML/Core/Maths.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AL final
	{
		enum Err : int32_t
		{
			NoError,
			InvalidName,
			InvalidEnum,
			InvalidValue,
			InvalidOperation,
			OutOfMemory,

			MAX_ERROR
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AL_HPP_