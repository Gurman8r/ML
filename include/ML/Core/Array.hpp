#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/StandardLib.hpp>
#include <ML/Core/IComparable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem,
		size_t _Size
	> class Array
		: public std::array<_Elem, _Size>
		, public IComparable<Array<_Elem, _Size>>
	{
	public:
		enum : size_t { Size = _Size };

	public:
		using self_type				= typename Array<_Elem, _Size>;
		using base_type				= typename std::array<_Elem, _Size>;
		using init_type				= typename Initializer<_Elem>;
		using value_type			= typename base_type::value_type;
		using size_type				= typename base_type::size_type;
		using difference_type		= typename base_type::difference_type;
		
		using pointer				= typename base_type::pointer;
		using const_pointer			= typename base_type::const_pointer;
		using reference				= typename base_type::reference;
		using const_reference		= typename base_type::const_reference;

		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;

	public:
		Array()
			: base_type()
		{
			this->fill((value_type)0);
		}
		Array(const value_type value)
			: base_type()
		{
			this->fill(value);
		}
		Array(const init_type & copy)
			: base_type()
		{
			assert((copy.size() == this->size()) && "Array initializer size mismatch!");
			std::copy(copy.begin(), copy.end(), this->begin());
		}
		Array(const base_type & copy)
			: base_type(copy)
		{
		}
		Array(const self_type & copy)
			: base_type((const base_type &)copy)
		{
		}
		~Array()
		{
		}

	public:
		inline operator base_type() const
		{
			return static_cast<base_type>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}

	public:
		inline bool equals(const self_type & other) const override
		{
			for (size_type i = 0; i < Size; i++)
			{
				if (this->at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}

		inline bool lessThan(const self_type & other) const override
		{
			for (size_type i = 0; i < Size; i++)
			{
				if (this->at(i) >= other.at(i))
				{
					return false;
				}
			}
			return true;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <
		class	_Elem,
		size_t	_Size
	>
	struct hash<ml::Array<_Elem, _Size>>
	{
		using argument_type = ml::Array<_Elem, _Size>;
		using result_type	= size_t;

		inline result_type operator()(const argument_type & value) const noexcept
		{
			return _Hash_array_representation(value.data(), value.size());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_
