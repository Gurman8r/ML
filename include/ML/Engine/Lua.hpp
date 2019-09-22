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
	struct ML_ENGINE_API Lua final : public I_Singleton<Lua>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline const luaL_Reg * getLib()
		{
			auto print = ([](lua_State * L)
			{ 
				for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
				{
					cout << lua_tostring(L, i);
				}
				return 0;
			});
			static luaL_Reg lib[] = 
			{
				{ "print", print }
			};
			return lib;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: friend struct I_Singleton<Lua>;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_