#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		int64_t N,
		int64_t D = 1
	> using Ratio = typename std::ratio<N, D>;

	/* * * * * * * * * * * * * * * * * * * * */

//#pragma warning(push)
//#pragma warning(disable: 4309) // C4309 truncation of constant value

	using Atto	= typename Ratio<1LL, 1000000000000000000LL>;
	using Femto = typename Ratio<1LL, 1000000000000000LL>;
	using Pico	= typename Ratio<1LL, 1000000000000LL>;
	using Nano	= typename Ratio<1LL, 1000000000LL>;
	using Micro = typename Ratio<1LL, 1000000LL>;
	using Milli	= typename Ratio<1LL, 1000LL>;
	using Centi = typename Ratio<1LL, 100LL>;
	using Deci	= typename Ratio<1LL, 10LL>;
	using Deca	= typename Ratio<10LL, 1LL>;
	using Hecto = typename Ratio<100LL, 1LL>;
	using Kilo	= typename Ratio<1000LL, 1LL>;
	using Mega	= typename Ratio<1000000LL, 1LL>;
	using Giga	= typename Ratio<1000000000LL, 1LL>;
	using Tera	= typename Ratio<100000000000LL, 1LL>;
	using Peta	= typename Ratio<1000000000000000LL, 1LL>;
	using Exa	= typename Ratio<1000000000000000000LL, 1LL>;

//#pragma warning(pop)

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, 
		int64_t N,
		int64_t D
	> constexpr T ratio_cast(const T & value, const Ratio<N, D> & r)
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