#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using IncludeCallback = typename String(*)(const String &);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static String parseIncludes(String line, char lhs, char rhs, IncludeCallback callback)
	{
		if (line && lhs && rhs && callback)
		{
			if (line.trim() && line.front() == '#')
			{
				line.erase(line.begin());
				static constexpr StringView inc{ "include" };
				if (line.trim().substr(0, inc.size()) == inc.str())
				{
					if (const size_t a{ line.find_first_of(lhs) }; a != String::npos)
					{
						if (const size_t b{ line.find_last_of(rhs) }; b != String::npos)
						{
							if (a != b) { return callback(line.substr(a + 1, b - a - 1)); }
						}
					}
				}
			}
		}
		return String();
	}

	static String parseIncludes(String const & src)
	{
		auto clbk = ([](String const & name) {
			if (String file{ ML_FS.getFileContents(ML_FS.pathTo(name)) })
			{
				return file;
			}
			return String();
		});

		SStream	out;
		SStream	ss{ src };
		String	line;
		while (std::getline(ss, line))
		{
			String content;
			if ((content = parseIncludes(line, '\"', '\"', clbk)) ||
				(content = parseIncludes(line, '<', '>', clbk)))
			{
				out << parseIncludes(content);
			}
			else
			{
				out << line << '\n';
			}
		}
		return out.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ShaderParser::parse(String const & src, SStream & v, SStream & g, SStream & f)
	{
		SStream * dst = nullptr;
		size_t	count = 0;
		SStream ss { parseIncludes(src.replace_all("\0", " ")) };
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
				(*dst) << line << '\n';
				count++;
			}
		}
		return count;
	}

	String ShaderParser::parse(String const & src)
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
				out << line << '\n';
			}
		}
		return out.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}