#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <ML/Core/StringView.hpp>

// Source: https://github.com/Manu343726/ctti
# if defined(ML_CC_MSC)
#	define ML_SIGNATURE		__FUNCSIG__
#	define ML_SIGNATURE_PRE	"struct ml::StringView __cdecl ml::signature<"
#	define ML_SIGNATURE_SUF	">(void)"
# elif defined(ML_CC_GNU)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"constexpr ml::StringView ml::signature() [with T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE		__PRETTY_FUNCTION__
#	define ML_SIGNATURE_PRE	"ml::StringView ml::signature() [T = "
#	define ML_SIGNATURE_SUF	"]"
# elif defined(ML_CC_INTEL)
#	define ML_SIGNATURE		0
#	define ML_SIGNATURE_PRE ""
#	define ML_SIGNATURE_SUF ""
# else
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Signature
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr StringView signature()
	{
		return { ML_SIGNATURE };
	}


	// Name Of
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	template <> struct nameof<> final
	{
		nameof() = delete;
		static constexpr StringView filter(const StringView & value) noexcept
		{
# if defined(ML_CC_MSC)
			const size_t lhs { value.find_first_of('<') };
			const size_t rhs { value.find_last_of('>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 1), (rhs - lhs) - 1)
				: value
			);
# else
			const size_t lhs { value.find_first_of('=') };
			const size_t rhs { value.find_last_of(']') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 2), (rhs - lhs) - 1)
				: value
			);
# endif
		}
	};

	template <class T> struct nameof<T> final
	{
		nameof() = delete;
		static constexpr StringView value { signature<T>() };
	};

	template <class T> static constexpr StringView nameof_v
	{ 
		nameof<>::filter(nameof<T>::value)
	};

	template <class T> static constexpr hash_t hashof_v
	{
		nameof_v<T>.hash()
	};


	// Type Of
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	template <class T> struct typeof<T> final
	{
		constexpr typeof() noexcept = default;
		static constexpr auto name { nameof_v<T> };
		static constexpr auto hash { hashof_v<T> };
	};

	template <> struct typeof<> final
	{
		StringView name; hash_t hash;

		template <class T> constexpr typeof(const typeof<T> & copy) noexcept
			: name { copy.name }, hash { copy.hash }
		{
		}

		template <class T> constexpr typeof(const T &) noexcept
			: typeof { typeof<T>() }
		{
		}

		template <class T> constexpr typeof(const T *) noexcept
			: typeof { typeof<const T *>() }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> inline ML_SERIALIZE(Ostream & out, const typeof<T...> & value)
	{
		return out << value.name;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> constexpr bool operator==(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return (lhs.hash == rhs.hash);
	}

	template <
		class ... T
	> constexpr bool operator!=(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator<(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return (lhs.hash < rhs.hash);
	}

	template <
		class ... T
	> constexpr bool operator>(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return !(lhs < rhs);
	}

	template <
		class ... T
	> constexpr bool operator<=(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator>=(const typeof<> & lhs, const typeof<T...> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class X, class ... Y
	> constexpr bool operator==(const typeof<X> & lhs, const typeof<Y...> & rhs)
	{
		return (lhs.hash == rhs.hash);
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
		return (lhs.hash < rhs.hash);
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

template <> struct ml::nameof<std::string>
{
	static constexpr auto value { "class std::string" };
};

template <> struct ml::nameof<std::wstring>
{
	static constexpr auto value { "class std::wstring" };
};

template <> struct ml::nameof<std::u16string>
{
	static constexpr auto value { "class std::u16string" };
};

template <> struct ml::nameof<std::u32string>
{
	static constexpr auto value { "class std::u32string" };
};

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_TYPE_INFO_HPP_