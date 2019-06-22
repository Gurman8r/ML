#ifndef _ML_VECTOR_HPP_
#define _ML_VECTOR_HPP_

#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class	_Elem, 
		size_t	_Size
	> class Vector
		: public Matrix<_Elem, _Size, 1>
		, public I_Comparable<Vector<_Elem, _Size>>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using self_type				= typename Vector<value_type, _Size>;
		using base_type				= typename Matrix<value_type, _Size, 1>;

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
		Vector()
			: base_type()
		{
		}
		
		Vector(const_reference value)
			: base_type(value)
		{
		}
		
		Vector(const init_type & value)
			: base_type(value)
		{
		}
		
		Vector(const self_type & value)
			: base_type(value)
		{
		}

		template <
			size_t S
		> Vector(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Vector(const Vector<T, self_type::Size> & copy)
			: base_type(copy)
		{
		}

		template <
			class T, 
			size_t S
		> Vector(const Vector<T, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector() {}


	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual bool equals(const self_type & value) const override
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		
		inline virtual bool lessThan(const self_type & value) const override
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline value_type distanceTo(const self_type & value) const
		{
			return self_type::Distance((*this), value);
		}

		inline value_type dot(const self_type & value) const
		{
			return self_type::Dot((*this), value);
		}
		
		inline self_type & normalize()
		{
			return ((*this) /= magnitude());
		};

		inline self_type normalized() const
		{
			return self_type(*this).normalize();
		};
		
		inline value_type magnitude() const
		{
			return std::sqrt(sqrMagnitude());
		};

		inline value_type sqrMagnitude() const
		{
			value_type value = 0;
			for (size_t i = 0; i < this->size(); i++)
			{
				value += (*this)[i] * (*this)[i];
			}
			return value;
		};


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static value_type Dot(const self_type & a, const self_type & b)
		{
			value_type value = static_cast<value_type>(0);
			for (size_t i = 0; i < self_type::Size; i++)
			{
				value += static_cast<value_type>(a[i]) * static_cast<value_type>(b[i]);
			}
			return value;
		};

		inline static self_type Direction(const self_type & from, const self_type & to)
		{
			return (to - from).normalized();
		};

		inline static value_type Distance(const self_type & a, const self_type & b)
		{
			return self_type(a - b).magnitude();
		};

		inline static self_type Lerp(const self_type & a, const self_type & b, value_type t)
		{
			return (a * t + b * (static_cast<value_type>(1) - t));
		};

		inline static self_type Reflect(const self_type & direction, const self_type & normal)
		{
			return 
				(normal - direction) * 
				(static_cast<value_type>(2) * self_type::Dot(direction, normal));
		};


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend self_type operator-(const self_type & rhs)
		{
			return (rhs * static_cast<value_type>(-1));
		}

		inline friend self_type operator*(const self_type & lhs, const_reference rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] * rhs;
			}
			return temp;
		};

		inline friend self_type operator/(const self_type & lhs, const_reference rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] / rhs;
			}
			return temp;
		};

		inline friend self_type & operator*=(self_type & lhs, const_reference rhs)
		{
			return (lhs = (lhs * rhs));
		};

		inline friend self_type & operator/=(self_type & lhs, const_reference rhs)
		{
			return (lhs = (lhs / rhs));
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T> 
		inline friend self_type operator+(const self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] + static_cast<value_type>(rhs[i]);
			}
			return temp;
		};
		
		template <class T>
		inline friend self_type operator-(const self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] - static_cast<value_type>(rhs[i]);
			}
			return temp;
		};
		
		template <class T>
		inline friend self_type operator*(const self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] * static_cast<value_type>(rhs[i]);
			}
			return temp;
		};
		
		template <class T>
		inline friend self_type operator/(const self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			self_type temp;
			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				temp[i] = lhs[i] / static_cast<value_type>(rhs[i]);
			}
			return temp;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		inline friend self_type & operator+=(self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			return (lhs = (lhs + rhs));
		};
		
		template <class T>
		inline friend self_type & operator-=(self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			return (lhs = (lhs - rhs));
		};
		
		template <class T>
		inline friend self_type & operator*=(self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		template <class T>
		inline friend self_type & operator/=(self_type & lhs, const Vector<T, self_type::Size> & rhs)
		{
			return (lhs = (lhs / rhs));
		};

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VECTOR_HPP_