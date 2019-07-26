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
			if (temp.trim()) { out << endl; }
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

	template <> struct input<int32_t>
	{
		using value_type = typename int32_t;

		static constexpr value_type err { _I32_MAX };
		
		inline value_type operator()(Ostream & out, Istream & in, const String & msg) const
		{
			try { return std::stoi(input<String>()(out, in, msg)); }
			catch (std::invalid_argument &) { return err; }
		}

		inline value_type operator()(const String & msg) const
		{
			return (*this)(cout, cin, msg);
		}

		inline value_type operator()(Istream & in) const
		{
			return (*this)(cout, in, String());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint32_t>
	{
		using value_type = typename uint32_t;

		static constexpr value_type err { _UI32_MAX };
		
		inline value_type operator()(Ostream & out, Istream & in, const String & msg) const
		{
			try { return static_cast<value_type>(input<int32_t>()(out, in, msg)); }
			catch (std::invalid_argument &) { return err; }
		}

		inline value_type operator()(const String & msg) const
		{
			return (*this)(cout, cin, msg);
		}

		inline value_type operator()(Istream & in) const
		{
			return (*this)(cout, in, String());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float32_t>
	{
		using value_type = typename float32_t;

		static constexpr value_type err { FLT_MAX };
		
		inline value_type operator()(Ostream & out, Istream & in, const String & msg) const
		{
			try { return static_cast<value_type>(std::stof(input<String>()(out, in, msg))); }
			catch (std::invalid_argument &) { return err; }
		}

		inline value_type operator()(const String & msg) const
		{
			return (*this)(cout, cin, msg);
		}

		inline value_type operator()(Istream & in) const
		{
			return (*this)(cout, in, String());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float64_t>
	{
		using value_type = typename float64_t;

		static constexpr value_type err { DBL_MAX };

		inline value_type operator()(Ostream & out, Istream & in, const String & msg) const
		{
			try { return static_cast<value_type>(std::stod(input<String>()(out, in, msg))); }
			catch (std::invalid_argument &) { return err; }
		}

		inline value_type operator()(const String & msg) const
		{
			return (*this)(cout, cin, msg);
		}

		inline value_type operator()(Istream & in) const
		{
			return (*this)(cout, in, String());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct input<T>
	{
		using value_type = typename std::decay<T>::type;

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