#ifndef _ML_SIGNATURE_HPP_
#define _ML_SIGNATURE_HPP_

#include <ML/Core/StringView.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Source: https://github.com/Manu343726/ctti
# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::StringView __cdecl ml::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_GNU)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"constexpr ml::StringView ml::signature() [with T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"ml::StringView ml::signature() [T = "
#	define ML_SIGNATURE_SUF	"]"
# else
#	error Type information is not available.
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <
		class T
	> static constexpr auto signature() { return StringView { ML_SIGNATURE }; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SIGNATURE_HPP_