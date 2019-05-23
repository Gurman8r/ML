#ifndef _ML_NAME_OF_HPP_
#define _ML_NAME_OF_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp

#include <ML/Core/Meta.hpp>
#include <ML/Core/NameFilters.hpp>
#include <ML/Core/PrettyFunction.hpp>
#include <ML/Core/StaticValue.hpp>
#include <ML/Core/TypeTag.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			ml::nameof<T>().length()
			>
		{
		};

		template<typename R, typename Class>
		struct TypeNameLength<R Class::*, void> :
			std::integral_constant<
			std::size_t,
			ml::nameof<R Class::*>().length()
			>
		{
		};

		template<typename Enum>
		struct TypeNameLength<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> :
			std::integral_constant<
			std::size_t,
			ml::nameof<Enum>().length()
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
				return ml::detail::filter_typename_prefix(ml::pretty_function::type<T>().pad(
					ML_TYPE_PRETTY_FUNCTION_LEFT,
					ML_TYPE_PRETTY_FUNCTION_RIGHT
				));
			}
		};

		template<typename T>
		struct nameof_impl<T, ml::meta::void_t<decltype(T::ml_nameof())>>
		{
			static constexpr XString apply()
			{
				return T::ml_nameof();
			}
		};

		template<typename T, T Value>
		struct nameof_impl<another_level_of_indirection<ml::static_value<T, Value>>, void>
		{
			static constexpr XString apply()
			{
				return ml::pretty_function::value<T, Value>().pad(
					ML_VALUE_PRETTY_FUNCTION_LEFT + ml::detail::TypeNameLength<T>::value + ML_VALUE_PRETTY_FUNCTION_DELIM,
					ML_VALUE_PRETTY_FUNCTION_RIGHT
				);
			}
		};

		template<typename T, T Value>
		struct nameof_impl<ml::static_value<T, Value>, void>
		{
			static constexpr XString apply()
			{
				return ml::nameof<T, Value>();
			}
		};

	}

	template<typename T>
	constexpr XString ml_nameof(ml::type_tag<T>)
	{
		return ml::detail::nameof_impl<T>::apply();
	}

	template<typename T, T Value>
	constexpr XString ml_nameof(ml::static_value<T, Value>)
	{
		return ml::detail::nameof_impl<ml::detail::another_level_of_indirection<ml::static_value<T, Value>>>::apply();
	}

	template<typename T>
	constexpr XString nameof()
	{
		using namespace ml;
		return ml_nameof(ml::type_tag<T>());
	}

	template<typename T, T Value>
	constexpr XString nameof()
	{
		using namespace ml;
		return ml_nameof(ml::static_value<T, Value>());
	}

#ifdef ML_HAS_VARIABLE_TEMPLATES
	template<typename T>
	constexpr XString nameof_v = ml::nameof<T>();

	// CONSIDER USING nameof_v<ML_STATIC_VALUE(value)> INSTEAD
	template<typename T, T Value>
	constexpr XString nameof_value_v = ml::nameof<T, Value>();
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr ml::XString ml_nameof(ml::type_tag<std::string>)
	{
		return "std::string";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAME_OF_HPP_