#ifndef _ML_ITERATOR_HPP_
#define _ML_ITERATOR_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>
#include <ML/Core/IComparable.hpp>

// https://codereview.stackexchange.com/a/74647/86601

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Category,
		class _Value, 
		class _Unqualified,
		class _Difference
	> class Iterator
		: public std::iterator<_Category, _Unqualified, _Difference, _Value *, _Value &>
		, public IComparable<Iterator<_Category, _Value, _Unqualified, _Difference>>
	{
	public:
		using iterator_category = typename _Category;
		using value_type		= typename _Value;
		using unqualified_type	= typename _Unqualified;
		using difference_type	= typename _Difference;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;
		using self_type			= typename Iterator<iterator_category, value_type, unqualified_type, difference_type>;
		using const_self_type	= typename Iterator<iterator_category, const value_type, unqualified_type, difference_type>;

	public:
		Iterator()
			: m_handle(NULL)
		{
		}
		
		Iterator(const self_type & copy)
			: m_handle(copy.m_handle)
		{
		}
		
		explicit Iterator(unqualified_type * ptr)
			: m_handle(ptr)
		{
		}
		
		virtual ~Iterator() {}

	public:
		virtual bool equals(const self_type & value) const override
		{
			return m_handle == value.m_handle;
		}
		
		virtual bool lessThan(const self_type & value) const override
		{
			return m_handle < value.m_handle;
		}

	public:
		inline operator const_self_type() const
		{
			return Iterator<iterator_category, const value_type>(m_handle);
		}


		inline reference operator*() const
		{
			assert((m_handle != NULL) && "Invalid iterator dereference!");
			return (*m_handle);
		}
		
		inline reference operator->() const
		{
			assert((m_handle != NULL) && "Invalid iterator dereference!");
			return (*m_handle);
		}


		inline self_type & operator++()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(++m_handle);
			return (*this);
		}
		
		inline self_type & operator--()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(--m_handle);
			return (*this);
		}
		

		inline self_type operator++(int32_t)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator++();
			return tmp;
		}
		
		inline self_type operator--(int32_t)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator--();
			return tmp;
		}
		

		inline self_type operator+(const difference_type & value) const
		{
			return self_type(m_handle + value);
		}
		
		inline self_type operator-(const difference_type & value) const
		{
			return self_type(m_handle - value);
		}
		

		inline self_type & operator+=(const difference_type & value)
		{
			return (*this) = (*this) + value;
		}
		
		inline self_type & operator-=(const difference_type & value)
		{
			return (*this) = (*this) - value;
		}
		

		inline const difference_type operator+(const self_type & other) const
		{
			return (m_handle + other.m_handle);
		}
		
		inline const difference_type operator-(const self_type & other) const
		{
			return (m_handle - other.m_handle);
		}

	private:
		unqualified_type * m_handle;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Value,
		class _Unqualified = std::remove_cv_t<_Value>,
		class _Difference = std::ptrdiff_t
	>
	using ForwardIterator = Iterator<std::forward_iterator_tag, _Value, _Unqualified, _Difference>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ITERATOR_HPP_
