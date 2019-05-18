#include <ML/Window/ContextSettings.hpp>

namespace ml
{
	ContextSettings::ContextSettings(
		uint32_t		majorVersion,
		uint32_t		minorVersion,
		uint32_t		profile,
		uint32_t		depthBits,
		uint32_t		stencilBits,
		bool			multisample,
		bool			srgbCapable)
		: majorVersion	(majorVersion)
		, minorVersion	(minorVersion)
		, profile		(profile)
		, depthBits		(depthBits)
		, stencilBits	(stencilBits)
		, multisample	(multisample)
		, srgbCapable	(srgbCapable)
	{};

	ContextSettings::ContextSettings(const ContextSettings & copy)
		: majorVersion(	copy.majorVersion)
		, minorVersion(	copy.minorVersion)
		, profile(		copy.profile)
		, depthBits(	copy.depthBits)
		, stencilBits(	copy.stencilBits)
		, multisample(	copy.multisample)
		, srgbCapable(	copy.srgbCapable)
	{};
}