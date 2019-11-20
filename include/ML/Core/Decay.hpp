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
				std::remove_const_t<
				std::remove_volatile_t<
				std::remove_reference_t<
				std::remove_pointer_t<
				T
			>>>>;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_USING_X decay_t = typename decay<X>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DECAY_HPP_