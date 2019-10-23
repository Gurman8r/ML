#ifndef _ML_TYPE_OF_HPP_
#define _ML_TYPE_OF_HPP_

#include <ML/Core/NameOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct typeof<T> final
	{
		constexpr typeof() noexcept = default;

		static constexpr StringView name { nameof<>::filter(nameof<T>::value) };
		
		static constexpr hash_t hash { name.hash() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		const StringView name; const hash_t hash;

		template <class T> constexpr typeof(const typeof<T> & copy) noexcept
			: name { copy.name }, hash { copy.hash }
		{
		}

		template <class T> constexpr typeof(const T &) noexcept 
			: typeof { typeof<T>{} } 
		{
		}

		template <class T> constexpr typeof(const T *) noexcept 
			: typeof { typeof<const T *>{} } 
		{
		}

		constexpr typeof() noexcept 
			: name { "" }, hash { 0 } 
		{
		}
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> inline ML_SERIALIZE(std::ostream & out, const typeof<T...> & value)
	{
		return out << value.name;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> constexpr bool operator==(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class ... T
	> constexpr bool operator!=(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator<(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return (lhs.hash < rhs.hash);
	}

	template <
		class ... T
	> constexpr bool operator>(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class ... T
	> constexpr bool operator<=(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator>=(const typeof<> & lhs, const typeof<T...> & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class X, class ... Y
	> constexpr bool operator==(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator!=(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return (lhs.hash < rhs.hash);
	}

	template <
		class X, class ... Y
	> constexpr bool operator>(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<=(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator>=(const typeof<X> & lhs, const typeof<Y...> & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_OF_HPP_