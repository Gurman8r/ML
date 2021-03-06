#include <ML/Engine/ContentImporter.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Engine.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderTexture.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentImporter<>::loadMetadata(Metadata const & value)
	{
#if (ML_DEBUG)
		if (!([&]()
		{
			Debug::logInfo("Loading [{0}]: {1}",
				value.getData("type").asString(),
				value.getData("name").asString()
			);
#endif
			switch (value.getData("type").asString().hash())
			{
			case Hash("Manifest")			: return true;
			case typeof<Entity>::hash()		: return (bool)ContentImporter<Entity>()(value);
			case typeof<Font>::hash()			: return (bool)ContentImporter<Font>()(value);
			case typeof<Image>::hash()		: return (bool)ContentImporter<Image>()(value);
			case typeof<Material>::hash()		: return (bool)ContentImporter<Material>()(value);
			case typeof<Model>::hash()		: return (bool)ContentImporter<Model>()(value);
			case typeof<Script>::hash()		: return (bool)ContentImporter<Script>()(value);
			case typeof<Shader>::hash()		: return (bool)ContentImporter<Shader>()(value);
			case typeof<Sound>::hash()		: return (bool)ContentImporter<Sound>()(value);
			case typeof<Sprite>::hash()		: return (bool)ContentImporter<Sprite>()(value);
			case typeof<RenderTexture>::hash(): return (bool)ContentImporter<RenderTexture>()(value);
			case typeof<Texture>::hash()		: return (bool)ContentImporter<Texture>()(value);
			} 
			return false;
#if (ML_DEBUG)
		})())
		{
			return Debug::logError("Failed Loading [{0}]: {1}",
				value.getData("type").asString(),
				value.getData("name").asString()
			);
		}
		return true;
#endif
	}

	int32_t ContentImporter<>::loadMetadata(std::vector<Metadata> const & value)
	{
#if (ML_DEBUG)
		Debug::logInfo("Preparing {0} items...", value.size());
		Timer t{ true };
#endif
		int32_t count{ 0 };
		for (auto const & elem : value)
		{
			count += loadMetadata(elem);
		}
#if (ML_DEBUG)
		t.stop();
		Debug::logInfo("Done. Operation completed in {0}s.\n", t.elapsed().count());
#endif
		return count;
	}

	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Entity * ContentImporter<Entity>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Entity>(name))
				{
					return ML_Engine.content().insert(name, new Entity{});
				}
			}
		}
		return nullptr;
	}


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Font * ContentImporter<Font>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Font>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Engine.content().insert(name, new Font{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Engine.content().insert(name, new Font{});
					}
				}
			}
		}
		return nullptr;
	}


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Image * ContentImporter<Image>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Image>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Engine.content().insert(name,
							new Image(ML_FS.pathTo(file), md.getData("flip_v", true))
						);
					}
					else
					{
						return ML_Engine.content().insert(name, new Image{});
					}
				}
			}
		}
		return nullptr;
	}


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Material * ContentImporter<Material>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Material>(name))
				{
					// New Material
					auto temp = new Material {};

					// Default Uniforms
					if (md.getData("defaults", true))
					{
						for (auto const & pair : ML_Engine.content().data<Uniform>())
						{
							if (auto u { (Uniform const *)pair.second })
							{
								temp->insert(u->clone());
							}
						}
					}

					// Load Uniforms
					if (!temp->loadFromFile(
						md.getData("uniforms").asString(),
						(const std::map<String, Texture *> *)&ML_Engine.content().data<Texture>()
					))
					{
						/* error */
					}

					return ML_Engine.content().insert<Material>(name, temp);
				}
			}
		}
		return nullptr;
	}


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Model * ContentImporter<Model>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Model>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Engine.content().insert(name, new Model{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Engine.content().insert(name, new Model{});
					}
				}
			}
		}
		return nullptr;
	}


	// RenderTexture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	RenderTexture * ContentImporter<RenderTexture>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name{ md.getData("name") })
			{
				if (!ML_Engine.content().get<RenderTexture>(name))
				{
					return ML_Engine.content().create<RenderTexture>(name,
						ML_Engine.content().get<Model>(md.getData("model")),
						ML_Engine.content().get<Material>(md.getData("material")),
						ML_Engine.content().get<Shader>(md.getData("shader"))
					);
				}
			}
		}
		return nullptr;
	}


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Script * ContentImporter<Script>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Script>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Engine.content().insert(name, new Script{ ML_FS.pathTo(file) });
					}
					else
					{
						return ML_Engine.content().insert(name, new Script{});
					}
				}
			}
		}
		return nullptr;
	}


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Shader * ContentImporter<Shader>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Shader>(name))
				{
					if (const String file { md.getData("file") })
					{
						return ML_Engine.content().insert(name, new Shader{ ML_FS.pathTo(file) });
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
								return ML_Engine.content().insert(name, temp);
							}
							delete temp;
						}
						else if (vert && frag)
						{
							auto temp = new Shader();
							if (temp->loadFromFile(vert, frag))
							{
								return ML_Engine.content().insert(name, temp);
							}
							delete temp;
						}
						else
						{
							return ML_Engine.content().insert(name, new Shader{});
						}
					}
				}
			}
		}
		return nullptr;
	}


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sound * ContentImporter<Sound>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Sound>(name))
				{
					if (const String file { md.getData("file") })
					{
						auto temp = new Sound();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Engine.content().insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Engine.content().insert(name, new Sound{});
					}
				}
			}
		}
		return nullptr;
	}


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sprite * ContentImporter<Sprite>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Sprite>(name))
				{
					if (const String file = md.getData("texture"))
					{
						if (Texture const * tex = ML_Engine.content().get<Texture>(file))
						{
							auto temp = new Sprite();
							if (temp->loadFromMemory(tex))
							{
								return ML_Engine.content().insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Engine.content().insert(name, new Sprite{});
					}
				}
			}
		}
		return nullptr;
	}


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Texture * ContentImporter<Texture>::operator()(Metadata const & md) const
	{
		if (this->info.hash == md.getData("type").asString().hash())
		{
			if (const String name { md.getData("name") })
			{
				if (!ML_Engine.content().get<Texture>(name))
				{
					if (Texture const * copy { ML_Engine.content().get<Texture>(md.getData("copy")) })
					{
						return ML_Engine.content().create<Texture>(name, (*copy));
					}

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
							if (!temp->loadFromFile(ML_FS.pathTo(file)))
							{
								/* error */
							}
							return ML_Engine.content().insert<Texture>(name, temp);
						}
						else if (const String file = md.getData("image"))
						{
							if (Image const * img = ML_Engine.content().get<Image>(file))
							{
								auto temp = new Texture {
									sampler, format, smooth, repeat, mipmap, level, pixfmt
								};
								if (!temp->loadFromImage(*img))
								{
									/* error */
								}
								return ML_Engine.content().insert(name, temp);
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
							if (!temp->loadFromFaces({
								&Image{ names[0] },
								&Image{ names[1] },
								&Image{ names[2] },
								&Image{ names[3] },
								&Image{ names[4] },
								&Image{ names[5] },
							}))
							{
								/* error */
							}
							return ML_Engine.content().insert(name, temp);
						}
						break;
						/* * * * * * * * * * * * * * * * * * * * */
						case Hash("images"):
						{
							auto temp = new Texture {
								sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (!temp->loadFromFaces({
								ML_Engine.content().get<Image>(names[0]),
								ML_Engine.content().get<Image>(names[1]),
								ML_Engine.content().get<Image>(names[2]),
								ML_Engine.content().get<Image>(names[3]),
								ML_Engine.content().get<Image>(names[4]),
								ML_Engine.content().get<Image>(names[5]),
							}))
							{
								/* error */
							}
							return ML_Engine.content().insert(name, temp);
						}
						break;
						/* * * * * * * * * * * * * * * * * * * * */
						case Hash("textures"):
						{
							Array<Texture const *, 6> tex {
								ML_Engine.content().get<Texture>(names[0]),
								ML_Engine.content().get<Texture>(names[1]),
								ML_Engine.content().get<Texture>(names[2]),
								ML_Engine.content().get<Texture>(names[3]),
								ML_Engine.content().get<Texture>(names[4]),
								ML_Engine.content().get<Texture>(names[5]),
							};
							Array<Image, 6> img {};
							for (size_t i = 0; i < 6; i++)
							{
								img[i] = (tex[i] ? tex[i]->copyToImage() : Image::Default);
							}
							auto temp = new Texture {
								sampler, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (!temp->loadFromFaces(img))
							{
								/* error */
							}
							return ML_Engine.content().insert(name, temp);
						}
						break;
						}
						/* * * * * * * * * * * * * * * * * * * * */
						
					}
					break;
					/* * * * * * * * * * * * * * * * * * * * */
					}

					return ML_Engine.content().insert(name, new Texture{});
				}
			}
		}
		return nullptr;
	}
}