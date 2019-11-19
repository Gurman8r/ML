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

		Lua() {}
		~Lua() {}

		lua_State * m_L{ nullptr };

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		lua_State * init();
		lua_State * init(bool openLibs, luaL_Reg const * userLib);
		bool		dispose() override;
		int32_t		doString(String const & value) const;
		int32_t		doFile(String const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_