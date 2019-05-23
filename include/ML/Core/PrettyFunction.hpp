#ifndef _ML_PRETTY_FUNCTION_HPP_
#define _ML_PRETTY_FUNCTION_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

#include <ML/Core/XString.hpp>

# if ML_CC_MSC
#	define ML_PRETTY_FUNCTION		__FUNCSIG__
#	define ML_TYPE_PRETTY_PREFIX	"struct ml::XString _cdecl ml::pretty_function::type<"
#	define ML_TYPE_PRETTY_SUFFIX	">(void)"
#	define ML_VALUE_PRETTY_PREFIX	"struct ml::XString _cdecl ml::pretty_function::value<"
#	define ML_VALUE_PRETTY_DELIM	"; T Value = "
#	define ML_VALUE_PRETTY_SUFFIX	">(void)"
# elif ML_CC_CLANG
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_TYPE_PRETTY_PREFIX	"ml::XString ml::pretty_function::type() [T = "
#	define ML_TYPE_PRETTY_SUFFIX	"]"
#	define ML_VALUE_PRETTY_PREFIX	"ml::XString ml::pretty_function::value() [T = "
#	define ML_VALUE_PRETTY_DELIM	"; Value = "
#	define ML_VALUE_PRETTY_SUFFIX	"]"
# elif ML_CC_GNUC
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_TYPE_PRETTY_PREFIX	"constexpr ml::XString ml::pretty_function::type() [with T = "
#	define ML_TYPE_PRETTY_SUFFIX	"]"
#	define ML_VALUE_PRETTY_PREFIX	"constexpr ml::XString ml::pretty_function::value() [with T = "
#	define ML_VALUE_PRETTY_DELIM	"; T Value = "
#	define ML_VALUE_PRETTY_SUFFIX	"]"
# endif

# define ML_TYPE_PRETTY_FUNCTION_LEFT	(sizeof(ML_TYPE_PRETTY_PREFIX) - 1)
# define ML_TYPE_PRETTY_FUNCTION_RIGHT	(sizeof(ML_TYPE_PRETTY_SUFFIX) - 1)
# define ML_VALUE_PRETTY_FUNCTION_LEFT	(sizeof(ML_VALUE_PRETTY_PREFIX) - 1)
# define ML_VALUE_PRETTY_FUNCTION_DELIM	(sizeof(ML_VALUE_PRETTY_DELIM) - 1)
# define ML_VALUE_PRETTY_FUNCTION_RIGHT	(sizeof(ML_VALUE_PRETTY_SUFFIX) - 1)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace pretty_function
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PRETTY_FUNCTION_HPP_