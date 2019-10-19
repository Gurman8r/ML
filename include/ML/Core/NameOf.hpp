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
			return filter_constexpr(filter_class(filter_struct(filter_signature(value))));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_NAMEOF_INTEGRALS
#ifdef ML_NAMEOF_INTEGRALS
template <> struct ml::nameof<ML_BOOL>		{ static constexpr auto value { ML_STRINGIFY(ML_BOOL) }; };
template <> struct ml::nameof<ML_CHAR>		{ static constexpr auto value { ML_STRINGIFY(ML_CHAR) }; };
template <> struct ml::nameof<ML_WCHAR>		{ static constexpr auto value { ML_STRINGIFY(ML_WCHAR) }; };
template <> struct ml::nameof<ML_CHAR16>	{ static constexpr auto value { ML_STRINGIFY(ML_CHAR16) }; };
template <> struct ml::nameof<ML_CHAR32>	{ static constexpr auto value { ML_STRINGIFY(ML_CHAR32) }; };
template <> struct ml::nameof<ML_INT8>		{ static constexpr auto value { ML_STRINGIFY(ML_INT8) }; };
template <> struct ml::nameof<ML_INT16>		{ static constexpr auto value { ML_STRINGIFY(ML_INT16) }; };
template <> struct ml::nameof<ML_INT32>		{ static constexpr auto value { ML_STRINGIFY(ML_INT32) }; };
template <> struct ml::nameof<ML_INT64>		{ static constexpr auto value { ML_STRINGIFY(ML_INT64) }; };
template <> struct ml::nameof<ML_UINT8>		{ static constexpr auto value { ML_STRINGIFY(ML_UINT8) }; };
template <> struct ml::nameof<ML_UINT16>	{ static constexpr auto value { ML_STRINGIFY(ML_UINT16) }; };
template <> struct ml::nameof<ML_UINT32>	{ static constexpr auto value { ML_STRINGIFY(ML_UINT32) }; };
template <> struct ml::nameof<ML_UINT64>	{ static constexpr auto value { ML_STRINGIFY(ML_UINT64) }; };
template <> struct ml::nameof<ML_FLOAT32>	{ static constexpr auto value { ML_STRINGIFY(ML_FLOAT32) }; };
template <> struct ml::nameof<ML_FLOAT64>	{ static constexpr auto value { ML_STRINGIFY(ML_FLOAT64) }; };
template <> struct ml::nameof<ML_FLOAT80>	{ static constexpr auto value { ML_STRINGIFY(ML_FLOAT80) }; };
#endif // ML_NAMEOF_INTEGRALS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_NAMEOF_STRINGS
#ifdef ML_NAMEOF_STRINGS
template <> struct ml::nameof<std::string>		{ static constexpr auto value { ML_STRINGIFY(std::string) }; };
template <> struct ml::nameof<std::wstring>		{ static constexpr auto value { ML_STRINGIFY(std::wstring) }; };
template <> struct ml::nameof<std::u16string>	{ static constexpr auto value { ML_STRINGIFY(std::u16string) }; };
template <> struct ml::nameof<std::u32string>	{ static constexpr auto value { ML_STRINGIFY(std::u32string) }; };
#endif // ML_NAMEOF_STRING

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NAMEOF_HPP_