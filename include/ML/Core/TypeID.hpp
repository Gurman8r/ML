#ifndef _ML_TYPE_ID_HPP_
#define _ML_TYPE_ID_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/type_id.hpp

#include <ML/Core/NameOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct type_id_t
	{
		constexpr type_id_t(const XString & name) 
			: m_name { name }
		{
		}

		constexpr type_id_t() 
			: type_id_t { "void" }
		{
		}

		type_id_t & operator=(const type_id_t &) = default;

		constexpr hash_t hash() const
		{
			return m_name.hash();
		}

		constexpr const XString & name() const
		{
			return m_name;
		}

		friend constexpr bool operator==(const type_id_t & lhs, const type_id_t & rhs)
		{
			return lhs.hash() == rhs.hash();
		}

		friend constexpr bool operator!=(const type_id_t & lhs, const type_id_t & rhs)
		{
			return !(lhs == rhs);
		}

	private:
		XString m_name;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct unnamed_type_id_t
	{
		constexpr unnamed_type_id_t(const hash_t hash) 
			: m_hash { hash }
		{
		}

		constexpr unnamed_type_id_t(const type_id_t & id) 
			: m_hash { id.hash() }
		{
		}

		unnamed_type_id_t & operator=(const unnamed_type_id_t &) = default;

		constexpr hash_t hash() const
		{
			return m_hash;
		}

		friend constexpr bool operator==(const unnamed_type_id_t & lhs, const unnamed_type_id_t & rhs)
		{
			return lhs.hash() == rhs.hash();
		}

		friend constexpr bool operator!=(const unnamed_type_id_t & lhs, const unnamed_type_id_t & rhs)
		{
			return !(lhs == rhs);
		}

	private:
		hash_t m_hash;
	};

#ifdef ML_TYPE_INDEX
	using type_index = unnamed_type_id_t; // To mimic std::type_index when using maps
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<size_t N>
	constexpr unnamed_type_id_t id_from_name(const char(&typeName)[N])
	{
		return hash::fnv::do_hash(typeName);
	}

	constexpr unnamed_type_id_t id_from_name(const char* typeName, size_t length)
	{
		return hash::fnv::do_hash(length, typeName);
	}

	constexpr unnamed_type_id_t id_from_name(const XString & name)
	{
		return hash::fnv::do_hash(name.size(), name.begin());
	}

	inline unnamed_type_id_t id_from_name(const String & typeName)
	{
		// Inline to prevent ODR violation
		return hash::fnv::do_hash(typeName.size(), typeName.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template<typename T>
		constexpr type_id_t type_id()
		{
			return { nameof<T>() };
		}

		template<typename T>
		constexpr unnamed_type_id_t unnamed_type_id()
		{
			return { id_from_name(nameof<T>()) };
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/**
	* Returns type information at compile-time for a value
	* of type T. Decay is applied to argument type first, use
	* type_id<decltype(arg)>() to preserve references and cv qualifiers.
	*/
	template<typename T>
	constexpr type_id_t type_id(T &&)
	{
		return detail::type_id<typename std::decay<T>::type>();
	}

	/**
	* Returns type information at compile-time for type T.
	*/
	template<typename T>
	constexpr type_id_t type_id()
	{
		return detail::type_id<T>();
	}

	/**
	* Returns unnamed type information at compile-time for a value
	* of type T. Decay is applied to argument type first, use
	* type_id<decltype(arg)>() to preserve references and cv qualifiers.
	*/
	template<typename T>
	constexpr unnamed_type_id_t unnamed_type_id(T &&)
	{
		return detail::unnamed_type_id<typename std::decay<T>::type>();
	}

	/**
	* Returns unnamed type information at compile-time for type T.
	*/
	template<typename T>
	constexpr unnamed_type_id_t unnamed_type_id()
	{
		return detail::unnamed_type_id<T>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_ID_HPP_