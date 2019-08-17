#include <ML/Engine/AssetImporter.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Asset.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>


namespace ml
{
	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Entity * EntityAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Entity>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Entity();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Entity());
					}
				}
			}
		}
		return nullptr;
	}


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Font * FontAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Font>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Font();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Font());
					}
				}
			}
		}
		return nullptr;
	}


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Image * ImageAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Image>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Image();

						bool flipV = md.getData("flip_v", true);

						if (temp->loadFromFile(ML_FS.pathTo(file), flipV))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Image());
					}
				}
			}
		}
		return nullptr;
	}


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Material * MaterialAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Material>(name))
				{
					// New Material
					auto temp = new Material { 
						ML_Content.get<Shader>(md.getData("shader")) // Shader
					};

					// Default Uniforms
					if (md.getData("defs", true))
					{
						if (auto u = ML_Content.get<uni_vec2_ptr>("CURSOR_POS"))
							temp->add(u->clone());

						if (auto u = ML_Content.get<uni_float_ptr>("DELTA_TIME"))
							temp->add(u->clone());

						if (auto u = ML_Content.get<uni_int_ptr>("FRAME_COUNT"))
							temp->add(u->clone());

						if (auto u = ML_Content.get<uni_float_ptr>("FRAME_RATE"))
							temp->add(u->clone());

						if (auto u = ML_Content.get<uni_float_ptr>("TOTAL_TIME"))
							temp->add(u->clone());

						if (auto u = ML_Content.get<uni_vec2_ptr>("VIEWPORT"))
							temp->add(u->clone());
					}

					// Load Uniforms
					using TexTree = Tree<String, Texture *>;
					if (!temp->loadFromFile(
						md.getData("file").asString(),
						reinterpret_cast<const TexTree *>(&ML_Content.data<Texture>())
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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Model * ModelAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Model>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Model();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else if (const String file = md.getData("mesh"))
					{
						if (const Mesh * mesh = ML_Content.get<Mesh>(file))
						{
							auto temp = new Model();
							if (temp->loadFromMemory(*mesh))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Content.insert(name, new Model());
					}
				}
			}
		}
		return nullptr;
	}


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Script * ScriptAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Script>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Script();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Script());
					}
				}
			}
		}
		return nullptr;
	}


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Shader * ShaderAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Shader>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Shader();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
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
							return ML_Content.insert(name, new Shader());
						}
					}
				}
			}
		}
		return nullptr;
	}


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sound * SoundAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Sound>(name))
				{
					if (const String file = md.getData("file"))
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
						return ML_Content.insert(name, new Sound());
					}
				}
			}
		}
		return nullptr;
	}


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Sprite * SpriteAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
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
						return ML_Content.insert(name, new Sprite());
					}
				}
			}
		}
		return nullptr;
	}


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Surface * SurfaceAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Surface>(name))
				{
					auto temp = new Surface {
						ML_Content.get<Model>(md.getData("model")),
						ML_Content.get<Shader>(md.getData("shader"))
					};
					if (!temp->create({
						md.getData("width", 1920),
						md.getData("height", 1080)
						}, GL::ColorAttachment0
					))
					{
						/* error */
					}
					return ML_Content.insert(name, temp);
				}
			}
		}
		return nullptr;
	}


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Texture * TextureAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
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
						{ "green",			GL::Green },
						{ "blue",			GL::Blue },
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
						if (const String file = md.getData("file"))
						{
							auto temp = new Texture {
								sampler, format, format, smooth, repeat, mipmap, level, pixfmt
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
								sampler, format, format, smooth, repeat, mipmap, level, pixfmt
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

						if (source == "images")
						{
							auto temp = new Texture {
								sampler, format, format, smooth, repeat, mipmap, level, pixfmt
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
						else if (source == "files")
						{
							auto temp = new Texture {
								sampler, format, format, smooth, repeat, mipmap, level, pixfmt
							};
							if (temp->loadFromFaces({
								&Image(names[0]),
								&Image(names[1]),
								&Image(names[2]),
								&Image(names[3]),
								&Image(names[4]),
								&Image(names[5]),
								}))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					break;
					/* * * * * * * * * * * * * * * * * * * * */
					}

					return ML_Content.insert(name, new Texture());
				}
			}
		}
		return nullptr;
	}
}