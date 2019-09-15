#ifndef _ML_DECAY_HPP_
#define _ML_DECAY_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct decay final
		{
			ML_USING_X remove_const_t = std::remove_const_t<X>;
			ML_USING_X remove_pointer_t = std::remove_pointer_t<X>;
			ML_USING_X remove_reference_t = std::remove_reference_t<X>;

			using type = typename remove_const_t<remove_reference_t<remove_pointer_t<T>>>;
		};

		template <class T> using decay_t = typename decay<T>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DECAY_HPP_