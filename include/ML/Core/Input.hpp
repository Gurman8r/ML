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
		input() = default;

		inline auto operator()(std::istream & in) const
		{
			T temp {};
			if (in.good()) { in >> temp; }
			return temp;
		}

		inline auto operator()(const String & str) const
		{
			SStream ss { str };
			return (*this)(ss);
		}

		inline auto operator()() const
		{
			return (*this)(cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<String> final
	{
		input() = default;

		inline auto operator()(std::istream & in) const
		{
			String temp {};
			if (in.good()) { in >> temp; }
			return temp;
		}

		inline auto operator()(const String & str) const
		{
			SStream ss { str };
			return (*this)(ss);
		}

		inline auto operator()(std::istream & in, char delim)
		{
			String temp {};
			std::getline(in, temp, delim);
			return temp;
		}

		inline auto operator()() const
		{
			return (*this)(cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<bool>
	{
		input() = default;

		inline auto operator()(const String & in) const
		{
			return alg::to_bool(in);
		}

		inline auto operator()(std::istream & in) const
		{
			return (*this)(input<String>()(in));
		}

		inline auto operator()(std::istream & in, char delim)
		{
			return (*this)(input<String>()(in, delim));
		}

		inline auto operator()() const
		{
			return (*this)(cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From> struct input<To, From>
	{
		input() = default;

		inline auto operator()(std::istream & in) const
		{
			return static_cast<To>(input<From>()(in));
		}

		inline auto operator()(const String & str) const
		{
			return static_cast<To>(input<From>()(str));
		}

		inline auto operator()(std::istream & in, char delim) const
		{
			return static_cast<To>(input<From>()(in, delim));
		}

		inline auto operator()() const
		{
			return static_cast<To>(input<From>()());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_