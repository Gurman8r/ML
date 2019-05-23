#include <ML/Engine/Resources.hpp>
#include <ML/Core/Debug.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Skybox.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Resources::Resources()
		: entities	("Entities"	)
		, fonts		("Fonts"	)
		, images	("Images"	)
		, materials	("Materials")
		, meshes	("Meshes"	)
		, models	("Models"	)
		, scripts	("Scripts"	)
		, shaders	("Shaders"	)
		, skyboxes	("Skyboxes"	)
		, sounds	("Sounds"	)
		, sprites	("Sprites"	)
		, surfaces	("Surfaces"	)
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
			surfaces.clean()	+
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
			scripts.clean()		;
	}

	size_t Resources::reloadAll()
	{
		return
			fonts.reload()		+
			images.reload()		+
			materials.reload()	+
			meshes.reload()		+
			models.reload()		+
			shaders.reload()	+
			skyboxes.reload()	+
			sprites.reload()	+
			textures.reload()	+
			surfaces.reload()	+
			entities.clean()	+
			sounds.reload()		+
			scripts.reload()	;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Resources::dispose()
	{
		return cleanupAll();
	}

	bool Resources::loadFromFile(const String & filename)
	{
		size_t count = 0;
		SStream file;
		if (ML_FS.getFileContents(filename, file))
		{
			String line;
			while (std::getline(file, line))
			{
				ManifestItem item;
				if (item.loadValues(file, line))
				{
					count += parseItem(item);
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
			// Surfaces
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "surface")
			{
				const String m = item.getStr("model");
				const String s = item.getStr("shader");
				const int32_t w = item.getInt("width", 1920);
				const int32_t h = item.getInt("height", 1080);
				if (m && s)
				{
					Surface * e;
					return
						(e = surfaces.load(name)) &&
						(e->create({ w, h }, GL::ColorAttachment0)) &&
						(e->setModel(models.get(m))) &&
						(e->setShader(shaders.get(s)));
				}
				else
				{
					return surfaces.load(name);
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