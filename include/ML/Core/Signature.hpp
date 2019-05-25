#ifndef _ML_SIGNATURE_HPP_
#define _ML_SIGNATURE_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

#include <ML/Core/XString.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if ML_CC_MSC
#	define ML_PRETTY_FUNCTION	__FUNCSIG__
#	define ML_SIG_TYPE_PREFIX	"struct ml::XString _cdecl ml::signature::type<"
#	define ML_SIG_TYPE_SUFFIX	">(void)"
#	define ML_SIG_VALUE_PREFIX	"struct ml::XString _cdecl ml::signature::value<"
#	define ML_SIG_VALUE_DELIM	"; T Value = "
#	define ML_SIG_VALUE_SUFFIX	">(void)"
# elif ML_CC_CLANG
#	define ML_PRETTY_FUNCTION	__PRETTY_FUNCTION__
#	define ML_SIG_TYPE_PREFIX	"ml::XString ml::signature::type() [T = "
#	define ML_SIG_TYPE_SUFFIX	"]"
#	define ML_SIG_VALUE_PREFIX	"ml::XString ml::signature::value() [T = "
#	define ML_SIG_VALUE_DELIM	"; Value = "
#	define ML_SIG_VALUE_SUFFIX	"]"
# elif ML_CC_GNUC
#	define ML_PRETTY_FUNCTION	__PRETTY_FUNCTION__
#	define ML_SIG_TYPE_PREFIX	"constexpr ml::XString ml::signature::type() [with T = "
#	define ML_SIG_TYPE_SUFFIX	"]"
#	define ML_SIG_VALUE_PREFIX	"constexpr ml::XString ml::signature::value() [with T = "
#	define ML_SIG_VALUE_DELIM	"; T Value = "
#	define ML_SIG_VALUE_SUFFIX	"]"
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# define ML_SIG_TYPE_FUNCTION_LEFT		(sizeof(ML_SIG_TYPE_PREFIX)  - 1)
# define ML_SIG_TYPE_FUNCTION_RIGHT		(sizeof(ML_SIG_TYPE_SUFFIX)  - 1)
# define ML_SIG_VALUE_FUNCTION_LEFT		(sizeof(ML_SIG_VALUE_PREFIX) - 1)
# define ML_SIG_VALUE_FUNCTION_DELIM	(sizeof(ML_SIG_VALUE_DELIM)  - 1)
# define ML_SIG_VALUE_FUNCTION_RIGHT	(sizeof(ML_SIG_VALUE_SUFFIX) - 1)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace signature
	{
		template<typename T>
		constexpr XString type()
		{
			return { ML_PRETTY_FUNCTION };
		}

		template<typename T, T Value>
		constexpr XString value()
		{
			return { ML_PRETTY_FUNCTION };
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SIGNATURE_HPP_