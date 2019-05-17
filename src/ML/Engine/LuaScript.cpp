#include <ML/Engine/LuaScript.hpp>
#include <ML/Engine/Lua.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	LuaScript::LuaScript()
		: m_file()
	{
	}

	LuaScript::LuaScript(const LuaScript & copy)
		: m_file(copy.m_file)
	{
	}

	LuaScript::~LuaScript()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool LuaScript::dispose()
	{
		return m_file.dispose();
	}

	bool LuaScript::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * */
}