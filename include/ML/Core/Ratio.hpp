#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <ML/Core/Numeric.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		int64_t Num, int64_t Den = 1
	> using Ratio = typename std::ratio<Num, Den>;

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		template <
			class T, int64_t Num, int64_t Den
		> static constexpr T ratio_cast(T value, const Ratio<Num, Den> & r)
		{
			using TT = numeric<T>;
			const T num { TT(r.num) };
			const T den { TT(r.den) };
			return (((num == TT::one) && (den == TT::one))
				? (value)
				: (((num != TT::one) && (den == TT::one))
					? (value * num)
					: (((num == TT::one) && (den != TT::one))
						? (value / den)
						: (value * num / den)
						)));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RATIO_HPP_