#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: m_filename()
	{
	}

	Script::Script(const String & filename)
		: m_filename()
	{
		loadFromFile(filename);
	}

	Script::Script(Script && copy)
		: Script()
	{
		std::swap(m_filename, copy.m_filename);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::dispose()
	{
		return false;
	}

	bool Script::loadFromFile(const String & filename)
	{
		if (String text { ML_FS.getFileContents(filename) })
		{

		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}