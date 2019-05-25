#ifndef _ML_NAME_OF_HPP_
#define _ML_NAME_OF_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp

#include <ML/Core/Meta.hpp>
#include <ML/Core/Signature.hpp>
#include <ML/Core/StaticValue.hpp>

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
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	constexpr XString ml_nameof(type_tag<String>) { return "ml::String"; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NAME_OF_HPP_