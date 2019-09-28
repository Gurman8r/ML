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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<String> final
	{
		input() = default;

		inline auto operator()(std::istream & in) const
		{
			String temp {};
			in >> temp;
			return temp;
		}

		inline auto operator()(std::istream & in, char delim)
		{
			String temp {};
			std::getline(in, temp, delim);
			return temp;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<bool> final
	{
		input() = default;

		inline auto operator()(const String & str) const
		{
			return alg::to_bool(str);
		}

		inline auto operator()(std::istream & in) const
		{
			return (*this)(input<String>()(in));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, float32_t dv = 0.0f) const
		{
			return alg::to_float(str, dv);
		}

		inline auto operator()(std::istream & in, float32_t dv = 0.0f) const
		{
			return (*this)(input<String>()(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, float64_t dv = 0.0) const
		{
			return alg::to_double(str, dv);
		}

		inline auto operator()(std::istream & in, float64_t dv = 0.0) const
		{
			return (*this)(input<String>()(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int32_t dv = 0.0f) const
		{
			return alg::to_int(str, dv);
		}

		inline auto operator()(std::istream & in, int32_t dv = 0.0f) const
		{
			return (*this)(input<String>()(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint32_t dv = 0.0f) const
		{
			return alg::to_uint(str, dv);
		}

		inline auto operator()(std::istream & in, uint32_t dv = 0.0f) const
		{
			return (*this)(input<String>()(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int64_t dv = 0) const
		{
			return static_cast<int64_t>(input<int32_t>()(str, static_cast<int32_t>(dv)));
		}

		inline auto operator()(std::istream & in, int64_t dv = 0) const
		{
			return static_cast<int64_t>(input<int32_t>()(in, static_cast<int32_t>(dv)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint64_t dv = 0) const
		{
			return static_cast<uint64_t>(input<uint32_t>()(str, static_cast<uint32_t>(dv)));
		}

		inline auto operator()(std::istream & in, uint64_t dv = 0) const
		{
			return static_cast<uint64_t>(input<uint32_t>()(in, static_cast<uint32_t>(dv)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_