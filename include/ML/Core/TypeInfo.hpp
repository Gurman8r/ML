#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <ML/Core/Signature.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		struct type_info final
		{
			constexpr type_info(const CX_String & name) : m_name { name } {}

			constexpr auto hash_code() const -> hash_t { return m_name.hash(); }

			constexpr auto raw_name() const -> const CX_String & { return m_name; }

		private: const CX_String m_name;
		};

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> static constexpr auto type_id()
	{
		return detail::type_info { signature_of<T>() };
	}

	template <class T> static constexpr auto type_id(T)
	{
		return type_id<T>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_INFO_HPP_