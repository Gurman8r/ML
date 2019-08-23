#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/C_String.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// Source: https://github.com/Manu343726/ctti

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::CX_String __cdecl ml::detail::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_GNUC)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"constexpr ml::CX_String ml::detail::signature() [with T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"ml::CX_String ml::detail::signature() [T = "
#	define ML_SIGNATURE_SUF	"]"
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

		static constexpr CX_String filter_signature(const CX_String & value)
		{
#ifdef ML_CC_MSC
			const size_t lhs { value.find_first_of(0, '<') };
			const size_t rhs { value.find_last_of(value.size() - 1, '>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? CX_String { (value.begin() + lhs + 1), (rhs - lhs) - 1 }
				: value
			);
#else
			const size_t lhs { value.find_first_of(0, '=') };
			const size_t rhs { value.find_last_of(value.size() - 1, ']') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? CX_String { (value.begin() + lhs + 2), (rhs - lhs) - 1 }
				: value
			);
#endif
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

	namespace detail
	{
		struct type_info final
		{
			const CX_String raw;
			const hash_t	hash;
			const CX_String name;

			constexpr type_info(const CX_String & str) noexcept
				: raw	{ str }
				, hash	{ str.hash() }
				, name	{ filter_signature(str) }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct typeof<T>
	{
		constexpr typeof() noexcept 
			: m_info { nameof<T>() } 
		{
		}

		constexpr auto hash_code()	const noexcept -> const hash_t &	{ return m_info.hash; }
		constexpr auto name()		const noexcept -> const CX_String &	{ return m_info.name; }
		constexpr auto raw_name()	const noexcept -> const CX_String &	{ return m_info.raw; }

	private: const detail::type_info m_info;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<>
	{
		template <class T> constexpr typeof(const T &) noexcept 
			: m_info { nameof<T>() }
		{
		}

		constexpr auto hash_code()	const noexcept -> const hash_t &	{ return m_info.hash; }
		constexpr auto name()		const noexcept -> const CX_String &	{ return m_info.name; }
		constexpr auto raw_name()	const noexcept -> const CX_String &	{ return m_info.raw; }

	private: const detail::type_info m_info;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_INFO_HPP_