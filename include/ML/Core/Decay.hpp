#ifndef _ML_DECAY_HPP_
#define _ML_DECAY_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Used to get the absolute type of T
		template <class T> struct decay final
		{
			decay() = delete;

			using type = typename 
				_STD remove_const_t<
				_STD remove_volatile_t<
				_STD remove_reference_t<
				_STD remove_pointer_t<
				T
			>>>>;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> using decay_t = typename decay<T>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DECAY_HPP_