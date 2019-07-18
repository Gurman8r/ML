#ifndef _ML_TRIG_HPP_
#define _ML_TRIG_HPP_

#include <ML/Core/StaticValue.hpp>
#include <gcem/gcem.hpp>

namespace ml
{
	// Trigonometry
	/* * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr T cos(const T & value)
		{
			return gcem::cos<T>(value);
		}

		template <
			class T
		> static constexpr T sin(const T & value)
		{
			return gcem::sin<T>(value);
		}

		template <
			class T
		> static constexpr T tan(const T value)
		{
			return gcem::tan<T>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr T acos(const T & value)
		{
			return gcem::acos<T>(value);
		}

		template <
			class T
		> static constexpr T asin(const T & value)
		{
			return gcem::asin<T>(value);
		}

		template <
			class T
		> static constexpr auto atan(const T & value)
		{
			return gcem::atan<T>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class Tx, class Ty
		> static constexpr T acos2(const Tx & x, const Ty & y)
		{
			return static_value<T> { gcem::acos2<Tx, Ty>(x, y) };
		}

		template <
			class T, class Tx, class Ty
		> static constexpr T asin2(const Tx & x, const Ty & y)
		{
			return static_value<T> { gcem::asin2<Tx, Ty>(x, y) };
		}

		template <
			class T, class Tx, class Ty
		> static constexpr T atan2(const Tx & x, const Ty & y)
		{
			return static_value<T>{ gcem::atan2<Tx, Ty>(x, y) };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRIG_HPP_