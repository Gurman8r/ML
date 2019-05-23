#ifndef _ML_NAME_FILTERS_HPP_
#define _ML_NAME_FILTERS_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/name_filters.hpp

#include <ML/Core/XString.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		constexpr XString filter_prefix(const XString & str, const XString & prefix)
		{
			return str.size() >= prefix.size() ? (str(0, prefix.size()) == prefix ? str(prefix.size(), str.size()) : str) : str;
		}

		constexpr XString leftpad(const XString & str)
		{
			return (str.size() > 0 && str[0] == ' ') ? leftpad(str(1, str.size())) : str;
		}

		constexpr XString filter_class(const XString & type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "class"));
		}

		constexpr XString filter_struct(const XString & type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "struct"));
		}

		constexpr XString filter_typename_prefix(const XString & type_name)
		{
			return filter_struct(filter_class(type_name));
		}

		namespace
		{
			constexpr CString find_ith_impl(const XString & name, const XString & substring, CString res, size_t i, bool infinite = false)
			{
				return (name.length() >= substring.length()) ?
					((name(0, substring.length()) == substring) ?
					((i == 0) ?
						name.begin()
						:
						find_ith_impl(name(substring.length(), name.length()), substring, name.begin(), i - 1, infinite))
					:
						find_ith_impl(name(1, name.length()), substring, res, i, infinite))
					:
					(!infinite) ? name.end() : res;
			}
		}

		constexpr CString find_ith(const XString & name, const XString & substring, size_t i)
		{
			return find_ith_impl(name, substring, name.end(), i);
		}

		constexpr CString find_last(const XString & name, const XString & substring)
		{
			return find_ith_impl(name, substring, name.end(), -1, true);
		}

		constexpr CString find(const XString & name, const XString & substring)
		{
			return find_ith(name, substring, 0);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAME_FILTERS_HPP_