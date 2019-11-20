#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <ML/Core/Cast.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING_T_(int64_t Num, int64_t Den = 1) Ratio = typename _STD ratio<Num, Den>;

	ML_USING Atto	= typename Ratio<1LL, 1000000000000000000LL>;
	ML_USING Femto	= typename Ratio<1LL, 1000000000000000LL>;
	ML_USING Pico	= typename Ratio<1LL, 1000000000000LL>;
	ML_USING Nano	= typename Ratio<1LL, 1000000000LL>;
	ML_USING Micro	= typename Ratio<1LL, 1000000LL>;
	ML_USING Milli	= typename Ratio<1LL, 1000LL>;
	ML_USING Centi	= typename Ratio<1LL, 100LL>;
	ML_USING Deci	= typename Ratio<1LL, 10LL>;
	ML_USING Deca	= typename Ratio<10LL, 1LL>;
	ML_USING Hecto	= typename Ratio<100LL, 1LL>;
	ML_USING Kilo	= typename Ratio<1000LL, 1LL>;
	ML_USING Mega	= typename Ratio<1000000LL, 1LL>;
	ML_USING Giga	= typename Ratio<1000000000LL, 1LL>;
	ML_USING Tera	= typename Ratio<100000000000LL, 1LL>;
	ML_USING Peta	= typename Ratio<1000000000000000LL, 1LL>;
	ML_USING Exa	= typename Ratio<1000000000000000000LL, 1LL>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		template <
			class T, int64_t Num, int64_t Den
		> static constexpr T ratio_cast(T value, const Ratio<Num, Den> & r)
		{
			using TT = cast<T>;
			const T num { TT {}(r.num) };
			const T den { TT {}(r.den) };
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RATIO_HPP_