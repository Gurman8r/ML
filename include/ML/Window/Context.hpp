#ifndef _ML_CONTEXT_HPP_
#define _ML_CONTEXT_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Context final
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Profile : uint32_t
		{
			Any		= 0,
			Debug   = 1,
			Core    = 2,
			Compat	= 3,
		};

		/* * * * * * * * * * * * * * * * * * * * */
		
		uint32_t	majorVersion;
		uint32_t	minorVersion;
		uint32_t	profile;
		uint32_t	depthBits;
		uint32_t	stencilBits;
		bool		multisample;
		bool		srgbCapable;

		/* * * * * * * * * * * * * * * * * * * * */

		Context(
			uint32_t	majorVersion	= 1,
			uint32_t	minorVersion	= 1,
			uint32_t	profile			= Profile::Compat,
			uint32_t	depthBits		= 0,
			uint32_t	stencilBits		= 0,
			bool		multisample		= false,
			bool		srgbCapable		= false
		);

		Context(const Context & copy);

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_HPP_