#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <ML/Core/ITrackable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Maths.hpp>

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
		: public ITrackable
		, public IComparable<Matrix<_Elem, _Cols, _Rows>>
	{
		static_assert(std::is_trivial<_Elem>::value, 
			"Matricies must contain trivial values."
		);


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
		using init_type				= typename InitList<value_type>;
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

		inline const_reference get(const size_t i) const
		{
			return (*this)[i];
		}

		inline self_type & set(const size_t i, const_reference value)
		{
			((*this)[i] = value); return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> inline const_reference get(const size_t x, const size_t y) const
		{
			return this->get(y * Cols + x);
		}

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> inline self_type & set(const size_t x, const size_t y, const_reference value)
		{
			return this->set(y * Cols + x, value);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> constexpr self_type & setCol(const size_t c, const Vector<value_type, Cols> & value)
		{

			return (*this)
				.set(c, 0, value[0])
				.set(c, 1, value[1])
				.set(c, 2, value[2])
				.set(c, 3, value[3]);
		}

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> constexpr self_type & setRow(const size_t r, const Vector<value_type, Rows> & value)
		{
			return (*this)
				.set(0, r, value[0])
				.set(1, r, value[1])
				.set(2, r, value[2])
				.set(3, r, value[3]);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> inline Vector<value_type, Cols> getCol(const size_t c)
		{
			return Vector<value_type, Rows> {
				this->get(c, 0),
				this->get(c, 1),
				this->get(c, 2),
				this->get(c, 3)
			};
		}

		template <
			size_t X = Cols, size_t Y = Rows, typename = ML_ENIF_MAT(X > 1, Y > 1)
		> inline Vector<value_type, Rows> getRow(const size_t r)
		{
			return Vector<value_type, Rows> {
				this->get(0, r),
				this->get(1, r),
				this->get(2, r),
				this->get(3, r)
			};
		}


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
		inline virtual void serialize(std::ostream & out) const override
		{
			for (size_t y = 0; y < this->rows(); y++)
			{
				for (size_t x = 0; x < this->cols(); x++)
				{
					out << (*this)[y * this->cols() + x] << " ";
				}
				out << endl;
			}
			out << endl;
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				if (in.good())
				{
					in >> (*this)[i];
				}
				else
				{
					(*this)[i] = static_cast<value_type>(0);
				}
			}
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
		inline iterator					begin()				{ return iterator(m_data, 0); }
		inline iterator					end()				{ return iterator(m_data, this->size()); }
		inline const_iterator			begin()		const	{ return const_iterator(m_data, 0); }
		inline const_iterator			end()		const	{ return const_iterator(m_data, this->size()); }
		inline const_iterator			cbegin()	const	{ return begin(); }
		inline const_iterator			cend()		const	{ return end(); }
		inline reverse_iterator			rbegin()			{ return reverse_iterator(end()); }
		inline reverse_iterator			rend()				{ return reverse_iterator(begin()); }
		inline const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crbegin()	const	{ return rbegin(); }
		inline const_reverse_iterator	crend()		const	{ return rend(); }


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