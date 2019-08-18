#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <ML/Core/Signature.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		constexpr CX_String filter_prefix(const CX_String& str, const CX_String& pre)
		{
			return str.size() >= pre.size() ? (str(0, pre.size()) == pre ? str(pre.size(), str.size()) : str) : str;
		}

		constexpr CX_String leftpad(const CX_String& str)
		{
			return (str.size() > 0 && str[0] == ' ') ? leftpad(str(1, str.size())) : str;
		}

		constexpr CX_String filter_class(const CX_String& type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "class"));
		}

		constexpr CX_String filter_struct(const CX_String& type_name)
		{
			return leftpad(filter_prefix(leftpad(type_name), "struct"));
		}

		constexpr CX_String filter_type_prefix(const CX_String& type_name)
		{
			return filter_struct(filter_class(type_name));
		}

		constexpr CX_String filter_sig_suffix(const CX_String & value)
		{
			return { value.begin(), value.size() - CX_String(ML_SIGNATURE_SUFFIX).size() };
		}

		constexpr CX_String filter_sig_prefix(const CX_String & value)
		{
			return filter_sig_suffix(
				leftpad(filter_prefix(leftpad(value), ML_SIGNATURE_PREFIX))
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr CX_String ml_name_of()
	{
		return detail::filter_sig_prefix(signature_of<T>());
	}

	template <class T> static constexpr CX_String ml_name_of(T)
	{
		return ml_name_of<T>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		struct type_info final
		{
			constexpr type_info(const CX_String & name) : m_name { name } {}

			constexpr auto hash_code() const -> hash_t { return m_name.hash(); }

			constexpr auto raw_name() const -> const CX_String & { return m_name; }

			constexpr auto name() const -> CX_String { return filter_sig_prefix(raw_name()); }

		private: const CX_String m_name;
		};

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr auto ml_type_id()
	{
		return detail::type_info { signature_of<T>() };
	}

	template <class T> static constexpr auto ml_type_id(T)
	{
		return ml_type_id<T>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_INFO_HPP_