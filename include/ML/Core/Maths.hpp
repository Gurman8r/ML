#ifndef _ML_MATHS_HPP_
#define _ML_MATHS_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace maths
	{
		constexpr float Pi		{ 3.14159265359f };
		constexpr float Deg2Rad { Pi / 180.f };
		constexpr float Rad2Deg { 180.f / Pi };

		constexpr float to_radians(float degrees) { return Deg2Rad * degrees; }
		constexpr float to_degrees(float radians) { return Rad2Deg * radians; }

		template <
			typename Value, typename Coeff = typename float
		> constexpr Value lerp(Value a, Value b, Coeff c)
		{
			return (a * c + b * (1 - c));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATHS_HPP_