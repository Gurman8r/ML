#include <ML/Engine/Resources.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Resources::Resources()
		: effects	("Effects"	)
		, entities	("Entities"	)
		, fonts		("Fonts"	)
		, images	("Images"	)
		, lua		("Lua"		)
		, materials	("Materials")
		, meshes	("Meshes"	)
		, models	("Models"	)
		, plugins	("Plugins"	)
		, scripts	("Scripts"	)
		, shaders	("Shaders"	)
		, skyboxes	("Skyboxes"	)
		, sounds	("Sounds"	)
		, sprites	("Sprites"	)
		, textures	("Textures"	)
	{
	}

	Resources::~Resources()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t Resources::cleanupAll()
	{
		return
			lua.clean()			+
			effects.clean()		+
			sprites.clean()		+
			materials.clean()	+
			models.clean()		+
			meshes.clean()		+
			shaders.clean()		+
			skyboxes.clean()	+
			textures.clean()	+
			images.clean()		+
			fonts.clean()		+
			entities.clean()	+
			sounds.clean()		+
			scripts.clean()		+
			plugins.clean()		;
	}

	size_t Resources::reloadAll()
	{
		return
			lua.reload()		+
			fonts.reload()		+
			images.reload()		+
			materials.reload()	+
			meshes.reload()		+
			models.reload()		+
			shaders.reload()	+
			skyboxes.reload()	+
			sprites.reload()	+
			textures.reload()	+
			effects.reload()	+
			entities.clean()	+
			sounds.reload()		+
			scripts.reload()	+
			plugins.reload()	;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Resources::dispose()
	{
		return cleanupAll();
	}

	bool Resources::loadFromFile(const String & filename)
	{
		SStream file;
		return ML_FS.getFileContents(filename, file) && parseFile(file);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Resources::parseFile(SStream & file)
	{
		size_t count = 0;
		String line;
		while (std::getline(file, line))
		{
			if (line.trim().front() == '#')
			{
				continue;
			}
			else if (line.find("<item>") != String::npos)
			{
				ManifestItem item;

				while (std::getline(file, line))
				{
					line.replaceAll("$(Configuration)", ML_CONFIGURATION);
					line.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET);

					if (line.find("</item>") != String::npos)
					{
						count += parseItem(item);
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
									item[key] = val;
								}
							}
						}
					}
				}
			}
		}
		return (bool)(count);
	}

	bool Resources::parseItem(const ManifestItem & item)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		const String type = item.getStr("type");
		const String name = item.getStr("name");
		if (type && name)
		{
			// Manifests
			/* * * * * * * * * * * * * * * * * * * * */
			if (type == "manifest")
			{
				return loadFromFile(name);
			}
			// Effects
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "effect")
			{
				const String m = item.getStr("model");
				const String s = item.getStr("shader");
				const int32_t w = item.getInt("width", 1920);
				const int32_t h = item.getInt("height", 1080);
				if (m && s)
				{
					Effect * e;
					return
						(e = effects.load(name)) &&
						(e->create({ w, h }, GL::ColorAttachment0)) &&
						(e->setModel(models.get(m))) &&
						(e->setShader(shaders.get(s)));
				}
				else
				{
					return effects.load(name);
				}
			}
			// Entities
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "entity")
			{
				return entities.load(name);
			}
			// Fonts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "font")
			{
				if (const String file = item.getStr("file"))
				{
					return fonts.load(name, file);
				}
				else
				{
					return fonts.load(name);
				}
			}
			// Images
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "image")
			{
				if (const String file = item.getStr("file"))
				{
					return images.load(name, file);
				}
				else
				{
					return images.load(name);
				}
			}
			// Lua
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "lua")
			{
				if (const String file = item.getStr("file"))
				{
					return lua.load(name, file);
				}
				else
				{
					return lua.load(name);
				}
			}
			// Material
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "material")
			{
				if (const String file = item.getStr("file"))
				{
					return materials.load(name, file);
				}
				else
				{
					return materials.load(name);
				}
			}
			// Mesh
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "mesh")
			{
				if (const String file = item.getStr("file"))
				{
					return meshes.load(name, file);
				}
				else
				{
					return meshes.load(name);
				}
			}
			// Models
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "model")
			{
				if (const String file = item.getStr("file"))
				{
					return models.load(name, file);
				}
				else if (const String file = item.getStr("mesh"))
				{
					const Mesh * temp;
					return
						(models.load(name)) &&
						(temp = meshes.get(file)) &&
						(models.get(name)->loadFromMemory(*temp));
				}
				else
				{
					return models.load(name);
				}
			}
			// Plugins
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "plugin")
			{
				if (const String file = item.getStr("file"))
				{
					return plugins.load(name, file);
				}
				else
				{
					return plugins.load(name);
				}
			}
			// Scripts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "script")
			{
				if (const String file = item.getStr("file"))
				{
					return scripts.load(name, file);
				}
				else
				{
					return scripts.load(name);
				}
			}
			// Shaders
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "shader")
			{
				if (const String file = item.getStr("file"))
				{
					return shaders.load(name, file);
				}
				else
				{
					const String vert = item.getStr("vert");
					const String geom = item.getStr("geom");
					const String frag = item.getStr("frag");
					if (vert || geom || frag)
					{
						return
							shaders.load(name) &&
							shaders.get(name)->loadFromFile(vert, geom, frag);
					}
					else
					{
						return shaders.load(name);
					}
				}
			}
			// Skyboxes
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "skybox")
			{
				if (const String file = item.getStr("file"))
				{
					return skyboxes.load(name, file);
				}
				else
				{
					return skyboxes.load(name);
				}
			}
			// Sounds
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sound")
			{
				if (const String file = item.getStr("file"))
				{
					return sounds.load(name, file);
				}
				else
				{
					return sounds.load(name);
				}
			}
			// Sprites
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sprite")
			{
				if (const String file = item.getStr("texture"))
				{
					const Texture * temp;
					return
						(sprites.load(name)) &&
						(temp = textures.get(file)) &&
						(sprites.get(name)->loadFromMemory(temp));
				}
				else
				{
					return sprites.load(name);
				}
			}
			// Textures
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "texture")
			{
				if (const String file = item.getStr("file"))
				{
					return textures.load(name, file);
				}
				else if (const String file = item.getStr("image"))
				{
					const Image * temp;
					return
						(textures.load(name)) &&
						(temp = images.get(file)) &&
						(textures.get(name)->loadFromImage(*temp));
				}
				else
				{
					return textures.load(name);
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}