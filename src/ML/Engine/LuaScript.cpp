#include <ML/Engine/LuaScript.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool LuaScript::dispose()
	{
		return m_file.dispose();
	}

	bool LuaScript::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}