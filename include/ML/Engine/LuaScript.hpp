#ifndef _ML_LUA_SCRIPT_HPP_
#define _ML_LUA_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API LuaScript final
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		LuaScript();
		LuaScript(const LuaScript & copy);
		~LuaScript();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:

	private:
		File m_file;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LUA_SCRIPT_HPP_