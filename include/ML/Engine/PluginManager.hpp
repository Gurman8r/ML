#ifndef _ML_PLUGIN_LOADER_HPP_
#define _ML_PLUGIN_LOADER_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/SharedLibrary.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API PluginManager final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();

		bool loadFromFile(String const & filename);
		
		size_t loadLibraries();

		size_t loadPlugins();

		bool loadOneShot(String const & filename);

		size_t loadList(const std::vector<std::string> & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto path()		const -> String const & { return m_path; }
		inline auto files()		const -> ArrayList<String> const & { return m_files; }
		inline auto libraries()	const -> ArrayList<SharedLibrary *> const &	{ return m_libraries; }
		inline auto plugins()	const -> ArrayList<Plugin *> const & { return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class Engine;

		PluginManager()
			: m_path{}
			, m_files{}
			, m_libraries{}
			, m_plugins{}
		{
		}

		~PluginManager() { this->dispose(); }

		String m_path;
		ArrayList<String> m_files;
		ArrayList<SharedLibrary *> m_libraries;
		ArrayList<Plugin *> m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_LOADER_HPP_