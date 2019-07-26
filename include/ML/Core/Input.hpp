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

	template <> struct input<String>
	{
		using value_type = typename String;

		inline String operator()(Ostream & out, Istream & in, const String & msg) const
		{
			if (msg) { out << msg << endl << endl; }
			String temp;
			std::getline(in, temp);
			if (temp.trim() && msg) { out << endl; }
			return temp;
		}

		inline String operator()(Istream & in, const String & msg) const
		{
			return (*this)(cout, in, msg);
		}

		inline String operator()(const String & msg) const
		{
			return (*this)(cout, cin, msg);
		}

		inline String operator()(Istream & in) const
		{
			return (*this)(cout, in, String());
		}

		inline String operator()() const
		{
			return (*this)(cout, cin, String());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct input<T>
	{
		using value_type = typename T;

		inline value_type operator()(Istream & in) const
		{
			value_type temp;
			in >> temp;
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