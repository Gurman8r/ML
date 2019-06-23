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
		if (!m_data.empty())
		{
			for (auto & tp : m_data)
			{
				if (!tp.second.empty())
				{
					for (auto & pair : tp.second)
					{
						if (pair.second)
						{
							delete pair.second;
							pair.second = nullptr;
						}
					}
					tp.second.clear();
				}
			}
			m_data.clear();
		}
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

	bool Content::readMetadata(Metadata & data, istream & file, String & line) const
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
		switch (hash()(md.getData("type").asString()))
		{
			case hash()("manifest")				: return loadFromFile(md.getData("file"));
			case AssetImporter<	CubeMap	>::ID	: return AssetImporter<	CubeMap	>()(md);
			case AssetImporter<	Entity	>::ID	: return AssetImporter<	Entity	>()(md);
			case AssetImporter<	Font	>::ID	: return AssetImporter<	Font	>()(md);
			case AssetImporter<	Image	>::ID	: return AssetImporter<	Image	>()(md);
			case AssetImporter<	Material>::ID	: return AssetImporter<	Material>()(md);
			case AssetImporter<	Mesh	>::ID	: return AssetImporter<	Mesh	>()(md);
			case AssetImporter<	Model	>::ID	: return AssetImporter<	Model	>()(md);
			case AssetImporter<	Script	>::ID	: return AssetImporter<	Script	>()(md);
			case AssetImporter<	Shader	>::ID	: return AssetImporter<	Shader	>()(md);
			case AssetImporter<	Sound	>::ID	: return AssetImporter<	Sound	>()(md);
			case AssetImporter<	Sprite	>::ID	: return AssetImporter<	Sprite	>()(md);
			case AssetImporter<	Surface	>::ID	: return AssetImporter<	Surface	>()(md);
			case AssetImporter<	Texture	>::ID	: return AssetImporter<	Texture	>()(md);
			case AssetImporter<	Uniform	>::ID	: return AssetImporter<	Uniform	>()(md);
			default:
				return false;
		}

	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}