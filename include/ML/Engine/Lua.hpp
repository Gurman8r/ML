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
	private: friend struct I_Singleton<Lua>;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_