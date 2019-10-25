#include <ML/Engine/ContentImporter.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Ref.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>


/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Entity * ContentImporter<Entity>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Entity>(name))
				{
					return ML_Content.insert(name, new Entity{});
				}
			}
		}
		return nullptr;
	}


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Font * ContentImporter<Font>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Font>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Content.insert(name, new Font{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Content.insert(name, new Font{});
					}
				}
			}
		}
		return nullptr;
	}


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Image * ContentImporter<Image>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Image>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Content.insert(name,
							new Image(ML_FS.pathTo(file), md.getData("flip_v", true))
						);
					}
					else
					{
						return ML_Content.insert(name, new Image{});
					}
				}
			}
		}
		return nullptr;
	}


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Material * ContentImporter<Material>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Material>(name))
				{
					// New Material
					auto temp = new Material {
						//ML_Content.get<Shader>(md.getData("shader")) // Shader
					};

					// Default Uniforms
					if (md.getData("defaults", true))
					{
						for (const auto & pair : ML_Content.data<Uniform>())
						{
							if (auto u { (const Uniform *)pair.second })
							{
								temp->insert(u->clone());
							}
						}
					}

					// Load Uniforms
					if (!temp->loadFromFile(
						md.getData("uniforms").asString(),
						(const Map<String, Texture *> *)&ML_Content.data<Texture>()
					))
					{
						/* error */
					}

					return ML_Content.insert<Material>(name, temp);
				}
			}
		}
		return nullptr;
	}


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Model * ContentImporter<Model>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Model>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Content.insert(name, new Model{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Content.insert(name, new Model{});
					}
				}
			}
		}
		return nullptr;
	}


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Script * ContentImporter<Script>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Script>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Content.insert(name, new Script{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Content.insert(name, new Script{});
					}
				}
			}
		}
		return nullptr;
	}


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Shader * ContentImporter<Shader>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Shader>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Content.insert(name, new Shader{ ML_FS.pathTo(file) });
					}
					else
					{
						const String vert = md.getData("vert");
						const String geom = md.getData("geom");
						const String frag = md.getData("frag");
						if (vert && geom && frag)
						{
							auto temp = new Shader();
							if (temp->loadFromFile(vert, geom, frag))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						else if (vert && frag)
						{
							auto temp = new Shader();
							if (temp->loadFromFile(vert, frag))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						else
						{
							return ML_Content.insert(name, new Shader{});
						}
					}
				}
			}
		}
		return nullptr;
	}


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sound * ContentImporter<Sound>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Sound>(name))
				{
					if (const String file { md.getData("file") })
					{
						auto temp = new Sound();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Sound{});
					}
				}
			}
		}
		return nullptr;
	}


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sprite * ContentImporter<Sprite>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Sprite>(name))
				{
					if (const String file = md.getData("texture"))
					{
						if (const Texture * tex = ML_Content.get<Texture>(file))
						{
							auto temp = new Sprite();
							if (temp->loadFromMemory(tex))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Content.insert(name, new Sprite{});
					}
				}
			}
		}
		return nullptr;
	}


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Surface * ContentImporter<Surface>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Surface>(name))
				{
					return ML_Content.create<Surface>(name, 
						ML_Content.get<Model>(md.getData("model")),
						ML_Content.get<Material>(md.getData("material")),
						ML_Content.get<Shader>(md.getData("shader"))
					);
				}
			}
		}
		return nullptr;
	}


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Texture * ContentImporter<Texture>::operator()(const Metadata & md) const
	{
		if (info().hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Content.get<Texture>(name))
				{
					// Smooth
					const bool smooth = md.getData("smooth", true);

					// Repeat
					const bool repeat = md.getData("repeat", false);

					// Mipmap
					const bool mipmap = md.getData("mipmap", false);

					// Level
					const int32_t level = md.getData("level", 0);
				
					// Target
					const GL::Sampler sampler = md.getData("sampler", GL::Texture2D, {
						{ "texture_2d",		GL::Texture2D },
						{ "texture_3d",		GL::Texture3D },
						{ "texture_cube",	GL::TextureCubeMap },
					});
				
					// Internal / Color Format
					const GL::Format format = md.getData("format", GL::RGBA, {
						{ "red",			GL::Red	},
						{ "rgb",			GL::RGB	},
						{ "rgba",			GL::RGBA },
					});
				
					// Pixel Format
					const GL::Type pixfmt = md.getData("pixfmt", GL::UnsignedByte, {
						{ "byte",			GL::Byte },
						{ "ubyte",			GL::UnsignedByte },
						{ "short",			GL::Short },
						{ "ushort",			GL::UnsignedShort },
						{ "int",			GL::Int },
						{ "uint",			GL::UnsignedInt },
						{ "float",			GL::Float },
						{ "half_float",		GL::HalfFloat },
					});

					switch (sampler)
					{
					/* * * * * * * * * * * * * * * * * * * * */
					case GL::Texture2D:
					{
						if (const String file { md.getData("file") })
						{
							auto temp = new Texture {
							sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (temp->loadFromFile(ML_FS.pathTo(file)))
							{
								return ML_Content.insert<Texture>(name, temp);
							}
							delete temp;
						}
						else if (const String file = md.getData("image"))
						{
							if (const Image * img = ML_Content.get<Image>(file))
							{
								auto temp = new Texture {
								sampler, format, smooth, repeat, mipmap, level, pixfmt
								};
								if (temp->loadFromImage(*img))
								{
									return ML_Content.insert(name, temp);
								}
								delete temp;
							}
						}
					}
					break;
					/* * * * * * * * * * * * * * * * * * * * */
					case GL::Texture3D:
					{
					}
					break;
					/* * * * * * * * * * * * * * * * * * * * */
					case GL::TextureCubeMap:
					{
						const String source = md.getData("source").asString();

						const Array<String, 6> names =
						{
							md.getData("right"),
							md.getData("left"),
							md.getData("top"),
							md.getData("bottom"),
							md.getData("front"),
							md.getData("back"),
						};

						switch (util::to_lower(source).hash())
						{
						/* * * * * * * * * * * * * * * * * * * * */
						case Hash("files"):
						{
							auto temp = new Texture {
							sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (temp->loadFromFaces({
								&Image{ names[0] },
								&Image{ names[1] },
								&Image{ names[2] },
								&Image{ names[3] },
								&Image{ names[4] },
								&Image{ names[5] },
							}))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						break;
						/* * * * * * * * * * * * * * * * * * * * */
						case Hash("images"):
						{
							auto temp = new Texture {
							sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (temp->loadFromFaces({
								ML_Content.get<Image>(names[0]),
								ML_Content.get<Image>(names[1]),
								ML_Content.get<Image>(names[2]),
								ML_Content.get<Image>(names[3]),
								ML_Content.get<Image>(names[4]),
								ML_Content.get<Image>(names[5]),
							}))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						break;
						/* * * * * * * * * * * * * * * * * * * * */
						case Hash("textures"):
						{
							Array<const Texture *, 6> tex {
								ML_Content.get<Texture>(names[0]),
								ML_Content.get<Texture>(names[1]),
								ML_Content.get<Texture>(names[2]),
								ML_Content.get<Texture>(names[3]),
								ML_Content.get<Texture>(names[4]),
								ML_Content.get<Texture>(names[5]),
							};
							Array<Image, 6> img {};
							for (size_t i = 0; i < 6; i++)
							{
								img[i] = (tex[i] ? tex[i]->copyToImage() : Image::Default);
							}
							auto temp = new Texture {
							sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (temp->loadFromFaces(img))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						break;
						}
						/* * * * * * * * * * * * * * * * * * * * */
						
					}
					break;
					/* * * * * * * * * * * * * * * * * * * * */
					}

					return ML_Content.insert(name, new Texture{});
				}
			}
		}
		return nullptr;
	}
}