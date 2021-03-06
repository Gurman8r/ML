#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/name_filters.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Signature.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct nameof<T> final
	{
		nameof() = delete;

		static constexpr StringView value { signature::type<T>() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct nameof<> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		nameof() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr StringView trim_front(StringView const & value) noexcept
		{
			return ((!value.empty() && (value.front() == ' ' || value.front() == '\t'))
				? trim_front(value.substr(1))
				: value
			);
		}

		static constexpr StringView trim_back(StringView const & value) noexcept
		{
			return ((value.size() && (value.back() == ' ' || value.back() == '\t'))
				? trim_back(value.substr(0, value.size() - 1))
				: value
			);
		}

		static constexpr StringView trim(StringView const & value) noexcept
		{
			return trim_front(trim_back(value));
		}

		static constexpr StringView filter_prefix(StringView const & value, StringView const & pre) noexcept
		{
			return ((value.size() >= pre.size() && (value.substr(0, pre.size()) == pre))
				? value.substr(pre.size())
				: value
			);
		}

		static constexpr StringView filter_suffix(StringView const & value, StringView const & suf) noexcept
		{
			return ((value.size() >= suf.size()) && ((value.substr(value.size() - suf.size(), suf.size()) == suf))
				? value.substr(0, (value.size() - suf.size()))
				: value
			);
		}

		static constexpr StringView filter_signature_type(StringView const & value) noexcept
		{
#if defined(ML_HAS_CONSTEXPR_17)
			return filter_suffix(filter_prefix(value, 
				std::get<0>(signature::detail::type)),
				std::get<1>(signature::detail::type)
			);
#elif defined(ML_CC_MSC)
			const auto lhs{ value.find_first_of('<') };
			const auto rhs{ value.find_last_of('>') };
			return (((lhs != StringView::npos) && (rhs != StringView::npos))
				? value.substr((lhs + 1), (rhs - lhs) - 1)
				: value
			);
#else
			const auto lhs{ value.find_first_of('=') };
			const auto rhs{ value.find_last_of(']') };
			return (((lhs != StringView::npos) && (rhs != StringView::npos))
				? value.substr((lhs + 2), (rhs - lhs) - 1)
				: value
			);
#endif
		}

		static constexpr StringView filter_namespace(StringView const & value) noexcept
		{
			return value.substr(value.find_first_of(':') + 2);
		}

		static constexpr StringView filter_struct(StringView const & value) noexcept
		{
			return filter_prefix(value, "struct ");
		}

		static constexpr StringView filter_class(StringView const & value) noexcept
		{
			return filter_prefix(value, "class ");
		}

		static constexpr StringView filter_constexpr(StringView const & value) noexcept
		{
			return filter_prefix(value, "constexpr ");
		}

		static constexpr StringView filter_template(StringView const & value) noexcept
		{
			const auto i { value.find_first_of('<') };
			return (i != StringView::npos) ? value.substr(0, i) : value;
		}

		static constexpr StringView filter_const(StringView const & value) noexcept
		{
			return filter_prefix(value, "const ");
		}

		static constexpr StringView filter(StringView const & value) noexcept
		{
			return 
				filter_constexpr(
				filter_class(
				filter_struct(
				filter_signature_type(
				value
			))));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#ifndef ML_HAS_CONSTEXPR_17
template <> struct _ML nameof<std::string> final
{
	static constexpr auto value { "std::string" };
};

template <> struct _ML :nameof<std::wstring> final
{
	static constexpr auto value { "std::wstring" };
};

template <> struct _ML nameof<std::u16string> final
{
	static constexpr auto value { "std::u16string" };
};

template <> struct _ML nameof<std::u32string> final
{
	static constexpr auto value { "std::u32string" };
};
#endif

#endif // !_ML_NAMEOF_HPP_