#ifndef _ML_INPUT_HPP_
#define _ML_INPUT_HPP_

#include <ML/Core/Debug.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/NonNewable.hpp>

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

	template <class To, class From> struct input<To, From> final
	{
		input() = default;

		inline auto operator()(const String & str, To dv = To { 0 }) const
		{
			return static_cast<To>(input<From>{}(str, static_cast<From>(dv)));
		}

		inline auto operator()(std::istream & in, To dv = To { 0 }) const
		{
			return static_cast<To>(input<From>{}(in, static_cast<From>(dv)));
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
			return util::to_bool(str);
		}

		inline auto operator()(std::istream & in) const
		{
			return (*this)(input<String>{}(in));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int8_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int8_t dv = 0) const
		{
			return util::to_i8(str, dv);
		}

		inline auto operator()(std::istream & in, int8_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int16_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int16_t dv = 0) const
		{
			return util::to_i16(str, dv);
		}

		inline auto operator()(std::istream & in, int16_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int32_t dv = 0) const
		{
			return util::to_i32(str, dv);
		}

		inline auto operator()(std::istream & in, int32_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, int64_t dv = 0) const
		{
			return util::to_i64(str, dv);
		}

		inline auto operator()(std::istream & in, int64_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint8_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint8_t dv = 0) const
		{
			return util::to_u8(str, dv);
		}

		inline auto operator()(std::istream & in, uint8_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint16_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint16_t dv = 0) const
		{
			return util::to_u16(str, dv);
		}

		inline auto operator()(std::istream & in, uint16_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint32_t dv = 0) const
		{
			return util::to_u32(str, dv);
		}

		inline auto operator()(std::istream & in, uint32_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, uint64_t dv = 0) const
		{
			return util::to_u64(str, dv);
		}

		inline auto operator()(std::istream & in, uint64_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float32_t> final
	{
		input() = default;

		inline auto operator()(const String & str, float32_t dv = 0) const
		{
			return util::to_f32(str, dv);
		}

		inline auto operator()(std::istream & in, float32_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float64_t> final
	{
		input() = default;

		inline auto operator()(const String & str, float64_t dv = 0) const
		{
			return util::to_f64(str, dv);
		}

		inline auto operator()(std::istream & in, float64_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float80_t> final
	{
		input() = default;

		inline auto operator()(const String & str, float80_t dv = 0) const
		{
			return util::to_f80(str, dv);
		}

		inline auto operator()(std::istream & in, float80_t dv = 0) const
		{
			return (*this)(input<String>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_