#ifndef _ML_QUAT_HPP_
#define _ML_QUAT_HPP_

#include <ML/Core/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Quaternion final
		: public Vector<float, 4>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename value_type;
		using self_type				= typename Quaternion;
		using base_type				= typename Vector<value_type, 4>;
		using complex_type			= typename Vector<value_type, 3>;

		using init_type				= typename base_type::init_type;
		using array_type			= typename base_type::array_type;
		using const_value			= typename base_type::const_value;
		using pointer				= typename base_type::pointer;
		using reference				= typename base_type::reference;
		using const_pointer			= typename base_type::const_pointer;
		using const_reference		= typename base_type::const_reference;

		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Quaternion()
			: self_type(1.0f)
		{
		}
		
		Quaternion(const_reference xyz)
			: self_type(xyz, xyz, xyz)
		{
		}
		
		Quaternion(const_reference x, const_reference y, const_reference z)
			: self_type(x, y, z, 0.0f)
		{
		}
		
		Quaternion(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type({ x, y, z, w })
		{
		}
		
		Quaternion(const complex_type & xyz, const_reference w)
			: base_type({ xyz[0], xyz[1], xyz[2], w })
		{
		}

		template <
			size_t S
		> Quaternion(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Quaternion(const Vector4<T> & copy)
			: base_type(copy)
		{
		}

		template <
			class T,
			size_t S
		> Quaternion(const Vector<T, S> & copy, const_reference def = 0.0f)
			: base_type(copy, def)
		{
		}
		
		~Quaternion() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline complex_type complex() const 
		{ 
			return complex_type({ (*this)[0], (*this)[1], (*this)[2] });
		}
		
		inline const_reference real() const 
		{ 
			return this->back();
		}

		inline const_value angle() const
		{ 
			return std::acosf(this->real()) * 2.0f;
		}

		inline complex_type axis() const
		{
			const_value temp = (1.0f - this->real() * this->real());
			if (temp <= 0.0f)
			{
				return complex_type({ 0, 0, 1 });
			}
			else
			{
				return this->complex() * (1.0f / std::sqrtf(temp));
			}
		}

		inline static self_type angleAxis(const_reference angle, const complex_type & axis)
		{
			const_value s = std::sinf(angle * 0.5f);
			return self_type(
				axis[0] * s,
				axis[1] * s,
				axis[2] * s,
				std::cosf(angle * 0.5f)
			);
		}

		inline complex_type eulerAngles() const
		{
			return complex_type({
				this->pitch(),
				this->yaw(),
				this->roll()
			});
		}

		inline const_value pitch() const
		{
			return (value_type)(std::atan2f(
				(2.0f * ((*this)[1] * (*this)[2] + this->real() * (*this)[0])),
				(
					this->real() * this->real() - (*this)[0] *
					(*this)[0] - (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2])
				)
			);
		}

		inline const_value roll() const
		{
			return (value_type)(std::atan2f(
				(2.0f * ((*this)[0] * (*this)[1] + this->real() * (*this)[2])),
				(
					this->real() * this->real() + (*this)[0] * (*this)[0] -
					(*this)[1] * (*this)[1] - (*this)[2] * (*this)[2])
				)
			);
		}

		inline const_value yaw() const
		{
			return std::asinf(ML_CLAMP(
				(-2.0f * ((*this)[0] * (*this)[2] - this->real() * (*this)[1])), 
				-1.0f, 
				1.0f
			));
		}

		inline mat3 toMat3() const
		{
			// not sure if this is correct
			const_value xx = ((*this)[0] * (*this)[0]);
			const_value xy = ((*this)[0] * (*this)[1]);
			const_value xz = ((*this)[0] * (*this)[2]);
			const_value xw = ((*this)[0] * (*this)[3]);
			const_value yy = ((*this)[1] * (*this)[1]);
			const_value yz = ((*this)[1] * (*this)[2]);
			const_value yw = ((*this)[1] * (*this)[3]);
			const_value zz = ((*this)[2] * (*this)[2]);
			const_value zw = ((*this)[2] * (*this)[3]);
			mat3 m;
			m[0] = (1.0f - 2.0f * yy - 2.0f * zz);
			m[1] = (2.0f * xy - 2.0f * zw); 
			m[2] = (2.0f * xz + 2.0f * yw);
			m[3] = (2.0f * xy + 2.0f * zw);
			m[4] = (1.0f - 2.0f * xx - 2.0f * zz);
			m[5] = (2.0f * yz - 2.0f * xw);
			m[6] = (2.0f * xz - 2.0f * yw);
			m[7] = (2.0f * yz + 2.0f * xw);
			m[8] = (1.0f - 2.0f * xx - 2.0f * yy);
			return m;
		}
		
		inline mat4 toMat4() const
		{
			const mat3 r = toMat3();
			mat4 m;
			m[ 0] = r[0];	m[ 1] = r[1];	m[ 2] = r[2];	m[ 3] = 0.0f;
			m[ 4] = r[3];	m[ 5] = r[4];	m[ 6] = r[5];	m[ 7] = 0.0f;
			m[ 8] = r[6];	m[ 9] = r[7];	m[10] = r[8];	m[11] = 0.0f;
			m[12] = 0.0f;	m[13] = 0.0f;	m[14] = 0.0f;	m[15] = 1.0f;
			return m;
		}

	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator vec4() const
		{
			return vec4((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
		}

		inline friend self_type operator*(const self_type & lhs, const self_type & rhs)
		{
			return self_type(
				( lhs[0] * rhs[3]) + (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]) + (lhs[3] * rhs[0]),
				(-lhs[0] * rhs[2]) + (lhs[1] * rhs[3]) + (lhs[2] * rhs[0]) + (lhs[3] * rhs[1]),
				( lhs[0] * rhs[1]) - (lhs[1] * rhs[0]) + (lhs[2] * rhs[3]) + (lhs[3] * rhs[2]),
				(-lhs[0] * rhs[0]) - (lhs[1] * rhs[1]) - (lhs[2] * rhs[2]) + (lhs[3] * rhs[3])
			);
		}

		inline self_type & operator*=(const self_type & other)
		{
			return ((*this) = ((*this) * other));
		}


# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * */
		Quaternion(const glm::tquat<float, glm::defaultp> & value)
			: self_type(value.x, value.y, value.z, value.w)
		{
		}

		inline operator glm::tquat<float, glm::defaultp>() const
		{
			return glm::tquat<float, glm::defaultp>(
				this->real(), (*this)[0], (*this)[1], (*this)[2]
			);
		}
# endif
	};

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using quat = typename Quaternion;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_QUAT_HPP_