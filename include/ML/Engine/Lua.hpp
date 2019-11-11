#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#define ML_Lua ::ml::Lua::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	class ML_ENGINE_API Lua final : public Singleton<Lua>, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct Singleton<Lua>;

		lua_State * m_L { nullptr };

		Lua() {}
		~Lua() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline lua_State * init()
		{
			auto my_print{ ([](lua_State * L)
			{
				for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
					cout << lua_tostring(L, i);
				return EXIT_SUCCESS;
			}) };

			static const struct luaL_Reg lib[] =
			{
				{ "print", my_print },
				{ nullptr, nullptr }
			};

			return init(true, lib);
		}

		inline lua_State * init(bool openLibs, const luaL_Reg * userLib)
		{
			if (!m_L && (m_L = luaL_newstate()))
			{
				if (openLibs) { luaL_openlibs(m_L); }

				lua_getglobal(m_L, "_G");

				if (userLib) { luaL_setfuncs(m_L, userLib, 0); }

				lua_pop(m_L, 1);
			}
			return m_L;
		}

		inline bool dispose() override
		{
			if (m_L)
			{
				lua_close(m_L);
				m_L = nullptr;
				return true;
			}
			return false;
		}

		inline int32_t doString(const String & value) const
		{
			return ((value && m_L) ? luaL_dostring(m_L, value.c_str()) : 0);
		}

		inline int32_t doFile(const String & filename) const
		{
			return doString(ML_FS.getFileContents(filename));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_