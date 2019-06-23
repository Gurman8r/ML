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
		if (md.getData("type").asString() == "manifest")
		{
			return loadFromFile(md.getData("name"));
		}
		else if (auto temp = AssetImporter<	CubeMap	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Entity	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Font	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Image	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Material>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Mesh	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Model	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Script	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Shader	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Sound	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Sprite	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Surface	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Texture	>()(md)) { return temp; }
		else if (auto temp = AssetImporter<	Uniform	>()(md)) { return temp; }
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}