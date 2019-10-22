#ifndef _ML_SIGNATURE_HPP_
#define _ML_SIGNATURE_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StringView.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE					__FUNCSIG__
#	define ML_TYPE_SIGNATURE_PREFIX		"struct ml::StringView __cdecl ml::signature::type<"
#	define ML_TYPE_SIGNATURE_SUFFIX		">(void)"
#	define ML_VALUE_SIGNATURE_PREFIX	"struct ml::StringView __cdecl ml::signature::value<"
#	define ML_VALUE_SIGNATURE_SEPARATOR	"; T Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	">(void)"
# elif defined(ML_CC_GCC)
#	define ML_SIGNATURE					__PRETTY_FUNCTION__
#	define ML_TYPE_SIGNATURE_PREFIX		"constexpr ml::StringView ml::signature::type() [with T = "
#	define ML_TYPE_SIGNATURE_SUFFIX		"]"
#	define ML_VALUE_SIGNATURE_PREFIX	"constexpr ml::StringView ml::signature::value() [with T = "
#	define ML_VALUE_SIGNATURE_SEPARATOR	"; Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE					__PRETTY_FUNCTION__
#	define ML_TYPE_SIGNATURE_PREFIX		"ml::StringView ml::signature::type() [T = "
#	define ML_TYPE_SIGNATURE_SUFFIX		"]"
#	define ML_VALUE_SIGNATURE_PREFIX	"ml::StringView ml::signature::value() [T = "
#	define ML_VALUE_SIGNATUTE_SEPARATOR	"; Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	"]"
# else
#	error Type information is not available.
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace signature
	{
#if defined(ML_HAS_CONSTEXPR_17)
		namespace detail
		{
			static constexpr std::tuple<StringView, StringView> type
			{
				ML_TYPE_SIGNATURE_PREFIX, 
				ML_TYPE_SIGNATURE_SUFFIX
			};

			static constexpr std::tuple<StringView, StringView, StringView> value
			{
				ML_VALUE_SIGNATURE_PREFIX,
				ML_VALUE_SIGNATURE_SEPARATOR, 
				ML_VALUE_SIGNATURE_SUFFIX
			};
		}
#endif
		template <class T> static constexpr StringView type()
		{ 
			return { ML_SIGNATURE };
		}

		template <class T, T Value> static constexpr StringView value()
		{ 
			return { ML_SIGNATURE };
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SIGNATURE_HPP_