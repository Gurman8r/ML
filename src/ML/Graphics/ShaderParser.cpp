#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool ShaderParser::parseShader(const String & src, SStream & v, SStream & g, SStream & f)
	{
		SStream *	dst = NULL;
		size_t		count = 0;
		SStream		ss(parseIncludes(src));
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
					dst = NULL;
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

	/* * * * * * * * * * * * * * * * * * * * */

	String ShaderParser::parseIncludes(const String & src)
	{
		SStream		out;
		SStream		ss(src);
		String		line;

		while (std::getline(ss, line))
		{
			if (line.find("#include") != String::npos)
			{
				String name, path;
				
				// '...'
				if (parseWrapped(line, '\'', '\'', name))
				{
					path = ML_FS.getPathTo(name);
				}
				// "..."
				else if (parseWrapped(line, '\"', '\"', name))
				{
					path = ML_FS.getPathTo(name);
				}
				// <...>
				else if (parseWrapped(line, '<', '>', name))
				{
					path = ML_FS.getPathTo("../../../assets/shaders/" + name);
				}
				else
				{
					Debug::logError("Shader Include Failed: \'{0}\'", name);
					name = path = String();
				}

				if (name && path)
				{
					String file;
					if (ML_FS.getFileContents(path, file))
					{
						file.pop_back();
						out << parseIncludes(file);
					}
					else
					{
						Debug::logError("Failed Including Shader: {0} {1}", name, path);
					}
				}
			}
			else
			{
				out << line << endl;
			}
		}
		return out.str();
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