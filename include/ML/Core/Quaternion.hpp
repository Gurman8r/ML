#ifndef _ML_QUATERNION_HPP_
#define _ML_QUATERNION_HPP_

#include <ML/Core/Matrix.hpp>
#include <ML/Core/Trig.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct Quaternion final : public tvec4<T>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Quaternion<value_type>;
		using base_type			= typename tvec4<value_type>;
		using complex_type		= typename tvec3<value_type>;
		using type				= typename base_type::type;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Quaternion()
			: base_type()
		{
		}

		constexpr Quaternion(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type({ x, y, z, w })
		{
		}

		constexpr Quaternion(const self_type & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> constexpr Quaternion(const tvec4<U> & copy)
			: base_type(copy)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr complex_type complex() const
		{
			return complex_type { (*this)[0], (*this)[1], (*this)[2] };
		}

		constexpr const_reference real() const
		{
			return (*this)[3];
		}

		constexpr value_type angle() const
		{
			return alg::acos(this->real()) * type::two;
		}

		constexpr complex_type axis() const
		{
			const value_type temp { type::one - this->real() * this->real() };
			if (temp < type::zero)
			{
				return complex_type { 0, 0, 1 };
			}
			else
			{
				return this->complex() * (type::one / alg::sqrt<value_type>()(temp));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr complex_type eulerAngles() const
		{
			return complex_type { pitch(), roll(), yaw() };
		}

		constexpr value_type pitch() const
		{
			return alg::atan2<value_type>(
				(type::two * ((*this)[1] * (*this)[2] + this->real() * (*this)[0])),
				(this->real() * this->real() - (*this)[0] *
					(*this)[0] - (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2])
			);
		}

		constexpr value_type roll() const
		{
			return alg::atan2<value_type>(
				(type::two * ((*this)[0] * (*this)[1] + this->real() * (*this)[2])),
				(this->real() * this->real() + (*this)[0] * (*this)[0] -
					(*this)[1] * (*this)[1] - (*this)[2] * (*this)[2])
			);
		}

		constexpr value_type yaw() const
		{
			return alg::asin<value_type>(ML_CLAMP(
				(type::two *
					((*this)[0] * (*this)[2] - this->real() * (*this)[1])),
				type::minus_one,
				type::one
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr tmat3<value_type> toMat3() const
		{
			// not sure if this is correct
			return tmat3<value_type> {
				(type::one - type::two * ((*this)[1] * (*this)[1]) - type::two * ((*this)[2] * (*this)[2])),
				(type::two * ((*this)[0] * (*this)[1]) - type::two * ((*this)[2] * (*this)[3])),
				(type::two * ((*this)[0] * (*this)[2]) + type::two * ((*this)[1] * (*this)[3])),
				(type::two * ((*this)[0] * (*this)[1]) + type::two * ((*this)[2] * (*this)[3])),
				(type::one - type::two * ((*this)[0] * (*this)[0]) - type::two * ((*this)[2] * (*this)[2])),
				(type::two * ((*this)[1] * (*this)[2]) - type::two * ((*this)[0] * (*this)[3])),
				(type::two * ((*this)[0] * (*this)[2]) - type::two * ((*this)[1] * (*this)[3])),
				(type::two * ((*this)[1] * (*this)[2]) + type::two * ((*this)[0] * (*this)[3])),
				(type::one - type::two * ((*this)[0] * (*this)[0]) - type::two * ((*this)[1] * (*this)[1]))
			};
		}

		constexpr tmat4<value_type> toMat4() const
		{
			const tmat3<value_type> temp = toMat3();
			return tmat4<value_type> {
				temp[0],	temp[1],	temp[2],	type::zero,
				temp[3],	temp[4],	temp[5],	type::zero,
				temp[6],	temp[7],	temp[8],	type::zero,
				type::zero, type::zero, type::zero, type::one,
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type angleAxis(const_reference angle, const complex_type & axis)
		{
			const value_type half_angle { angle * type::half };
			const value_type temp { alg::sin(half_angle) };
			return self_type {
				axis[0] * temp,
				axis[1] * temp,
				axis[2] * temp,
				alg::cos(half_angle)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr operator base_type() const
		{
			return base_type { (*this)[0], (*this)[1], (*this)[2], (*this)[3] };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> 
	using tquat = Quaternion<T>;
	using quat	= tquat<float32_t>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_QUATERNION_HPP_