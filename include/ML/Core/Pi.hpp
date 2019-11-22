#ifndef _ML_PI_HPP_
#define _ML_PI_HPP_

#include <ML/Core/Cast.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct pi;

	template <> struct pi<> final { pi() = delete; };

	template <class T> struct pi<T> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename detail::root_t<T>;

		using self_type = typename pi<type>;

		static constexpr auto value		{ static_cast<type>(3.14159265358979323846264338327L) };
		static constexpr auto two		{ value * cast<type>::two };
		static constexpr auto half		{ value * cast<type>::half };
		static constexpr auto quarter	{ value * cast<type>::quarter };
		static constexpr auto third		{ value * cast<type>::third };
		static constexpr auto deg2rad	{ value / cast<type>::one_eighty };
		static constexpr auto rad2deg	{ cast<type>::one_eighty / value };

		constexpr pi() noexcept = default;

		constexpr operator type const &() const { return self_type::value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PI_HPP_