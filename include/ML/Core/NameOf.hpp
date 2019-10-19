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
		static constexpr auto value { detail::signature<T>() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct nameof<> final
	{
		const StringView value;

		template <class T> constexpr nameof(const nameof<T> & copy)
			: value { copy.value }
		{
		}

		template <class T> constexpr nameof(const T &)
			: nameof { nameof<T>() }
		{
		}

		template <class T> constexpr nameof(const T *)
			: nameof { nameof<const T *>() }
		{
		}

		static constexpr StringView trim_front(const StringView & value)
		{
			return ((!value.empty() && (value.front() == ' ' || value.front() == '\t'))
				? trim_front(StringView { value.begin() + 1, value.size() - 1 })
				: value
			);
		}

		static constexpr StringView trim_back(const StringView & value)
		{
			return ((value.size() && (value.back() == ' ' || value.back() == '\t'))
				? StringView { value.begin(), value.size() - 1 }
				: value
			);
		}

		static constexpr StringView trim(const StringView & value)
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
# if defined(ML_CC_MSC)
			const auto lhs { value.find_first_of('<') };
			const auto rhs { value.find_last_of('>') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 1), (rhs - lhs) - 1)
				: value
			);
# else
			const auto lhs { value.find_first_of('=') };
			const auto rhs { value.find_last_of(']') };
			return (((lhs != value.npos) && (rhs != value.npos))
				? value.substr((lhs + 2), (rhs - lhs) - 1)
				: value
			);
# endif
		}

		static constexpr StringView filter_namespace(const StringView & value)
		{
			return value.substr(value.find_first_of(':') + 2);
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

		static constexpr StringView filter_template(const StringView & value)
		{
			const auto i { value.find_first_of('<') };
			return (i != value.npos) ? value.substr(0, i) : value;
		}

		static constexpr StringView filter_const(const StringView & value)
		{
			return filter_prefix(value, "const ");
		}

		static constexpr StringView filter(const StringView & value)
		{
			return 
				filter_constexpr(
				filter_class(
				filter_struct(
				filter_signature(
				value
			))));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_