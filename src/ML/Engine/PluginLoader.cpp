#include <ML/Engine/PluginLoader.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PluginLoader::PluginLoader(EventSystem & eventSystem)
		: m_eventSystem	(eventSystem)
		, m_path		()
		, m_files		()
		, m_libraries	()
		, m_plugins		()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PluginLoader::dispose()
	{
		for (auto & plugin : m_plugins) delete plugin;
		for (auto & library : m_libraries) delete library;
		m_plugins.clear();
		m_libraries.clear();
		m_files.clear();
		return m_files.empty() && m_plugins.empty() && m_libraries.empty();
	}
	
	bool PluginLoader::loadFromFile(const String & filename)
	{
		// Load Filenames
		if (std::ifstream file { filename })
		{
			String line;
			while (std::getline(file, line))
			{
				if (!line || (line.trim().front() == '#'))
					continue;

				const String file_path { ML_FS.pathTo(line
					.replaceAll("$(Configuration)", ML_CONFIGURATION)
					.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
				) };

				if (std::find(m_files.begin(), m_files.end(), file_path) == m_files.end())
				{
					m_files.push_back(line);
				}
			}
			file.close();
			return !m_files.empty();
		}
		return false;
	}

	size_t PluginLoader::loadLibraries()
	{
		if (!m_files.empty() && m_libraries.empty())
		{
			m_libraries.reserve(m_files.size());
			for (size_t i = 0; i < m_files.size(); i++)
			{
				// Load Library
				m_libraries.push_back(new SharedLibrary(m_files[i]));
			}
		}
		return m_libraries.size();
	}

	size_t PluginLoader::loadPlugins()
	{
		if (!m_files.empty() && !m_libraries.empty() && m_plugins.empty())
		{
			m_plugins.reserve(m_libraries.size());
			for (size_t i = 0; i < m_libraries.size(); i++)
			{
				// Load Plugin
				if (Plugin * plugin = m_libraries[i]->callFunction<Plugin *>(
					ML_TOSTRING(ML_Plugin_Main), m_eventSystem
				))
				{
					Debug::log("Loaded Plugin: \'{0}\'", m_files[i]);
					m_plugins.push_back(plugin);
				}
				else
				{
					Debug::logError("Failed Loading Plugin: \'{0}\'", m_files[i]);
				}
			}
		}
		return m_plugins.size();
	}

	bool PluginLoader::loadOneShot(const String & filename)
	{
		if (!filename || (filename.trim().front() == '#'))
			return false;

		// Load File
		const String file_path { ML_FS.pathTo(filename
			.replaceAll("$(Configuration)", ML_CONFIGURATION)
			.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
		) };
		if (std::find(m_files.begin(), m_files.end(), file_path) == m_files.end())
		{
			m_files.push_back(file_path);
		}

		// Load Library
		m_libraries.push_back(new SharedLibrary(m_files.back()));

		// Load Plugin
		if (Plugin * plugin = m_libraries.back()->callFunction<Plugin *>(
			ML_TOSTRING(ML_Plugin_Main), m_eventSystem
			))
		{
			m_plugins.push_back(plugin);
		}
		else
		{
			return Debug::logError("Failed Loading Plugin: \'{0}\'", m_files.back());
		}
		return true;
	}

	size_t PluginLoader::loadList(const std::vector<std::string>& value)
	{
		size_t count { 0 };
		for (const auto & filename : value)
		{
			count += loadOneShot(filename);
		}
		return count;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}