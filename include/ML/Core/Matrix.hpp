#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Comparable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/GLM.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_ENIF_MAT(XCOND, YCOND) typename std::enable_if<(XCOND  &&  YCOND)>::type
#define ML_ENIF_3x3(X, Y) ML_ENIF_MAT(X == 3, Y == 3)
#define ML_ENIF_4x4(X, Y) ML_ENIF_MAT(X == 4, Y == 4)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Declarations
	/* * * * * * * * * * * * * * * * * * * * */

	template <class E, size_t N>	class Vector;
	template <class E>				class Vector2;
	template <class E>				class Vector3;
	template <class E>				class Vector4;

	/* * * * * * * * * * * * * * * * * * * * */
	
	template <
		class	_Elem, 
		size_t	_Cols, 
		size_t	_Rows
	> class Matrix
		: public I_Newable
		, public I_Comparable<Matrix<_Elem, _Cols, _Rows>>
	{
	public: // Enums
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t
		{
			Cols = _Cols,
			Rows = _Rows,
			Size = (Cols * Rows)
		};


	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using array_type			= typename value_type[Size];
		using const_value			= typename const value_type;

		using pointer				= typename value_type *;
		using reference				= typename value_type &;
		using const_pointer			= typename const value_type *;
		using const_reference		= typename const value_type &;

		using self_type				= typename Matrix<value_type, Cols, Rows>;
		using init_type				= typename Initializer<value_type>;
		using contiguous_type		= typename List<value_type>;

		using iterator				= typename std::_Array_iterator<value_type, Size>;
		using const_iterator		= typename std::_Array_const_iterator<value_type, Size>;
		using reverse_iterator		= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator= typename std::reverse_iterator<const_iterator>;


	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		array_type m_data;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Matrix()
			: m_data()
		{
			std::fill(this->begin(), this->end(), static_cast<value_type>(0));
		}

		Matrix(const_reference value)
			: m_data()
		{
			std::fill(this->begin(), this->end(), value);
		}

		Matrix(const init_type & value)
			: m_data()
		{
			assert((value.size() == this->size()) && "Matrix initializer size mismatch");
			std::copy(value.begin(), value.end(), this->begin());
		}

		Matrix(const self_type & value)
			: m_data()
		{
			std::copy(value.begin(), value.end(), this->begin());
		}

		template <
			class	T,
			size_t	X,
			size_t	Y
		> Matrix(const Matrix<T, X, Y> & value, const_reference def = static_cast<value_type>(0))
			: m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = ((i < value.size())
					? (static_cast<value_type>(value[i]))
					: (def)
				);
			}
		}

		virtual ~Matrix() {}


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline const_reference operator[](const size_t index) const
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline reference operator[](const size_t index)
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline self_type & operator=(const_reference value)
		{
			return ((*this) = self_type(value));
		}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline size_t			cols()	const	{ return Cols;				}
		inline size_t			rows()	const	{ return Rows;				}
		inline size_t			size()	const	{ return Size;				}

		inline reference		back()			{ return (*this)[Size - 1];	}
		inline pointer			data()			{ return &(*this)[0];		}
		inline reference		front()			{ return (*this)[0];		}

		inline const_reference	back()	const	{ return (*this)[Size - 1];	}
		inline const_pointer	data()	const	{ return &(*this)[0];		}
		inline const_reference	front()	const	{ return (*this)[0];		}

		/* * * * * * * * * * * * * * * * * * * * */


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static const contiguous_type & Contiguous(const self_type * value, const size_t length)
		{
			static contiguous_type temp;
			if (value)
			{
				if (const size_t imax = (length * Size))
				{
					if (temp.size() != imax)
					{
						temp.resize(imax);
					}
					for (size_t i = 0; i < imax; i++)
					{
						temp[i] = value[i / Size][i % Size];
					}
				}
				else if (!temp.empty())
				{
					temp.clear();
				}
			}
			return temp;
		}

		inline static const self_type & Identity()
		{
			static self_type temp;
			static bool check = true;
			if (check)
			{	check = false;
				for (size_t y = 0; y < Rows; y++)
				{
					for (size_t x = 0; x < Cols; x++)
					{
						temp[y * Cols + x] = ((x == y)
							? static_cast<value_type>(1)
							: static_cast<value_type>(0)
						);
					}
				}
			}
			return temp;
		}


	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend ML_SERIALIZE(ostream & out, const self_type & value)
		{
			for (const auto & e : value)
			{
				out << e << " ";
			}
			return out;
		}

		inline friend ML_DESERIALIZE(istream & in, self_type & value)
		{
			for (size_t i = 0; i < value.size(); i++)
			{
				in >> value[i];
			}
			return in;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool equals(const self_type & value) const override
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

		inline bool lessThan(const self_type & value) const override
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
		

	public: // Iterators
		/* * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()			{ return iterator(m_data, 0); }
		inline const_iterator	begin() const	{ return const_iterator(m_data, 0); }
		inline const_iterator	cbegin() const	{ return begin(); }
		inline iterator			end()			{ return iterator(m_data, this->size()); }
		inline const_iterator	end() const		{ return const_iterator(m_data, this->size()); }
		inline const_iterator	cend() const	{ return end(); }


# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// From glm::mat3
		template <
			size_t X = Cols,
			size_t Y = Rows
		> Matrix(const glm::tmat3x3<value_type, glm::defaultp> & value, ML_ENIF_3x3(X, Y) * = 0)
			: m_data()
		{
			if (const_pointer temp = glm::value_ptr(value))
			{
				std::copy(&temp[0], &temp[this->size()], this->begin());
			}
		}

		// To glm::mat3
		template <
			size_t X = Cols,
			size_t Y = Rows
		> inline operator glm::tmat3x3<value_type, glm::defaultp>() const
		{
			static_assert((X == 3 && Y == 3),
				"Size mismatch, unable to convert matrix to glm::mat3"
			);
			return glm::tmat3x3<value_type, glm::defaultp>(
				(*this)[0], (*this)[1], (*this)[2],
				(*this)[3], (*this)[4], (*this)[5],
				(*this)[6], (*this)[7], (*this)[8] 
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		// From glm::mat4
		template <
			size_t X = Cols,
			size_t Y = Rows
		> Matrix(const glm::tmat4x4<value_type, glm::defaultp> & value, ML_ENIF_4x4(X, Y) * = 0)
			: m_data()
		{
			if (const_pointer temp = glm::value_ptr(value))
			{
				std::copy(&temp[0], &temp[this->size()], this->begin());
			}
		}

		// To glm::mat4
		template <
			size_t X = Cols,
			size_t Y = Rows
		> inline operator glm::tmat4x4<value_type, glm::defaultp>() const
		{
			static_assert((X == 4 && Y == 4),
				"Size mismatch, unable to convert matrix to glm::mat4"
			);
			return glm::tmat4x4<value_type, glm::defaultp>(
				(*this)[ 0], (*this)[ 1], (*this)[ 2], (*this)[ 3],
				(*this)[ 4], (*this)[ 5], (*this)[ 6], (*this)[ 7],
				(*this)[ 8], (*this)[ 9], (*this)[10], (*this)[11],
				(*this)[12], (*this)[13], (*this)[14], (*this)[15]
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# endif

	};

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using mat3f = Matrix<float, 3, 3>;
	using mat4f = Matrix<float, 4, 4>;
	using mat3	= typename mat3f;
	using mat4	= typename mat4f;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_