#ifndef _ML_INPUT_HPP_
#define _ML_INPUT_HPP_

#include <ML/Core/Debug.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct input;

	template <> struct input<> final { input() = delete; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct input<T> final
	{
		inline T operator()(std::istream & in) const
		{
			T temp {};
			if (in.good()) { in >> temp; }
			return temp;
		}

		inline T operator()(const String & str) const
		{
			SStream ss { str };
			return (*this)(ss);
		}

		inline T operator()() const
		{
			return (*this)(cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From> struct input<To, From>
	{
		inline To operator()(std::istream & in) const
		{
			return static_cast<To>(input<From>()(in));
		}

		inline To operator()(const String & str) const
		{
			return static_cast<To>(input<From>()(str));
		}

		inline To operator()() const
		{
			return static_cast<To>(input<From>()());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_