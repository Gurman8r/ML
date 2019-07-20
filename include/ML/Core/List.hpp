#ifndef _ML_LIST_HPP_
#define _ML_LIST_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// just a wrapper for std::vector<>
	template <
		class _Elem,
		class _Alloc = Allocator<_Elem>
	> struct List : public std::vector<_Elem, _Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type			= typename _Elem;
		using allocator_type		= typename _Alloc;
		using self_type				= typename List<value_type, allocator_type>;
		using base_type				= typename std::vector<value_type, allocator_type>;
		using init_type				= typename std::initializer_list<value_type>;
		using difference_type		= typename base_type::difference_type;
		using size_type				= typename base_type::size_type;
		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		List()
			: base_type()
		{
		}

		explicit List(const allocator_type & alloc)
			: base_type(alloc)
		{
		}
		
		explicit List(const size_type count, const allocator_type & alloc = allocator_type())
			: base_type(count, alloc)
		{
		}

		List(const base_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		List(const init_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		List(const self_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		template <
			class Iter
		> List(Iter begin, Iter end, const allocator_type & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}
		
		virtual ~List() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return !(this->empty());
		}

		inline operator base_type() const
		{
			return static_cast<base_type>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIST_HPP_