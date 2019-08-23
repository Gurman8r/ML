#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StringView.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// Source: https://github.com/Manu343726/ctti

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::StringView __cdecl ml::detail::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_GNUC)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"constexpr ml::StringView ml::detail::signature() [with T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"ml::StringView ml::detail::signature() [T = "
#	define ML_SIGNATURE_SUF	"]"
# endif

/* * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_SIGNATURE(TYPE, NAME)					\
template <> struct _ML nameof<TYPE>	final				\
{														\
	constexpr operator _ML StringView() const noexcept	\
	{													\
		return _ML StringView { NAME };					\
	}													\
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr StringView signature()
		{
			return { ML_SIGNATURE };
		}

		static constexpr StringView filter_signature(const StringView & value)
		{
#ifdef ML_CC_MSC
			const size_t lhs { value.find_first_of(0, '<') };
			const size_t rhs { value.find_last_of(value.size() - 1, '>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 1), (rhs - lhs) - 1)
				: value
			);
#else
			const size_t lhs { value.find_first_of(0, '=') };
			const size_t rhs { value.find_last_of(value.size() - 1, ']') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 2), (rhs - lhs) - 1)
				: value
			);
#endif
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> struct nameof;

	template <class T> struct nameof<T> final
	{
		constexpr operator StringView() const noexcept
		{
			return detail::filter_signature(detail::signature<T>());
		}
	};

	template <> struct nameof<> final
	{
		template <class T>
		constexpr nameof(const T &) : m_name { nameof<T>() } {}
		constexpr nameof() : m_name { "" } {}
		constexpr operator StringView() const noexcept { return { m_name }; }
	private: const StringView m_name;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> struct typeof;

	template <class T> struct typeof<T> final
	{
		constexpr typeof() noexcept : m_name { nameof<T>() } {}
		constexpr auto hash() const noexcept -> hash_t { return m_name.hash(); }
		constexpr auto name() const noexcept -> const StringView & { return m_name; }
	private: const StringView m_name;
	};

	template <> struct typeof<> final
	{
		template <class T> 
		constexpr typeof(const T &) noexcept : m_name { nameof<T>() } {}
		constexpr auto hash() const noexcept -> hash_t { return m_name.hash(); }
		constexpr auto name() const noexcept -> const StringView & { return m_name; }
	private: const StringView m_name;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> inline ML_SERIALIZE(Ostream & out, const typeof<T...> & value)
	{
		return out << value.name();
	}

	template <
		class X, class ... Y
	> constexpr bool operator==(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return (lhs.hash() == rhs.hash());
	}

	template <
		class X, class ... Y
	> constexpr bool operator!=(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return (lhs.hash() < rhs.hash());
	}

	template <
		class X, class ... Y
	> constexpr bool operator>(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return !(lhs < rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<=(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator>=(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

ML_GEN_SIGNATURE(std::string, "std::string");

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_TYPE_INFO_HPP_