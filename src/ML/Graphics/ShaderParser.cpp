#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ShaderParser::parseShader(const String & src, SStream & v, SStream & g, SStream & f)
	{
		SStream * dst = nullptr;
		size_t	count = 0;
		SStream ss { parseIncludes(src.replaceAll("\0", " ")) };
		String	line;
		while (std::getline(ss, line))
		{
			if (line.find("#shader") != String::npos)
			{
				if (line.find("vertex") != String::npos)
				{
					dst = &v;
				}
				else if (line.find("fragment") != String::npos)
				{
					dst = &f;
				}
				else if (line.find("geometry") != String::npos)
				{
					dst = &g;
				}
				else
				{
					dst = nullptr;
				}
			}
			else if (dst)
			{
				(*dst) << line << endl;
				count++;
			}
		}
		return count;
	}

	String ShaderParser::parseShader(const String & src)
	{
		if ((src.find("#shader") == String::npos) &&
			(src.find("#include") == String::npos))
			return src;

		SStream ss { parseIncludes(src) };
		String	line;
		SStream out;
		while (std::getline(ss, line))
		{
			if (line.find("#shader") == String::npos)
			{
				out << line << endl;
			}
		}
		return out.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ShaderParser::parseIncludes(const String & src)
	{
		auto clbk = ([](const String & name)
		{
			if (String file { ML_FS.getFileContents(ML_FS.pathTo(name)) })
			{
				return file;
			}
			return String();
		});

		SStream	out;
		SStream	ss { src };
		String	line;
		while (std::getline(ss, line))
		{
			String content;
			if ((content = parseIncludes(line, '\"', '\"', clbk)) ||
				(content = parseIncludes(line, '<',  '>',  clbk)))
			{
				out << parseIncludes(content);
			}
			else
			{
				out << line << endl;
			}
		}
		return out.str();
	}

	String ShaderParser::parseIncludes(String line, char lhs, char rhs, IncludeCallback callback)
	{
		if (line && lhs && rhs && callback)
		{
			if (line.trim() && line.front() == '#')
			{
				line.erase(line.begin());
				static constexpr StringView inc { "include" };
				if (line.trim().substr(0, inc.size()) == inc.str())
				{
					size_t a;
					if ((a = line.find_first_of(lhs)) != String::npos)
					{
						size_t b;
						if ((b = line.find_last_of(rhs)) != String::npos)
						{
							if (a != b)
							{
								return callback(line.substr(a + 1, b - a - 1));
							}
						}
					}
				}
			}
		}
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}