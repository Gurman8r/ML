#ifndef _ML_LIST_HPP_
#define _ML_LIST_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// just a wrapper for std::vector<>
	template <class T> struct List : public std::vector<T, Allocator<T>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type			= typename T;
		using allocator_type		= typename Allocator<T>;
		using self_type				= typename List<value_type>;
		using base_type				= typename std::vector<value_type, allocator_type>;
		using init_type				= typename Initializer<value_type>;
		using difference_type		= typename base_type::difference_type;
		using size_type				= typename base_type::size_type;
		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return !(this->empty());
		}

		inline operator base_type &()
		{
			return static_cast<base_type &>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <class T> struct hash<::ml::List<T>>
	{
		using argument_type = ::ml::List<T>;

		inline ::ml::hash_t operator()(const argument_type & value) const noexcept
		{
			return _Hash_array_representation(value.data(), value.size());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LIST_HPP_