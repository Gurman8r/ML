#ifndef _ML_CONTEXT_HPP_
#define _ML_CONTEXT_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ContextSettings final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Profile : uint32_t
		{
			Any		= 0,
			Core    = 1,
			Compat	= 2,
			Debug   = 3,
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

		constexpr ContextSettings(
			uint32_t	majorVersion	= 1,
			uint32_t	minorVersion	= 1,
			uint32_t	profile			= Profile::Compat,
			uint32_t	depthBits		= 0,
			uint32_t	stencilBits		= 0,
			bool		multisample		= false,
			bool		srgbCapable		= false)
			: majorVersion	(majorVersion)
			, minorVersion	(minorVersion)
			, profile		(profile)
			, depthBits		(depthBits)
			, stencilBits	(stencilBits)
			, multisample	(multisample)
			, srgbCapable	(srgbCapable)
		{
		};

		constexpr ContextSettings(const ContextSettings & copy)
			: majorVersion	(copy.majorVersion)
			, minorVersion	(copy.minorVersion)
			, profile		(copy.profile)
			, depthBits		(copy.depthBits)
			, stencilBits	(copy.stencilBits)
			, multisample	(copy.multisample)
			, srgbCapable	(copy.srgbCapable)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_HPP_