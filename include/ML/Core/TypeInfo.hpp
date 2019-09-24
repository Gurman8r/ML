#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <ML/Core/StringView.hpp>

/* * * * * * * * * * * * * * * * * * * * */

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
# else
#	error Type information is not available.
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

	template <class T> struct nameof<T> final
	{
		constexpr nameof() = default;
		static constexpr auto value { signature<T>() };
	};

	template <> struct nameof<> final
	{
		const StringView value;

		template <class T> constexpr nameof(const nameof<T> & copy)
			: value { copy.value }
		{
		}

		template <class T> constexpr nameof(const T &) noexcept
			: nameof { nameof<T>() }
		{
		}

		template <class T> constexpr nameof(const T *) noexcept
			: nameof { nameof<const T *>() }
		{
		}

		static constexpr StringView trim_front(const StringView & value)
		{
			return ((value.size() && (value.front() == ' '))
				? trim_front(StringView { value.begin() + 1, value.size() - 1 })
				: value
			);
		}

		static constexpr StringView trim_back(const StringView & value)
		{
			return ((value.size() && (value.back() == ' '))
				? StringView { value.begin(), value.size() - 1 }
				: value
			);
		}

		static constexpr StringView trim_whitespace(const StringView & value)
		{
			return trim_front(trim_back(value));
		}

		static constexpr auto filter_prefix(const StringView & value, const StringView & pre)
		{
			return ((value.size() >= pre.size() && (value.substr(0, pre.size()) == pre))
				? value.substr(pre.size())
				: value
			);
		}

		static constexpr auto filter_suffix(const StringView & value, const StringView & suf)
		{
			return ((value.substr(value.size() - suf.size(), suf.size()) == suf)
				? value.substr(0, (value.size() - suf.size()))
				: value
			);
		}

		static constexpr auto filter_signature(const StringView & value)
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

		static constexpr auto filter(const StringView & value)
		{
			return filter_signature(value);
		}
	};


	// Type Of
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class ... T> struct typeof;

	template <class T> struct typeof<T> final
	{
		constexpr typeof() noexcept = default;
		static constexpr StringView name { nameof<>::filter(nameof<T>::value) };
		static constexpr hash_t hash { name.hash() };
	};

	template <> struct typeof<> final
	{
		const StringView name;
		const hash_t hash;

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