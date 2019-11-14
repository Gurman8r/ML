#ifndef _ML_PLUGIN_LOADER_HPP_
#define _ML_PLUGIN_LOADER_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/SharedLibrary.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Engine;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API PluginManager final : public Newable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		bool loadFromFile(const String & filename);
		
		size_t loadLibraries();

		size_t loadPlugins();

		bool loadOneShot(const String & filename);

		size_t loadList(const std::vector<std::string> & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto path()		const -> const String & { return m_path; }
		inline auto files()		const -> const List<String> & { return m_files; }
		inline auto libraries()	const -> const List<ptr_t<SharedLibrary>> &	{ return m_libraries; }
		inline auto plugins()	const -> const List<ptr_t<Plugin>> & { return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class Engine;
		
		PluginManager()
			: m_path{}
			, m_files{}
			, m_libraries{}
			, m_plugins{}
		{
		};
		
		~PluginManager() { this->dispose(); }

		String m_path;
		List<String> m_files;
		List<ptr_t<SharedLibrary>> m_libraries;
		List<ptr_t<Plugin>> m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_LOADER_HPP_