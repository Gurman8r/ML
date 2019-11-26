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

		static constexpr auto name() noexcept -> StringView { return m_name; }
		
		static constexpr auto hash() noexcept -> hash_t { return m_hash; }

	private:
		static constexpr StringView m_name{ nameof<>::filter(nameof<T>::value) };

		static constexpr hash_t m_hash{ m_name.hash() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		constexpr typeof() noexcept
			: m_name{ "" }, m_hash{ 0 }
		{
		}

		template <class T> constexpr typeof(T const &) noexcept
			: typeof { typeof<T>{} }
		{
		}

		template <class T> constexpr typeof(T const *) noexcept
			: typeof { typeof<T const *>{} }
		{
		}

		template <class T> constexpr typeof(typeof<T> const & copy) noexcept
			: m_name{ copy.name() }, m_hash{ copy.hash() }
		{
		}

		constexpr auto name() const noexcept -> StringView { return m_name; }

		constexpr auto hash() const noexcept -> hash_t { return m_hash; }

	private: StringView m_name; hash_t m_hash;
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> inline ML_SERIALIZE(std::ostream & out, typeof<T...> const & value)
	{
		return out << value.name();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> constexpr bool operator==(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class ... T
	> constexpr bool operator!=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator<(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs.hash() < rhs.hash());
	}

	template <
		class ... T
	> constexpr bool operator>(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class ... T
	> constexpr bool operator<=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator>=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class X, class ... Y
	> constexpr bool operator==(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator!=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs.hash() < rhs.hash());
	}

	template <
		class X, class ... Y
	> constexpr bool operator>(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator>=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_OF_HPP_