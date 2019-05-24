#ifndef _ML_NAME_OF_HPP_
#define _ML_NAME_OF_HPP_

// https://github.com/Manu343726/ml/blob/master/include/ml/nameof.hpp
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/signature.hpp

#include <ML/Core/Meta.hpp>
#include <ML/Core/XString.hpp>
#include <ML/Core/StaticValue.hpp>

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

namespace ml
{
	template<typename T>
	struct type_tag
	{
		constexpr type_tag() = default;
		using type = T;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace detail
	{
		constexpr XString filter_prefix(const XString & str, const XString & prefix)
		{
			return str.size() >= prefix.size() ? (str(0, prefix.size()) == prefix ? str(prefix.size(), str.size()) : str) : str;
		}

		constexpr XString leftpad(const XString & str)
		{
			return (str.size() > 0 && str[0] == ' ') ? leftpad(str(1, str.size())) : str;
		}

		constexpr XString filter_class(const XString & type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "class"));
		}

		constexpr XString filter_struct(const XString & type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "struct"));
		}

		constexpr XString filter_typename_prefix(const XString & type_name)
		{
			return filter_struct(filter_class(type_name));
		}

		namespace
		{
			constexpr const char* find_ith_impl(const XString & name, const XString & substring, const char* res, size_t i, bool infinite = false)
			{
				return (name.length() >= substring.length()) ?
					((name(0, substring.length()) == substring) ?
					((i == 0) ?
						name.begin()
						:
						find_ith_impl(name(substring.length(), name.length()), substring, name.begin(), i - 1, infinite))
					:
						find_ith_impl(name(1, name.length()), substring, res, i, infinite))
					:
					(!infinite) ? name.end() : res;
			}
		}

		constexpr const char* find_ith(const XString & name, const XString & substring, size_t i)
		{
			return find_ith_impl(name, substring, name.end(), i);
		}

		constexpr const char* find_last(const XString & name, const XString & substring)
		{
			return find_ith_impl(name, substring, name.end(), -1, true);
		}

		constexpr const char* find(const XString & name, const XString & substring)
		{
			return find_ith(name, substring, 0);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template<typename T>
	constexpr XString nameof();

	template<typename T, T Value>
	constexpr XString nameof();

	namespace detail
	{
		template<typename T, typename = void>
		struct TypeNameLength :
			std::integral_constant<
			size_t,
			nameof<T>().length()
			>
		{
		};

		template<typename R, typename Class>
		struct TypeNameLength<R Class::*, void> :
			std::integral_constant<
			size_t,
			nameof<R Class::*>().length()
			>
		{
		};

		template<typename Enum>
		struct TypeNameLength<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> :
			std::integral_constant<
			size_t,
			nameof<Enum>().length()
			>
		{
		};

	}

	template<typename T, T Value>
	constexpr XString nameof();

	namespace detail
	{

		template<typename T>
		struct another_level_of_indirection {};

		template<typename T, typename = void>
		struct nameof_impl
		{
			static constexpr XString apply()
			{
				return detail::filter_typename_prefix(signature::type<T>().pad(
					ML_SIG_TYPE_FUNCTION_LEFT,
					ML_SIG_TYPE_FUNCTION_RIGHT
				));
			}
		};

		template<typename T>
		struct nameof_impl<T, meta::void_t<decltype(T::ml_nameof())>>
		{
			static constexpr XString apply()
			{
				return T::ml_nameof();
			}
		};

		template<typename T, T Value>
		struct nameof_impl<another_level_of_indirection<static_value<T, Value>>, void>
		{
			static constexpr XString apply()
			{
				return signature::value<T, Value>().pad(
					ML_SIG_VALUE_FUNCTION_LEFT + detail::TypeNameLength<T>::value + ML_SIG_VALUE_FUNCTION_DELIM,
					ML_SIG_VALUE_FUNCTION_RIGHT
				);
			}
		};

		template<typename T, T Value>
		struct nameof_impl<static_value<T, Value>, void>
		{
			static constexpr XString apply()
			{
				return nameof<T, Value>();
			}
		};

	}

	template<typename T>
	constexpr XString ml_nameof(type_tag<T>)
	{
		return detail::nameof_impl<T>::apply();
	}

	template<typename T, T Value>
	constexpr XString ml_nameof(static_value<T, Value>)
	{
		return detail::nameof_impl<detail::another_level_of_indirection<static_value<T, Value>>>::apply();
	}

	template<typename T>
	constexpr XString nameof()
	{
		using namespace ml;
		return ml_nameof(type_tag<T>());
	}

	template<typename T, T Value>
	constexpr XString nameof()
	{
		using namespace ml;
		return ml_nameof(static_value<T, Value>());
	}

#ifdef ML_HAS_VARIABLE_TEMPLATES
	template<typename T>
	constexpr XString nameof_v = nameof<T>();

	// CONSIDER USING nameof_v<ML_STATIC_VALUE(value)> INSTEAD
	template<typename T, T Value>
	constexpr XString nameof_value_v = nameof<T, Value>();
#endif // ML_HAS_VARIABLE_TEMPLATES

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
#define ML_GEN_NAMEOF(TYPE, NAME) \
	constexpr XString ml_nameof(type_tag<TYPE>) { return NAME; }

	ML_GEN_NAMEOF(int, "int")
	ML_GEN_NAMEOF(std::string, "std::string")
	ML_GEN_NAMEOF(String, "ml::String")
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NAME_OF_HPP_