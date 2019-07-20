#include <ML/Engine/Content.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
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

	bool Content::dispose()
	{
		for (auto & asset_types : m_data)
		{
			for (auto & asset_data : asset_types.second)
			{
				if (asset_data.second)
				{
					// Delete the asset container
					delete asset_data.second;
					asset_data.second = nullptr;
				}
			}
			asset_types.second.clear();
		}
		m_data.clear();
		return m_data.empty();
	}

	bool Content::loadFromFile(const String & filename)
	{
		if (!filename) return false;

		size_t count = 0;
		SStream file;
		if (ML_FS.getFileContents(filename, file))
		{
			String line;
			while (std::getline(file, line))
			{
				Metadata data;
				if (readMetadata(data, file, line))
				{
					count += parseMetadata(data);
				}
			}
		}
		return (bool)(count);
	}

	bool Content::readMetadata(Metadata & data, Istream & file, String & line) const
	{
		if (line.empty() || (line.trim().front() == '#'))
		{
			return false;
		}

		if (line.find("<import>") != String::npos)
		{
			while (std::getline(file, line))
			{
				line.replaceAll("$(Configuration)", ML_CONFIGURATION);
				line.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET);

				if (line.find("</import>") != String::npos)
				{
					return true;
				}
				else
				{
					size_t i;
					if ((i = line.find_first_of("=")) != String::npos)
					{
						if (const String key = String(line.substr(0, i)).trim())
						{
							if (const String val = String(
								line.substr((i + 1), (line.size() - i - 2))).trim())
							{
								data.setData(key, val);
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool Content::parseMetadata(const Metadata & md)
	{
		switch (Hash()(md.getData("type").asString()))
		{
			case Hash()("manifest")				: return loadFromFile(md.getData("file"));
			case AssetImporter<	CubeMap	>::id	: return AssetImporter<	CubeMap	>()(md);
			case AssetImporter<	Entity	>::id	: return AssetImporter<	Entity	>()(md);
			case AssetImporter<	Font	>::id	: return AssetImporter<	Font	>()(md);
			case AssetImporter<	Image	>::id	: return AssetImporter<	Image	>()(md);
			case AssetImporter<	Material>::id	: return AssetImporter<	Material>()(md);
			case AssetImporter<	Mesh	>::id	: return AssetImporter<	Mesh	>()(md);
			case AssetImporter<	Model	>::id	: return AssetImporter<	Model	>()(md);
			case AssetImporter<	Script	>::id	: return AssetImporter<	Script	>()(md);
			case AssetImporter<	Shader	>::id	: return AssetImporter<	Shader	>()(md);
			case AssetImporter<	Sound	>::id	: return AssetImporter<	Sound	>()(md);
			case AssetImporter<	Sprite	>::id	: return AssetImporter<	Sprite	>()(md);
			case AssetImporter<	Surface	>::id	: return AssetImporter<	Surface	>()(md);
			case AssetImporter<	Texture	>::id	: return AssetImporter<	Texture	>()(md);
			case AssetImporter<	Uniform	>::id	: return AssetImporter<	Uniform	>()(md);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}