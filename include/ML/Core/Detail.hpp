#ifndef _ML_DETAIL_HPP_
#define _ML_DETAIL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// https://github.com/Manu343726/ctti/tree/master/include/ctti

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/XString.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if ML_CC_MS
#	define ML_PRETTY_FUNCTION		__FUNCSIG__
#	define ML_TYPE_PRETTY_PREFIX	"class XString pretty_function::type<"
#	define ML_TYPE_PRETTY_SUFFIX	">(void)"
#	define ML_VALUE_PRETTY_PREFIX	"class XString pretty_function::value<"
#	define ML_VALUE_PRETTY_DELIM	"; T Value = "
#	define ML_VALUE_PRETTY_SUFFIX	">(void)"
# elif ML_CC_CLANG
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_TYPE_PRETTY_PREFIX	"XString pretty_function::type() [T = "
#	define ML_TYPE_PRETTY_SUFFIX	"]"
#	define ML_VALUE_PRETTY_PREFIX	"XString pretty_function::value() [T = "
#	define ML_VALUE_PRETTY_DELIM	"; Value = "
#	define ML_VALUE_PRETTY_SUFFIX	"]"
# elif ML_CC_GNU
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_TYPE_PRETTY_PREFIX	"constexpr XString pretty_function::type() [with T = "
#	define ML_TYPE_PRETTY_SUFFIX	"]"
#	define ML_VALUE_PRETTY_PREFIX	"constexpr XString pretty_function::value() [with T = "
#	define ML_VALUE_PRETTY_DELIM	"; T Value = "
#	define ML_VALUE_PRETTY_SUFFIX	"]"
# endif

# define ML_TYPE_PRETTY_FUNCTION_LEFT	(sizeof(ML_TYPE_PRETTY_PREFIX) - 1)
# define ML_TYPE_PRETTY_FUNCTION_RIGHT	(sizeof(ML_TYPE_PRETTY_SUFFIX) - 1)
# define ML_VALUE_PRETTY_FUNCTION_LEFT	(sizeof(ML_VALUE_PRETTY_PREFIX) - 1)
# define ML_VALUE_PRETTY_FUNCTION_DELIM	(sizeof(ML_VALUE_PRETTY_DELIM) - 1)
# define ML_VALUE_PRETTY_FUNCTION_RIGHT	(sizeof(ML_VALUE_PRETTY_SUFFIX) - 1)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace meta
	{
		template<typename T>
		using void_t = typename std::conditional<sizeof(T*) >= 0, void, T>::type;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template<typename T, T Value>
	struct static_value
	{
		constexpr static_value() = default;
		using value_type = T;
		static constexpr value_type value = Value;

		constexpr operator value_type() const
		{
			return Value;
		}

		constexpr value_type get() const
		{
			return Value;
		}

		friend constexpr bool operator==(const static_value& lhs, const value_type rhs)
		{
			return lhs.get() == rhs;
		}

		friend constexpr bool operator==(const value_type lhs, const static_value& rhs)
		{
			return rhs == lhs;
		}

		friend constexpr bool operator!=(const static_value& lhs, const value_type rhs)
		{
			return !(lhs == rhs);
		}

		friend constexpr bool operator!=(const value_type lhs, const static_value& rhs)
		{
			return !(lhs == rhs);
		}
	};

	template<typename T, T Value>
	constexpr T static_value<T, Value>::value;
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
			constexpr CString find_ith_impl(const XString & name, const XString & substring, CString res, size_t i, bool infinite = false)
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

		constexpr CString find_ith(const XString & name, const XString & substring, size_t i)
		{
			return find_ith_impl(name, substring, name.end(), i);
		}

		constexpr CString find_last(const XString & name, const XString & substring)
		{
			return find_ith_impl(name, substring, name.end(), -1, true);
		}

		constexpr CString find(const XString & name, const XString & substring)
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
			std::size_t,
			nameof<T>().length()
			>
		{
		};

		template<typename R, typename Class>
		struct TypeNameLength<R Class::*, void> :
			std::integral_constant<
			std::size_t,
			nameof<R Class::*>().length()
			>
		{
		};

		template<typename Enum>
		struct TypeNameLength<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> :
			std::integral_constant<
			std::size_t,
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
				return detail::filter_typename_prefix(pretty_function::type<T>().pad(
					ML_TYPE_PRETTY_FUNCTION_LEFT,
					ML_TYPE_PRETTY_FUNCTION_RIGHT
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
				return pretty_function::value<T, Value>().pad(
					ML_VALUE_PRETTY_FUNCTION_LEFT + detail::TypeNameLength<T>::value + ML_VALUE_PRETTY_FUNCTION_DELIM,
					ML_VALUE_PRETTY_FUNCTION_RIGHT
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

#ifdef ML_VARIABLE_TEMPLATES
	template<typename T>
	constexpr XString nameof_v = nameof<T>();

	template<typename T, T Value>
	constexpr XString nameof_value_v = nameof<T, Value>();
#endif // ML_HAS_VARIABLE_TEMPLATES
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	constexpr ml::XString ml_nameof(ml::type_tag<std::string>)
	{
		return "std::string";
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_DETAIL_HPP_