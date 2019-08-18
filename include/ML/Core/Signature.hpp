#ifndef _ML_SIGNATURE_HPP_
#define _ML_SIGNATURE_HPP_

#include <ML/Core/C_String.hpp>

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE __FUNCSIG__
#	define ML_SIGNATURE_PREFIX "ml::CX_String __cdecl ml::signature_of<"
#	define ML_SIGNATURE_SUFFIX ">(void)"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE __PRETTY_FUNCTION__
# elif defined(ML_CC_GNUC)
#	define ML_SIGNATURE __PRETTY_FUNCTION__
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr CX_String signature_of()
	{
		return { ML_SIGNATURE };
	}

	template <class T> static constexpr CX_String signature_of(T)
	{
		return signature_of<T>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SIGNATURE_HPP_