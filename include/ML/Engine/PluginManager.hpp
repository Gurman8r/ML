#ifndef _ML_PLUGIN_LOADER_HPP_
#define _ML_PLUGIN_LOADER_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/SharedLibrary.hpp>

#define ML_Plugins ::ml::PluginManager::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Used to load and store plugin instances
	// Stages broken out into separate functions
	struct ML_ENGINE_API PluginManager final : public Singleton<PluginManager>, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit PluginManager();
		
		~PluginManager() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		bool loadFromFile(const String & filename);
		
		size_t loadLibraries();

		size_t loadPlugins();

		bool loadOneShot(const String & filename);

		size_t loadList(const std::vector<std::string> & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto path()		const -> const String &					{ return m_path; }
		inline auto files()		const -> const List<String> &			{ return m_files; }
		inline auto libraries()	const -> const List<SharedLibrary *> &	{ return m_libraries; }
		inline auto plugins()	const -> const List<Plugin *> &			{ return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String					m_path;
		List<String>			m_files;
		List<SharedLibrary *>	m_libraries;
		List<Plugin *>			m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_LOADER_HPP_