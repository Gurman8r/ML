#ifndef _ML_INPUT_HPP_
#define _ML_INPUT_HPP_

#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... Ts
	> struct input;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct input<T>
	{
		using value_type = typename T;

		inline value_type operator()(std::istream & in) const
		{
			value_type temp {};
			if (in.good()) { in >> temp; }
			return temp;
		}

		inline value_type operator()() const
		{
			return (*this)(cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_