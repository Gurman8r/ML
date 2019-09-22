#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: Script(Language::Python)
	{
	}

	Script::Script(Language language)
		: m_language(language)
		, m_filename()
		, m_text()
	{
	}

	Script::Script(const String & filename)
		: Script()

	{
		loadFromFile(filename);
	}

	Script::Script(const Script & copy)
		: m_language(copy.m_language)
		, m_filename(copy.m_filename)
		, m_text(copy.m_text)
	{
	}

	Script::~Script() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::loadFromFile(const String & filename)
	{
		if (m_text = ML_FS.getFileContents(m_filename = filename))
		{
			if (ML_FS.getFileType(m_filename) == "py")
				m_language = Language::Python;

			if (ML_FS.getFileType(m_filename) == "lua")
				m_language = Language::Lua;

			return true;
		}
		return false;
	}

	int32_t Script::execute() const
	{
		switch (m_language)
		{
		case Language::Lua:
			if (m_text)
			{
				auto my_print = [](lua_State * L)
				{
					for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
					{
						cout << lua_tostring(L, i);
					}
					return 0;
				};
				static const struct luaL_Reg printLib[] = {
					{ "print", my_print },
					{ nullptr, nullptr }
				};
				lua_State * L = luaL_newstate();
				luaL_openlibs(L);
				lua_getglobal(L, "_G");
				luaL_setfuncs(L, printLib, 0);
				lua_pop(L, 1);
				if (luaL_dostring(L, m_text.c_str()) != LUA_OK)
				{
					cout << "Lua Error: " << String(lua_tostring(L, -1)) << endl;
				}
				lua_close(L);
				return 1;
			}
		case Language::Python: 
			return Py::RunString(m_text);
		default: 
			return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script & Script::setLanguage(Language value)
	{
		m_language = value;
		return (*this);
	}

	Script & Script::setText(const String & value)
	{
		m_text = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}