#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool ShaderParser::parseShader(const String & src, SStream & v, SStream & g, SStream & f)
	{
		SStream *	dst = nullptr;
		size_t		count = 0;
		SStream		ss(parseIncludes(src.replaceAll("\0", " ")));
		String		line;

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

		SStream ss(parseIncludes(src));
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

	/* * * * * * * * * * * * * * * * * * * * */

	String ShaderParser::parseIncludes(const String & src)
	{
		auto includeCallback = [](const String & name)
		{
			if (name)
			{
				const String path(ML_FS.pathTo(
					"../../../assets/shaders/" + name
				));
				String file;
				if (ML_FS.getFileContents(path, file))
				{
					file.pop_back();
					return file;
				}
			}
			return String();
		};

		SStream	out;
		SStream	ss(src);
		String	line;
		while (std::getline(ss, line))
		{
			String data;
			if ((data = parseIncludes(line, '\'', '\'', includeCallback)) ||
				(data = parseIncludes(line, '\"', '\"', includeCallback)) ||
				(data = parseIncludes(line, '<', '>',	includeCallback)))
			{
				out << parseIncludes(data);
			}
			else
			{
				if (line.find("#include") != String::npos)
				{
					Debug::logError("Include Failed: {0}", line);
				}

				out << line << endl;
			}
		}
		return out.str();
	}

	String ShaderParser::parseIncludes(const String & line, char lhs, char rhs, IncludeClbk callback)
	{
		if (line && lhs && rhs && callback)
		{
			SStream	out;
			if (line.find("#include") != String::npos)
			{
				String name, path;
				if (parseWrapped(line, lhs, rhs, name))
				{
					out << callback(name);
				}
			}
			return out.str();
		}
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ShaderParser::parseWrapped(const String & src, const char lhs, const char rhs, String & out)
	{
		size_t a;
		if ((a = src.find_first_of(lhs)) != String::npos)
		{
			size_t b;
			if ((b = src.find_last_of(rhs)) != String::npos)
			{
				if (a != b)
				{
					return (bool)(out = src.substr((a + 1), (b - a - 1)));
				}
			}
		}
		return (bool)(out = String());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}