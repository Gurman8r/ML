#ifndef _ML_PLUGIN_LOADER_HPP_
#define _ML_PLUGIN_LOADER_HPP_

#include <ML/Engine/PluginAPI.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	struct ML_ENGINE_API PluginLoader final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PluginLoader();
		explicit PluginLoader(const String & filename);
		~PluginLoader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PluginLoader & initLibraries();
		PluginLoader & initPlugins(EventSystem * eventSystem);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto filename()	const -> const String &	{ return m_filename; }
		inline auto config()	const -> const List<String> & { return m_config; }
		inline auto libs()		const -> const List<SharedLibrary *> & { return m_libs; }
		inline auto plugins()	const -> const List<Plugin *> & { return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String					m_filename;
		List<String>			m_config;
		List<SharedLibrary *>	m_libs;
		List<Plugin *>			m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_LOADER_HPP_