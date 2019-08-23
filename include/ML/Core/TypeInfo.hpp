#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/C_String.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::CX_String __cdecl ml::detail::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	""
#	define ML_SIGNATURE_SUF	""
# elif defined(ML_CC_GNUC)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	""
#	define ML_SIGNATURE_SUF	""
# endif

/* * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_SIGNATURE(TYPE, NAME)						\
template <> struct _ML nameof<TYPE>							\
{															\
	constexpr nameof() noexcept = default;					\
	constexpr operator _ML CX_String() const noexcept		\
	{														\
		return { ML_SIGNATURE_PRE NAME ML_SIGNATURE_SUF };	\
	}														\
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr CX_String signature()
		{
			return { ML_SIGNATURE };
		}

		static constexpr CX_String filter_name(const CX_String & value)
		{
			const size_t lhs { value.find_first_of(0, '<') };
			const size_t rhs { value.find_last_of(value.size() - 1, '>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? CX_String { (value.begin() + lhs + 1), (rhs - lhs) - 1 }
				: value
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct nameof<T> final
	{
		constexpr nameof() noexcept = default;

		constexpr operator CX_String() const noexcept
		{
			return detail::signature<T>(); 
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct typeof<T>
	{
		constexpr typeof() noexcept = default;

		constexpr auto hash_code()	const noexcept -> hash_t	{ return raw_name().hash(); }
		constexpr auto name()		const noexcept -> CX_String	{ return detail::filter_name(raw_name()); }
		constexpr auto raw_name()	const noexcept -> CX_String	{ return nameof<T>(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<>
	{
		template <class T> constexpr typeof(const T &) noexcept
			: typeof<> { typeof<T>() }
		{
		}

		template <class T> explicit constexpr typeof(const typeof<T> & copy) noexcept
			: m_raw  { copy.raw_name() }
			, m_name { copy.name() }
			, m_hash { copy.hash_code() }
		{
		}

		constexpr auto hash_code()	const noexcept -> hash_t	{ return m_hash; }
		constexpr auto name()		const noexcept -> CX_String	{ return m_name; }
		constexpr auto raw_name()	const noexcept -> CX_String	{ return m_raw; }

	private: 
		const hash_t	m_hash;
		const CX_String	m_name;
		const CX_String	m_raw;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_INFO_HPP_