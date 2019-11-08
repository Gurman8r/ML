#include <ML/Engine/MetadataParser.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Engine/ContentImporter.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MetadataParser::MetadataParser() : m_lists() {}

	MetadataParser::MetadataParser(const List<Metadata *> & data)
		: m_lists(data)
	{
	}

	MetadataParser::~MetadataParser() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool MetadataParser::dispose()
	{
		for (auto & data : m_lists) { delete data; }
		m_lists.clear();
		return m_lists.empty();
	}

	bool MetadataParser::loadFromFile(const String & filename)
	{
		return this->dispose() && readFile(filename, m_lists);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const List<Metadata *> & MetadataParser::loadLists(List<Metadata *> & data, const List<Map<String, String>>& value)
	{
		for (const auto & elem : value)
		{
			Metadata * md = new Metadata();

			for (const auto & pair : elem)
			{
				md->setData(pair.first, pair.second);

				data.push_back(md);
			}
		}
		return data;
	}

	bool MetadataParser::readFile(const String & filename, List<Metadata *>& list)
	{
		SStream file;
		String	line;
		return ML_FS.getFileContents(filename, file) && readLists(list, file, line);
	}

	bool MetadataParser::readLists(List<Metadata *>& list, std::istream & file, String & line)
	{
		while (std::getline(file, line))
		{
			Metadata * data = nullptr;
			if (readMetadata(data, file, line))
			{
				// Load Manifest
				if (data->getData("type").asString() == "manifest")
				{
					readFile(data->getData("file").asString(), list);
				}
				list.push_back(data);
			}
		}
		return !list.empty();
	}

	bool MetadataParser::readMetadata(Metadata *& data, std::istream & file, String & line)
	{
		if ((!data) &&
			(line) &&
			(line.trim().front() != '#') &&
			(line.find("<import>") != String::npos))
		{
			data = new Metadata();
			while (std::getline(file, line))
			{
				line.replace_all("$(Configuration)", ML_CONFIGURATION);
				line.replace_all("$(PlatformTarget)", ML_PLATFORM_TARGET);

				size_t i;
				if ((i = line.find("</import>")) != String::npos)
				{
					return data; // Done
				}
				else if ((i = line.find_first_of("=")) != String::npos)
				{
					// Key
					if (const String key = String { line.substr(0, i) }.trim())
					{
						// Value
						if (const String val = String { line.substr(
							(i + 1), (line.size() - i - 2)
						) }.trim())
						{
							data->setData(key, val);
						}
					}
				}
			}
		}
		if (data) { delete data; }
		return (data = nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}