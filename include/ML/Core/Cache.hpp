#ifndef _ML_CACHE_HPP_
#define _ML_CACHE_HPP_

#include <ML/Core/INonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Tool for quickly mapping values
	// Inserts value into map if not exists then returns const ref to value
	// Should only be used when a failed map insert is inconsequential
	template <
		template <class, class> class Base, class Key, class Val
	> struct Cache : public INonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using key_type			= typename Key;
		using value_type		= typename Val;
		using hash_type			= typename hash_t;

		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		using base_type			= typename Base<hash_type, value_type>;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * */

		inline base_type & base() { return m_data; }
		
		inline const base_type & base() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * */

		inline const_pointer operator()(const hash_type id)
		{
			const_iterator it;
			return (((it = this->base().find(id)) != this->base().end())
				? &it->second
				: nullptr
			);
		}

		inline const_reference operator()(const key_type & key, const_reference value)
		{
			const hash_type id { std::hash<Key>()(key) };

			if (const_pointer temp = (*this)(id))
			{
				return (*temp);
			}
			else
			{
				return this->base().insert({ id, value }).first->second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private: base_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <
		class Key, class Val
	> using HashCache = Cache<HashMap, Key, Val>; // cache using hash map

	template <
		class Key, class Val
	> using TreeCache = Cache<Map, Key, Val>; // cache using BST

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CACHE_HPP_