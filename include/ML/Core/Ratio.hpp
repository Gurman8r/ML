#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <ML/Core/Type.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		int64_t N,
		int64_t D = 1
	> using Ratio = typename std::ratio<N, D>;

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, 
		int64_t N,
		int64_t D
	> constexpr T ratio_cast(const T & value, const Ratio<N, D> & r)
	{
		using type = type_t<T>;
		const type num { r.num };
		const type den { r.den };
		return 
			(((num == type::one) && (den == type::one))
				? (value)
				: (((num != type::one) && (den == type::one))
					? (value * num)
					: (((num == type::one) && (den != type::one))
						? (value / den)
						: (value * num / den)
					)
				)
			);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RATIO_HPP_