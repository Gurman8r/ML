#include <ML/Engine/PluginLoader.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PluginLoader::PluginLoader()
		: m_config	()
		, m_filename()
		, m_libs	()
	{
	}

	PluginLoader::PluginLoader(const String & filename)
		: PluginLoader()
	{
		this->loadFromFile(filename);
	}

	PluginLoader::~PluginLoader()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PluginLoader::dispose()
	{
		for (auto & plugin : m_plugins) delete plugin;
		for (auto & library : m_libs) delete library;
		m_plugins.clear();
		m_libs.clear();
		m_config.clear();
		return m_config.empty() && m_plugins.empty() && m_libs.empty();
	}
	
	bool PluginLoader::loadFromFile(const String & filename)
	{
		// Load Filenames
		if (Ifstream file { filename })
		{
			String line;
			while (std::getline(file, line))
			{
				if (!line || (line.trim().front() == '#'))
					continue;

				// Load Config
				m_config.push_back(ML_FS.pathTo(line
					.replaceAll("$(Configuration)", ML_CONFIGURATION)
					.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
				));
			}
			file.close();
			return !m_config.empty();
		}
		return false;
	}

	PluginLoader & PluginLoader::initLibraries()
	{
		// Load Libraries
		if (!m_config.empty() && m_libs.empty())
		{
			for (size_t i = 0; i < m_config.size(); i++)
			{
				m_libs.push_back(new SharedLibrary(m_config[i]));
			}
		}
		return (*this);
	}

	PluginLoader & PluginLoader::initPlugins(EventSystem * eventSystem)
	{
		// Load Plugins
		if (eventSystem && !m_config.empty() && !m_libs.empty())
		{
			for (size_t i = 0; i < m_libs.size(); i++)
			{
				if (auto plugin = m_libs[i]->callFunction<Plugin *>(
					ML_str(ML_Plugin_Main), (*eventSystem)
					))
				{
					Debug::log("Loaded Plugin: \'{0}\'", m_config[i]);
					m_plugins.push_back(plugin);
				}
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}