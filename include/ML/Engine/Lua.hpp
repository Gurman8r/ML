#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "lua.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#include <lua/lua.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Lua ::ml::Lua::getInstance()

namespace ml
{
	class ML_ENGINE_API Lua final : public Singleton<Lua>, public Disposable
	{
		friend struct Singleton<Lua>;
		mutable lua_State * m_L { nullptr };

	public:
		inline lua_State * getL() const
		{ 
			auto my_print = ([](lua_State * L)
			{
				for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
				{
					cout << lua_tostring(L, i);
				}
				return 0;
			});
			static const struct luaL_Reg lib[] = {
				{ "print", my_print },
				{ nullptr, nullptr }
			};
			if (!m_L && (m_L = luaL_newstate()))
			{
				luaL_openlibs(m_L);
				lua_getglobal(m_L, "_G");
				luaL_setfuncs(m_L, lib, 0);
				lua_pop(m_L, 1);
			}
			return m_L;
		}

		inline bool dispose() override
		{
			if (m_L)
			{
				lua_close(m_L);
				return true;
			}
			return false;
		}

		inline int32_t DoString(const String & value) const
		{
			return ((value && getL()) ? luaL_dostring(getL(), value.c_str()) : 0);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_