#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		intmax_t Num,
		intmax_t Den = 1
	> using Ratio = typename std::ratio<Num, Den>;

	/* * * * * * * * * * * * * * * * * * * * */

	using Atto	= typename Ratio<1, 1000000000000000000ULL>;
	using Femto = typename Ratio<1, 1000000000000000ULL>;
	using Pico	= typename Ratio<1, 1000000000000ULL>;
	using Nano	= typename Ratio<1, 1000000000>;
	using Micro = typename Ratio<1, 1000000>;
	using Milli	= typename Ratio<1, 1000>;
	using Centi = typename Ratio<1, 100>;
	using Deci	= typename Ratio<1, 10>;
	using Deca	= typename Ratio<10, 1>;
	using Hecto = typename Ratio<100, 1>;
	using Kilo	= typename Ratio<1000, 1>;
	using Mega	= typename Ratio<1000000, 1>;
	using Giga	= typename Ratio<1000000000, 1>;
	using Tera	= typename Ratio<1000000000000ULL, 1>;
	using Peta	= typename Ratio<1000000000000000ULL, 1>;
	using Exa	= typename Ratio<1000000000000000000ULL, 1>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		typename T, 
		intmax_t Num,
		intmax_t Den
	> constexpr T ratio_cast(const T & value, const Ratio<Num, Den> & r)
	{
		return 
			((((T)(r.num) == (T)(1)) && ((T)(r.den) == (T)(1)))
				? (value)
				: ((((T)(r.num) != (T)(1)) && ((T)(r.den) == (T)(1)))
					? (value * (T)(r.num))
					: ((((T)(r.num) == (T)(1)) && ((T)(r.den) != (T)(1)))
						? (value / (T)(r.den))
						: (value * (T)(r.num) / (T)(r.den))
					)
				)
			);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RATIO_HPP_