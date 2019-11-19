#ifndef _ML_LIST_HPP_
#define _ML_LIST_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
#ifdef ML_LIST_IMPL
	template <class T> struct List : public std::vector<T, std::allocator<T>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename T;
		using allocator_type			= typename std::allocator<T>;
		using self_type					= typename List<value_type>;
		using base_type					= typename std::vector<value_type, allocator_type>;
		using init_type					= typename std::initializer_list<value_type>;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		List()
			: base_type()
		{
		}

		explicit List(allocator_type const & alloc)
			: base_type(alloc)
		{
		}

		explicit List(const size_type count, allocator_type const & alloc = allocator_type())
			: base_type(count, alloc)
		{
		}

		List(base_type const & value, allocator_type const & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}

		List(init_type const & value, allocator_type const & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}

		List(self_type const & value, allocator_type const & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}

		template <
			class Iter
		> List(Iter begin, Iter end, allocator_type const & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}

		virtual ~List() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return !(this->empty());
		}

		inline operator base_type &()
		{
			return static_cast<base_type &>(*this);
		}

		inline operator base_type const &() const
		{
			return static_cast<base_type const &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
#endif 

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIST_HPP_