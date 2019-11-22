#ifndef _ML_ROOT_OF_HPP_
#define _ML_ROOT_OF_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct root final
		{
			root() = delete;

			using type = typename 
				std::remove_const_t<
				std::remove_volatile_t<
				std::remove_reference_t<
				std::remove_pointer_t<
				T
			>>>>;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// remove all qualifiers pointers and references
		ML_USING_X root_t = typename root<X>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ROOT_OF_HPP_