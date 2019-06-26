#ifndef _ML_AL_HPP_
#define _ML_AL_HPP_

#include <ML/Core/CString.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace AL
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Err : int32_t
		{
			NoError	= 0,				// AL_NO_ERROR
			InvalidName = 0xA001,		// AL_INVALID_NAME
			InvalidEnum,				// AL_INVALID_ENUM
			InvalidValue,				// AL_INVALID_VALUE
			InvalidOperation,			// AL_INVALID_OPERATION
			OutOfMemory					// AL_OUT_OF_MEMORY
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum StringID : int32_t
		{
			Vendor = 0xB001,			// AL_VENDOR    
			Version,					// AL_VERSION   
			Renderer,					// AL_RENDERER  
			Extensions					// AL_EXTENSIONS
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Format : int32_t
		{
			Mono8 = 0x1027,				// AL_FORMAT_MONO8
			Mono16,						// AL_FORMAT_MONO16
			Stereo8,					// AL_FORMAT_STEREO8
			Stereo16					// AL_FORMAT_STEREO16
		};

		static constexpr Format to_format(uint6_t channels, uint6_t samples)
		{
			switch (samples)
			{
			case 16	: return (channels > 1) ? Format::Stereo16 : Format::Mono16;
			case 8	: return (channels > 1) ? Format::Stereo8 : Format::Mono8;
			default	: return static_cast<Format>(-1);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#include <ML/Audio/AL.inl>

#endif // !_ML_AL_HPP_