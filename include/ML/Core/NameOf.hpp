#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

#include <ML/Core/Signature.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct nameof<T> final
	{
		constexpr nameof() = default;
		static constexpr auto value { signature<T>() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			return ((!value.empty() && value.front() == ' ' || value.front() == '\t')
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

		static constexpr StringView filter_prefix(const StringView & value, const StringView & pre)
		{
			return ((value.size() >= pre.size() && (value.substr(0, pre.size()) == pre))
				? value.substr(pre.size())
				: value
			);
		}

		static constexpr StringView filter_suffix(const StringView & value, const StringView & suf)
		{
			return ((value.substr(value.size() - suf.size(), suf.size()) == suf)
				? value.substr(0, (value.size() - suf.size()))
				: value
			);
		}

		static constexpr StringView filter_signature(const StringView & value)
		{
# if defined(ML_CC_MICROSOFT)
			const auto lhs { value.find_first_of('<') };
			const auto rhs { value.find_last_of('>') };
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

		static constexpr StringView filter_struct(const StringView & value)
		{
			return filter_prefix(value, "struct ");
		}

		static constexpr StringView filter_class(const StringView & value)
		{
			return filter_prefix(value, "class ");
		}

		static constexpr StringView filter_constexpr(const StringView & value)
		{
			return filter_prefix(value, "constexpr ");
		}

		static constexpr StringView filter(const StringView & value)
		{
			return filter_constexpr(filter_class(filter_struct(filter_signature(value))));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr auto nameof_v { nameof<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NAMEOF_HPP_