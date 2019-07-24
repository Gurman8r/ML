#include <ML/Engine/ContentLoader.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Engine/AssetImporter.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/CubeMap.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ContentLoader::ContentLoader() : m_list() {}

	ContentLoader::~ContentLoader() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentLoader::dispose()
	{
		for (auto & data : m_list) { delete data; }
		m_list.clear();
		return m_list.empty();
	}

	bool ContentLoader::loadFromFile(const String & filename)
	{
		if (this->dispose() && readFile(filename, m_list))
		{
			for (const MetaData * data : m_list)
			{
				if (!parseMetadata(*data)) { /* error */ }
			}
			return this->dispose();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentLoader::readFile(const String & filename, List<MetaData *>& list)
	{
		SStream file;
		String	line;
		return ML_FS.getFileContents(filename, file) && readLists(list, file, line);
	}

	bool ContentLoader::readLists(List<MetaData *>& list, Istream & file, String & line)
	{
		while (std::getline(file, line))
		{
			MetaData * data = nullptr;
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

	bool ContentLoader::readMetadata(MetaData *& data, Istream & file, String & line)
	{
		if ((!data) &&
			(line) &&
			(line.trim().front() != '#') &&
			(line.find("<import>") != String::npos))
		{
			data = new MetaData();
			while (std::getline(file, line))
			{
				line.replaceAll("$(Configuration)", ML_CONFIGURATION);
				line.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET);

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

	bool ContentLoader::parseMetadata(const MetaData & data)
	{
		switch (Hash()(data.getData("type").asString()))
		{
			case Hash()("manifest") : return true;

			case AssetImporter<CubeMap	>::id: return AssetImporter<CubeMap	>()(data);
			case AssetImporter<Entity	>::id: return AssetImporter<Entity	>()(data);
			case AssetImporter<Font		>::id: return AssetImporter<Font	>()(data);
			case AssetImporter<Image	>::id: return AssetImporter<Image	>()(data);
			case AssetImporter<Material	>::id: return AssetImporter<Material>()(data);
			case AssetImporter<Mesh		>::id: return AssetImporter<Mesh	>()(data);
			case AssetImporter<Model	>::id: return AssetImporter<Model	>()(data);
			case AssetImporter<Script	>::id: return AssetImporter<Script	>()(data);
			case AssetImporter<Shader	>::id: return AssetImporter<Shader	>()(data);
			case AssetImporter<Sound	>::id: return AssetImporter<Sound	>()(data);
			case AssetImporter<Sprite	>::id: return AssetImporter<Sprite	>()(data);
			case AssetImporter<Surface	>::id: return AssetImporter<Surface	>()(data);
			case AssetImporter<Texture	>::id: return AssetImporter<Texture	>()(data);
			case AssetImporter<Uniform	>::id: return AssetImporter<Uniform	>()(data);
		
			default:
				return Debug::logError("Failed Loading: [{0}] | \'{1}\'",
					data.getData("type").asString(),
					data.getData("name").asString()
				);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}