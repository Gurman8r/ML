#include <ML/Engine/Content.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/CubeMap.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Engine/Content.hpp>

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

				//ManifestItem item;
				//if (readItem(item, file, line))
				//{
				//	count += parseItem(item);
				//}
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

	bool Content::parseMetadata(const Metadata & data)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		const String type = data.getData("type");
		const String name = data.getData("name");
		if (type && name)
		{
			// Manifests
			/* * * * * * * * * * * * * * * * * * * * */
			if (type == "manifest")
			{
				return loadFromFile(name);
			}
			// Entities
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "entity")
			{
				return create<Entity>(name);
			}
			// Fonts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "font")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Font>(name, file);
				}
				else
				{
					return create<Font>(name);
				}
			}
			// Images
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "image")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Image>(name, file);
				}
				else
				{
					return create<Image>(name);
				}
			}
			// Material
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "material")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Material>(name, file);
				}
				else
				{
					return create<Material>(name);
				}
			}
			// Mesh
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "mesh")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Mesh>(name, file);
				}
				else
				{
					return create<Mesh>(name);
				}
			}
			// Models
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "model")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Model>(name, file);
				}
				else if (const String file = data.getData("mesh"))
				{
					const Mesh * temp;
					return
						(create<Model>(name)) &&
						(temp = get<Mesh>(file)) &&
						(get<Model>(name)->loadFromMemory(*temp));
				}
				else
				{
					return create<Model>(name);
				}
			}
			// Scripts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "script")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Script>(name, file);
				}
				else
				{
					return create<Script>(name);
				}
			}
			// Shaders
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "shader")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Shader>(name, file);
				}
				else
				{
					const String vert = data.getData("vert");
					const String geom = data.getData("geom");
					const String frag = data.getData("frag");

					if (vert && geom && frag)
					{
						if (auto temp = create<Shader>(name))
						{
							return temp->loadFromFile(vert, geom, frag);
						}
					}
					else if (vert && frag)
					{
						if (auto temp = create<Shader>(name))
						{
							return temp->loadFromFile(vert, frag);
						}
					}
					else
					{
						return create<Shader>(name);
					}
				}
			}
			// Sounds
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sound")
			{
				if (const String file = data.getData("file"))
				{
					return create_from_file<Sound>(name, file);
				}
				else
				{
					return create<Sound>(name);
				}
			}
			// Sprites
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sprite")
			{
				if (const String file = data.getData("texture"))
				{
					const Texture * temp;
					return
						(create<Sprite>(name)) &&
						(temp = get<Texture>(file)) &&
						(get<Sprite>(name)->loadFromMemory(temp));
				}
				else
				{
					return create<Sprite>(name);
				}
			}
			// Surfaces
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "surface")
			{
				const String  m = data.getData("model");
				const String  s = data.getData("shader");
				const int32_t w = data.getData("width", 1920);
				const int32_t h = data.getData("height", 1080);
				if (m && s)
				{
					Surface * e;
					return
						(e = create<Surface>(name)) &&
						(e->create({ w, h }, GL::ColorAttachment0)) &&
						(e->setModel(get<Model>(m))) &&
						(e->setShader(get<Shader>(s)));
				}
				else
				{
					return create<Surface>(name);
				}
			}
			// Textures
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "texture")
			{
				const bool smooth = data.getData("smooth", true);
				const bool repeat = data.getData("repeat", false);
				const bool mipmap = data.getData("mipmap", false);
				
				const int32_t level = data.getData("level", 0);
				
				const GL::Target target = data.getData("target", GL::Texture2D, 
					Map<String, GL::Target> {
					{ "texture_2d",	GL::Texture2D },
					{ "texture_3d",	GL::Texture3D },
				});
				
				const GL::Format format = data.getData("format", GL::RGBA, {
					{ "red",		GL::Red		},
					{ "green",		GL::Green	},
					{ "blue",		GL::Blue	},
					{ "rgb",		GL::RGB		},
					{ "rgba",		GL::RGBA	},
				});
				
				const GL::Type pix_ty = data.getData("pix_ty", GL::UnsignedByte, {
					{ "byte",		GL::Byte },
					{ "ubyte",		GL::UnsignedByte },
					{ "short",		GL::Short },
					{ "ushort",		GL::UnsignedShort },
					{ "int",		GL::Int },
					{ "uint",		GL::UnsignedInt },
					{ "float",		GL::Float },
					{ "half_float",	GL::HalfFloat },
				});
				
				if (const String file = data.getData("file"))
				{
					return create_from_file<Texture>(name, file,
						target, format, format, smooth, repeat, mipmap, level, pix_ty
					);
				}
				else if (const String file = data.getData("image"))
				{
					const Image * temp;
					return
						(create<Texture>(name,
							target, format, format, smooth, repeat, mipmap, level, pix_ty
							)) &&
							(temp = get<Image>(file)) &&
						(get<Texture>(name)->loadFromImage(*temp));
				}
				else
				{
					return create<Texture>(name);
				}
			}
			return Debug::log("Failed Loading: {0} | {1}", type, name);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}