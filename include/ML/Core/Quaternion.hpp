#ifndef _ML_QUATERNION_HPP_
#define _ML_QUATERNION_HPP_

#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Quaternion final : public tvec4<T>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Quaternion<value_type>;
		using base_type			= typename tvec4<value_type>;
		using complex_type		= typename tvec3<value_type>;
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

		constexpr Quaternion(value_type x, value_type y, value_type z, value_type w)
			: base_type({ x, y, z, w })
		{
		}

		constexpr Quaternion(self_type const & copy)
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
			return gcem::acos(this->real()) * 2.0f;
		}

		constexpr complex_type axis() const
		{
			const auto temp { 1.0f - this->real() * this->real() };
			return ((temp < 0.0f)
				? complex_type{ 0, 0, 1 }
				: this->complex() * (1.0f / alg::sqrt<value_type>()(temp))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr complex_type eulerAngles() const
		{
			return { pitch(), roll(), yaw() };
		}

		constexpr value_type pitch() const
		{
			return gcem::atan2(
				(2.0f * ((*this)[1] * (*this)[2] + (*this)[3] * (*this)[0])),
				((*this)[3] * (*this)[3] - (*this)[0] * (*this)[0] - (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2])
			);
		}

		constexpr value_type roll() const
		{
			return gcem::atan2(
				(2.0f * ((*this)[0] * (*this)[1] + (*this)[3] * (*this)[2])),
				((*this)[3] * (*this)[3] + (*this)[0] * (*this)[0] - (*this)[1] * (*this)[1] - (*this)[2] * (*this)[2])
			);
		}

		constexpr value_type yaw() const
		{
			return gcem::asin(alg::clamp(
				(2.0f * ((*this)[0] * (*this)[2] - (*this)[3] * (*this)[1])),
				-1.0f,
				1.0f
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr tmat3<value_type> as_mat3() const
		{
			// not sure if this is correct
			return tmat3<value_type> {
				(1.0f - 2.0f * ((*this)[1] * (*this)[1]) - 2.0f   * ((*this)[2] * (*this)[2])),
				(2.0f * ((*this)[0] * (*this)[1]) - 2.0f * ((*this)[2] * (*this)[3])),
				(2.0f * ((*this)[0] * (*this)[2]) + 2.0f * ((*this)[1] * (*this)[3])),
				(2.0f * ((*this)[0] * (*this)[1]) + 2.0f * ((*this)[2] * (*this)[3])),
				(1.0f - 2.0f * ((*this)[0] * (*this)[0]) - 2.0f   * ((*this)[2] * (*this)[2])),
				(2.0f * ((*this)[1] * (*this)[2]) - 2.0f * ((*this)[0] * (*this)[3])),
				(2.0f * ((*this)[0] * (*this)[2]) - 2.0f * ((*this)[1] * (*this)[3])),
				(2.0f * ((*this)[1] * (*this)[2]) + 2.0f * ((*this)[0] * (*this)[3])),
				(1.0f - 2.0f * ((*this)[0] * (*this)[0]) - 2.0f   * ((*this)[1] * (*this)[1]))
			};
		}

		constexpr tmat4<value_type> as_mat4() const
		{
			const auto temp{ as_mat3() };
			return tmat4<value_type> {
				temp[0],	temp[1],	temp[2],	0.0f,
				temp[3],	temp[4],	temp[5],	0.0f,
				temp[6],	temp[7],	temp[8],	0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type angleAxis(value_type angle, complex_type const & axis)
		{
			const auto half_angle { angle * 0.5f };
			const auto temp { gcem::sin(half_angle) };
			return self_type {
				axis[0] * temp,
				axis[1] * temp,
				axis[2] * temp,
				gcem::cos(half_angle)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr operator base_type &()
		{
			return static_cast<base_type &>(*this);
		}
		
		constexpr operator base_type const &() const
		{
			return static_cast<base_type const &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING_X tquat = Quaternion<X>;
	ML_USING quat = tquat<float32_t>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_QUATERNION_HPP_