#ifndef _ML_MATHS_HPP_
#define _ML_MATHS_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace maths
	{
		
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace maths
	{
		constexpr float Pi		{ 3.14159265359f };
		constexpr float Deg2Rad { Pi / 180.f };
		constexpr float Rad2Deg { 180.f / Pi };

		constexpr float to_radians(float degrees) { return Deg2Rad * degrees; }
		constexpr float to_degrees(float radians) { return Rad2Deg * radians; }

		template <
			typename Value,
			typename Coeff = typename float
		> constexpr Value lerp(Value a, Value b, Coeff c)
		{
			return (a * c + b * (1 - c));
		}

		template <typename T>
		constexpr const T & map_range(
			const T & value,
			const T & min1,
			const T & max1,
			const T & min2,
			const T & max2)
		{
			return (min2 + (value - min1) * (max2 - min2) / (max1 - min1));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace maths
	{
		template <typename T>
		constexpr T bitRead(T value, T bit)
		{
			return ((value >> bit) & static_cast<T>(1));
		}

		template <typename T>
		constexpr T & bitSet(T & value, T bit)
		{
			return (value |= (static_cast<T>(1) << bit));
		}

		template <typename T>
		constexpr T & bitClear(T & value, T bit)
		{
			return (value &= ~(static_cast<T>(1) << bit));
		}

		template <typename T>
		constexpr T & bitWrite(T & value, T bit, T bitValue)
		{
			return (bitValue ? bitSet(value, bit) : bitClear(value, bit));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATHS_HPP_