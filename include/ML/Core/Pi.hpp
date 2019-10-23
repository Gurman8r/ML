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

		using type = typename detail::decay_t<T>;

		using self_type = typename pi<type>;

		static constexpr type value		{ static_cast<type>(3.14159265358979323846264338327L) };
		static constexpr type two		{ value * cast<type>::two };
		static constexpr type half		{ value * cast<type>::half };
		static constexpr type quarter	{ value * cast<type>::quarter };
		static constexpr type third		{ value * cast<type>::third };
		static constexpr type deg2rad	{ value / cast<type>::one_eighty };
		static constexpr type rad2deg	{ cast<type>::one_eighty / value };

		constexpr pi() noexcept = default;

		constexpr operator const type &() const { return self_type::value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PI_HPP_