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
			decay() = delete;

			ML_USING_X remove_const_t		= typename _STD remove_const_t<X>;
			ML_USING_X remove_volatile_t	= typename _STD remove_volatile_t<X>;
			ML_USING_X remove_pointer_t		= typename _STD remove_pointer_t<X>;
			ML_USING_X remove_reference_t	= typename _STD remove_reference_t<X>;

			using type = typename 
				remove_const_t<
				remove_volatile_t<
				remove_reference_t<
				remove_pointer_t<
				T
			>>>>;
		};

		template <class T> using decay_t = typename decay<T>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DECAY_HPP_