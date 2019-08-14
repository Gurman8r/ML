#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

// String Utilities

#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace alg
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline String narrow(const WString & value)
		{
			return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(value);
		}

		static inline WString widen(const String & value)
		{
			return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_alpha(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc;
			for (const auto & elem : value)
				if (!std::isalpha(elem, loc))
					return false;
			return true;
		}

		static inline bool is_alnum(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc;
			for (const auto & elem : value)
				if (!std::isalnum(elem, loc))
					return false;
			return true;
		}

		static inline bool is_graph(const String & value)
		{
			if (value.empty()) return false;
			std::locale loc;
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
			std::locale loc;
			for (auto & elem : value)
				elem = std::tolower(elem, loc);
			return value;
		}

		static inline String to_upper(String value)
		{
			std::locale loc;
			for (auto & elem : value)
				elem = std::toupper(elem, loc);
			return value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_bool(const String & value)
		{
			if (value.size() == 1) 
				return value.front() == '0' || value.front() == '1';
			const String temp { to_lower(value) };
			return (temp == "true") || (temp == "false");
		}

		static inline bool is_int(const String & value)
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

		static inline bool is_float(const String & value)
		{
			try { std::stof(value); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		static inline bool is_double(const String & value)
		{
			try { std::stod(value); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		static inline bool is_decimal(const String & value)
		{
			if (!value) { return false; }
			String::pointer endptr = nullptr;
			std::strtod(value.c_str(), &endptr);
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

		static inline bool to_bool(const String & value, bool dv = false)
		{
			return (value && (value == "1" || to_lower(value) == "true")) ? true : dv;
		}

		static inline int32_t to_int(const String & value, int32_t dv = 0)
		{
			try { return std::stoi(value); }
			catch (std::invalid_argument &) { return dv; }
		}

		static inline uint32_t to_uint(const String & value, uint32_t dv = 0)
		{
			try { return static_cast<uint32_t>(std::stoul(value)); }
			catch (std::invalid_argument &) { return dv; }
		}

		static inline float32_t to_float(const String & value, float32_t dv = 0.0f)
		{
			try { return std::stof(value); }
			catch (std::invalid_argument &) { return dv; }
		}

		static inline float64_t to_double(const String & value, float64_t dv = 0.0)
		{
			try { return std::stod(value); }
			catch (std::invalid_argument &) { return dv; }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool parse_bool(const String & value, bool & out)
		{
			if (is_bool(value))
			{
				out = to_bool(value); 
				return true;
			}
			return false;
		}

		static inline bool parse_int(const String & value, int32_t & out)
		{
			try { out = std::stoi(value); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		static inline bool parse_uint(const String & value, uint32_t & out)
		{
			try { out = static_cast<uint32_t>(std::stoul(value)); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		static inline bool parse_float(const String & value, float32_t & out)
		{
			try { out = std::stof(value); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		static inline bool parse_double(const String & value, float64_t & out)
		{
			try { out = std::stod(value); return true; }
			catch (std::invalid_argument &) { return false; }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline String to_bin(T value)
		{
			SStream ss;
			for (size_t i = 0; i < (sizeof(T) * 8); i++) 
				ss << bitRead(value, i);
			return ss.str();
		}

		template <class T> static inline String to_hex(T value)
		{
			SStream ss;
			ss << std::hex << value;
			return ss.str();
		}

		template <class T> static inline String to_oct(T value)
		{
			SStream ss;
			ss << std::oct << value;
			return ss.str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STRING_UTILITY_HPP_