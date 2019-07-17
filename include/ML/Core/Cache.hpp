#ifndef _ML_CACHE_HPP_
#define _ML_CACHE_HPP_

#include <ML/Core/Hash.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Tool for quickly mapping values
	// Inserts value into map if not exists then returns const ref to value
	// Should only be used when a failed map insert is inconsequential
	// NOT TO BE USED IN PLACE OF Map/HashMap for general use
	template <
		template <class, class> class Base, 
		class Key, class Val
	> struct Cache final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using key_type			= typename Key;
		using value_type		= typename Val;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using base_type			= typename Base<hash_t, value_type>;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * */

		inline base_type & base() { return m_data; }
		
		inline const base_type & base() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * */

		inline const_pointer operator()(const hash_t id)
		{
			const_iterator it;
			return (((it = this->base().find(id)) != this->base().end())
				? &it->second
				: nullptr
			);
		}

		inline const_reference operator()(const key_type & key, const_reference value)
		{
			const hash_t id { Hash()(key) };

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
	> using TreeCache = Cache<Map, Key, Val>; // cache using b-tree

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CACHE_HPP_