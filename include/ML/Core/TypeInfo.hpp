#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StringView.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// Source: https://github.com/Manu343726/ctti

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::StringView __cdecl ml::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_GNUC)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"constexpr ml::StringView ml::signature() [with T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"ml::StringView ml::signature() [T = "
#	define ML_SIGNATURE_SUF	"]"
# endif

/* * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_SIGNATURE(TYPE, NAME)						\
template <> struct ::ml::nameof<TYPE>	final				\
{															\
	constexpr nameof() = default;							\
	constexpr operator ::ml::StringView() const noexcept	\
	{														\
		return ::ml::StringView { NAME };					\
	}														\
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr StringView signature()
	{
		return { ML_SIGNATURE };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace filter
	{
		static constexpr StringView name(const StringView & value)
		{
#ifdef ML_CC_MSC
			const size_t lhs { value.find_first_of('<') };
			const size_t rhs { value.find_last_of('>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 1), (rhs - lhs) - 1)
				: value
			);
#else
			const size_t lhs { value.find_first_of('=') };
			const size_t rhs { value.find_last_of(']') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 2), (rhs - lhs) - 1)
				: value
			);
#endif
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	template <class T> struct nameof<T> final
	{
		using type = typename T;
		constexpr nameof() = default;
		constexpr operator StringView() const noexcept { return signature<type>(); }
	};

	template <> struct nameof<> final
	{
		template <class T> constexpr nameof(const nameof<T> &) noexcept : m_sig { nameof<T>() } {}
		template <class T> constexpr nameof(const T &) noexcept : m_sig { nameof<T>() } {}
		template <class T> constexpr nameof(const T *) noexcept : m_sig { nameof<const T *>() } {}
		constexpr nameof() noexcept : m_sig { nullptr } {}
		constexpr operator StringView() const noexcept { return m_sig; }
	private: const StringView m_sig;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	template <class T> struct typeof<T> final
	{
		using type = typename T;
		constexpr typeof() noexcept : m_name { nameof<T>() } {};
		constexpr auto name() const noexcept -> StringView	{ return filter::name(m_name); }
		constexpr auto hash() const noexcept -> hash_t		{ return name().hash(); }
	private: const nameof<> m_name;
	};

	template <> struct typeof<> final
	{
		template <class T> constexpr typeof(const typeof<T> &) noexcept : m_name { nameof<T>() } {}
		template <class T> constexpr typeof(const T &) noexcept : m_name { nameof<T>() } {}
		template <class T> constexpr typeof(const T *) noexcept : m_name { nameof<const T *>() } {}
		constexpr auto name() const noexcept -> StringView	{ return filter::name(m_name); }
		constexpr auto hash() const noexcept -> hash_t		{ return name().hash(); }
	private: const nameof<> m_name;
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

ML_GEN_SIGNATURE(std::string,		"class std::string");
ML_GEN_SIGNATURE(std::wstring,		"class std::wstring");
ML_GEN_SIGNATURE(std::u16string,	"class std::u16string");
ML_GEN_SIGNATURE(std::u32string,	"class std::u32string");

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_TYPE_INFO_HPP_