#include <ML/Core/StringUtility.hpp>


#define BUFFER_SIZE 512

namespace ml
{
	String StringUtility::Replace(const String & src, const String & find, const String & replace)
	{
		String copy = src;
		if (src != "" && find != "")
		{
			for (size_t i = 0; (i = copy.find(find, i)) != String::npos;)
			{
				copy.replace(i, find.size(), replace);
				i += replace.size();
			}
		}
		return copy;
	}

	String StringUtility::Sprintf(const String fmt, ...)
	{
		List<char> buf(BUFFER_SIZE);
		va_list args;
		va_start(args, fmt);
		{
			vsnprintf_s(
				&buf[0],
				buf.size(),
				buf.size() + strlen(fmt.c_str()),
				fmt.c_str(),
				args);
		}
		va_end(args);
		return String(&buf[0]);
	}
	
	String	StringUtility::ToLower(const String & src)
	{
		std::locale loc;

		String toStr = src;

		for (String::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::tolower(src[i], loc);
		}

		return toStr;
	}
	
	String	StringUtility::ToUpper(const String & src)
	{
		std::locale loc;

		String toStr = src;

		for (String::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::toupper(src[i], loc);
		}

		return toStr;
	}

	List<String> StringUtility::Split(const String & value, const String & delim)
	{
		List<String> list;
		String	cpy = value;
		size_t	pos = 0;
		String	tok;
		while ((pos = cpy.find(delim)) != String::npos)
		{
			tok = cpy.substr(0, pos);
			list.push_back(tok);
			cpy.erase(0, pos + delim.length());
		}
		list.push_back(cpy);
		return list;
	}



	bool	StringUtility::IsAlpha(const String & src)
	{
		for (char c : src)
		{
			if (!isalpha(c))
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsAlnum(const String & src)
	{
		for (char c : src)
		{
			if (!isalpha(c) || !isdigit(c))
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsBool(const String & src)
	{
		const String low = ToLower(src);
		return
			low == "true" ||
			low == "false" ||
			low == "1" ||
			low == "0";
	}
	
	bool	StringUtility::IsDecimal(const String & src)
	{
		if (!src.empty())
		{
			char * endptr = 0;
			strtod(src.c_str(), &endptr);
			if (*endptr != '\0' || endptr == src)
			{
				return false;
			}
			return true;
		}
		return false;
	}
	
	bool	StringUtility::IsInt(const String & src)
	{
		if (!src.empty())
		{
			String::const_iterator it = src.begin();

			if (*it == '-')
			{
				it++;
			}

			while (it != src.end() && std::isdigit(*it))
			{
				++it;
			}

			return !src.empty() && it == src.end();
		}
		return false;
	}
	
	bool	StringUtility::IsLower(const String & src)
	{
		for (auto c : src)
		{
			if (!std::islower(c))
				return false;
		}
		return true;
	}
	
	bool	StringUtility::IsName(const String & src)
	{
		if (src.empty())
		{
			return false;
		}
		else if (src.size() == 1)
		{
			return IsAlpha(src);
		}
		else
		{
			if (isalpha(src[0]) || src[0] == '_')
			{
				for (size_t i = 1; i < src.size(); i++)
				{
					if (src[i] == '_')
					{
						continue;
					}
					else if (!isalnum(src[i]))
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsNumber(const String & src)
	{
		return IsInt(src) || IsDecimal(src);
	}
	
	bool	StringUtility::IsText(const String & src)
	{
		return IsWrap(src, '\"');
	}
	
	bool	StringUtility::IsUpper(const String & src)
	{
		for (auto c : src)
		{
			if (!std::isupper(c))
				return false;
		}
		return true;
	}
	
	bool	StringUtility::IsWrap(const String & src, char c)
	{
		return IsWrap(src, c, c);
	}
	
	bool	StringUtility::IsWrap(const String & src, char front, char back)
	{
		return
			src.length() > 2 &&
			src.front() == front &&
			src.back() == back;
	}
	
	bool	StringUtility::IsWrap(const String & src, const String & s)
	{
		return IsWrap(src, s, s);
	}
	
	bool	StringUtility::IsWrap(const String & src, const String & front, const String & back)
	{
		if (src.length() >= (front.length() + back.length()))
		{
			return
				(src.substr(0, front.length()) == front) &&
				(src.substr(src.length() - back.length()) == back);
		}

		return false;
	}


	bool	StringUtility::ToBool(const String & src)
	{
		return (src == "1" || ToLower(src) == "true");
	}
	
	int32_t	StringUtility::ToInt(const String & src)
	{
		return std::stoi(src);
	}
	
	float64_t	StringUtility::ToDecimal(const String & src)
	{
		return std::stod(src);
	}
	
	float_t	StringUtility::ToFloat(const String & src)
	{
		return std::stof(src);
	}


	bool	StringUtility::MakeBool(const String & src, bool & value)
	{
		if (ToBool(src))
		{
			return (value = true);
		}

		if (src == "0")		value = false;	return true;
		if (src == "false") value = true;	return true;

		return (value = 0);
	}
	
	bool	StringUtility::MakeInt(const String & src, int32_t & value)
	{
		if (IsInt(src))
		{
			value = std::stoi(src);
			return true;
		}
		return (value = 0);
	}
	
	bool	StringUtility::MakeDouble(const String & src, float64_t & value)
	{
		if (IsDecimal(src))
		{
			value = std::stod(src);
			return true;
		}
		return (value = 0);
	}
	
	bool	StringUtility::MakeFloat(const String & src, float_t & value)
	{
		if (IsDecimal(src))
		{
			value = std::stof(src);
			return true;
		}
		return (value = 0);
	}
}