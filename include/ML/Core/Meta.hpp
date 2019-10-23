#ifndef _ML_META_HPP_
#define _ML_META_HPP_

#include <ML/Core/TypeOf.hpp>

namespace ml
{
	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... T> struct info;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct info<T> final
		{
			constexpr info() noexcept {}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct info<> final
		{
			constexpr info() noexcept {}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

#endif // !_ML_META_HPP_