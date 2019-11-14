#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	inline auto operator "" _s(C_String data, size_t size)
	{
		return String { data, size };
	}

	inline auto operator "" _s(CW_String data, size_t size)
	{
		return W_String { data, size };
	}

	inline auto operator "" _s(C16_String data, size_t size)
	{
		return U16_String { data, size };
	}

	inline auto operator "" _s(C32_String data, size_t size)
	{
		return U32_String { data, size };
	}

	/* * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline List<String> tokenize(String value, const String & delim)
		{
			List<String> out;
			size_t pos = 0;
			while ((pos = value.find(delim)) != String::npos)
			{
				out.push_back(value.substr(0, pos));
				value.erase(0, pos + delim.length());
			}
			out.push_back(value);
			return out;
		}

		template <
			class Arg0, class ... Args
		> inline SStream sink(const Arg0 & arg0, Args && ... args)
		{
			SStream ss {}; ss << arg0 << endl;
			int32_t i[] = { 0, ((void)(ss << args << endl), 0)... }; (void)i;
			return ss;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class To, class From
		> static inline BasicString<To> str_convert(const BasicString<From> & value)
		{
			BasicString<To> temp {};
			temp.reserve(value.size());
			for (const From & c : value)
				temp.push_back(static_cast<To>(c));
			return temp;
		}

		static inline String narrow(const W_String & value)
		{ 
			return str_convert<char>(value);
		}

		static inline W_String widen(const String & value)
		{ 
			return str_convert<wchar_t>(value); 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_alpha(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (const auto & elem : value)
				if (!std::isalpha(elem, loc))
					return false;
			return true;
		}

		static inline bool is_alnum(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (const auto & elem : value)
				if (!std::isalnum(elem, loc))
					return false;
			return true;
		}

		static inline bool is_graph(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (const auto & elem : value)
				if (!std::isgraph(elem, loc))
					return false;
			return true;
		}

		static inline bool is_print(const String & value)
		{
			for (const auto & elem : value)
				if (!std::isprint(elem))
					return false;
			return true;
		}

		static inline bool is_lower(const String & value)
		{
			for (const auto & elem : value)
				if (!std::islower(elem))
					return false;
			return true;
		}

		static inline bool is_upper(const String & value)
		{
			for (const auto & elem : value)
				if (!std::islower(elem))
					return false;
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline String to_lower(String value)
		{
			std::locale loc {};
			for (auto & elem : value)
				elem = std::tolower(elem, loc);
			return value;
		}

		static inline String to_upper(String value)
		{
			std::locale loc {};
			for (auto & elem : value)
				elem = std::toupper(elem, loc);
			return value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_bool(const String & value)
		{
			switch (to_lower(value).hash())
			{
			case Hash("1"):
			case Hash("true"):
			case Hash("on"):
			case Hash("yes"):
			case Hash("0"):
			case Hash("false"):
			case Hash("off"):
			case Hash("no"):
				return true;

			default: return false;
			}
		}

		static inline bool is_integer(const String & value)
		{
			if (!value) return false; 
			String::const_iterator it = value.cbegin();
			if ((*it) == '-')
			{ 
				it++; 
			}
			while (it != value.cend() && std::isdigit(*it))
			{
				++it;
			}
			return (it == value.cend());
		}

		static inline bool is_decimal(const String & value)
		{
			if (!value) { return false; }
			String::pointer endptr = nullptr;
			auto temp { std::strtod(value.c_str(), &endptr) };
			return !(*endptr != '\0' || endptr == value);
		}

		static inline bool is_name(const String & value)
		{
			if (!value.empty() && (std::isalpha(value.front()) || (value.front() == '_')))
			{
				for (size_t i = 1; i < value.size(); i++)
				{
					if (value[i] == '_')
					{
						continue;
					}
					else if (!std::isalnum(value[i]))
					{
						return false;
					}
				}
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool to_bool(const String & value, bool dv = 0)
		{
			switch (to_lower(value).hash())
			{
			case Hash("1"):
			case Hash("true"):
			case Hash("on"):
			case Hash("yes"):
				return true;

			case Hash("0"):
			case Hash("false"):
			case Hash("off"):
			case Hash("no"):
				return false;
			
			default: return dv;
			}
		}

		static inline int32_t to_i8(const String & value, int8_t dv = 0)
		{
			try { return static_cast<int8_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int32_t to_i16(const String & value, int16_t dv = 0)
		{
			try { return static_cast<int16_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int32_t to_i32(const String & value, int32_t dv = 0)
		{
			try { return static_cast<int32_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int64_t to_i64(const String & value, int64_t dv = 0)
		{
			try { return static_cast<int64_t>(std::stoll(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint8_t to_u8(const String & value, uint8_t dv = 0)
		{
			try { return static_cast<uint8_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint16_t to_u16(const String & value, uint16_t dv = 0)
		{
			try { return static_cast<uint16_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint32_t to_u32(const String & value, uint32_t dv = 0)
		{
			try { return static_cast<uint32_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint64_t to_u64(const String & value, uint64_t dv = 0)
		{
			try { return static_cast<uint64_t>(std::stoull(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float32_t to_f32(const String & value, float32_t dv = 0)
		{
			try { return static_cast<float32_t>(std::stof(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float64_t to_f64(const String & value, float64_t dv = 0)
		{
			try { return static_cast<float64_t>(std::stod(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float80_t to_f80(const String & value, float80_t dv = 0)
		{
			try { return static_cast<float80_t>(std::stold(value)); }
			catch (std::exception &) { return dv; }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline String to_string(const String & value) { return value; }

		static inline String to_string(int8_t value) { return std::to_string(value); }

		static inline String to_string(int16_t value) { return std::to_string(value); }

		static inline String to_string(int32_t value) { return std::to_string(value); }

		static inline String to_string(int64_t value) { return std::to_string(value); }

		static inline String to_string(uint8_t value) { return std::to_string(value); }

		static inline String to_string(uint16_t value) { return std::to_string(value); }

		static inline String to_string(uint32_t value) { return std::to_string(value); }

		static inline String to_string(uint64_t value) { return std::to_string(value); }

		static inline String to_string(float32_t value) { return std::to_string(value); }

		static inline String to_string(float64_t value) { return std::to_string(value); }

		static inline String to_string(float80_t value) { return std::to_string(value); }

		template <class T> static inline String to_string(const T & value)
		{
			SStream ss {}; 
			ss << value; 
			return ss.str();
		}

		template <class T> static inline C_String to_cstring(const T & value)
		{
			return to_string<T>(value).c_str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S> static inline String format(String fmt, const List<S> & args)
		{
			for (size_t i = 0, imax = args.size(); i < imax; i++)
			{
				fmt.replace_all(("{" + to_string(i) + "}"), args[i]);
			}
			return fmt;
		}

		template <class T> static inline String to_hex(const T & value)
		{
			SStream ss {};
			ss << std::hex << value;
			return ss.str();
		}

		template <class T> static inline String to_oct(const T & value)
		{
			SStream ss {};
			ss << std::oct << value;
			return ss.str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STRING_UTILITY_HPP_