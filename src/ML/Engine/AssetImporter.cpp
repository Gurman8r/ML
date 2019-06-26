#include <ML/Engine/AssetImporter.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Content.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/CubeMap.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	// CubeMap Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	CubeMap * CubeMapAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
					}
				}
			}
		}
		return nullptr;
	}


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Entity * EntityAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
					}
				}
			}
		}
		return nullptr;
	}


	// Mesh Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Mesh * MeshAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
					}
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else if (const String file = md.getData("mesh"))
					{
						if (const Mesh * mesh = ML_Content.get<Mesh>(file))
						{
							auto temp = new value_type();
							if (temp->loadFromMemory(*mesh))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
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
							auto temp = new value_type();
							if (temp->loadFromFile(vert, geom, frag))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						else if (vert && frag)
						{
							auto temp = new value_type();
							if (temp->loadFromFile(vert, frag))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
						else
						{
							return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new value_type();
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					if (const String file = md.getData("file"))
					{
						if (const Texture * tex = ML_Content.get<Texture>(file))
						{
							auto temp = new value_type();
							if (temp->loadFromMemory(tex))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Content.insert(name, new value_type());
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
				if (!ML_Content.get<value_type>(name))
				{
					const String  m = md.getData("model");
					const String  s = md.getData("shader");
					const int32_t w = md.getData("width", 1920);
					const int32_t h = md.getData("height", 1080);
					if (m && s)
					{
						auto temp = new value_type();
						if (temp->create({ w, h }, GL::ColorAttachment0) &&
							temp->setModel(ML_Content.get<Model>(m)) &&
							temp->setShader(ML_Content.get<Shader>(s)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new value_type());
					}
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
				if (!ML_Content.get<value_type>(name))
				{
					const bool smooth = md.getData("smooth", true);
					const bool repeat = md.getData("repeat", false);
					const bool mipmap = md.getData("mipmap", false);
				
					const int32_t level = md.getData("level", 0);
				
					const GL::Target target = md.getData("target", GL::Texture2D, 
						Map<String, GL::Target> {
						{ "texture_2d",	GL::Texture2D },
						{ "texture_3d",	GL::Texture3D },
					});
				
					const GL::Format format = md.getData("format", GL::RGBA, {
						{ "red",		GL::Red		},
						{ "green",		GL::Green	},
						{ "blue",		GL::Blue	},
						{ "rgb",		GL::RGB		},
						{ "rgba",		GL::RGBA	},
					});
				
					const GL::Type pix_ty = md.getData("pix_ty", GL::UnsignedByte, {
						{ "byte",		GL::Byte },
						{ "ubyte",		GL::UnsignedByte },
						{ "short",		GL::Short },
						{ "ushort",		GL::UnsignedShort },
						{ "int",		GL::Int },
						{ "uint",		GL::UnsignedInt },
						{ "float_t",		GL::Float },
						{ "half_float",	GL::HalfFloat },
					});

					if (const String file = md.getData("file"))
					{
						auto temp = new value_type(
							target, 
							format,
							format,
							smooth,
							repeat,
							mipmap,
							level, 
							pix_ty
						);
						if (temp->loadFromFile(ML_FS.getPathTo(file)))
						{
							return ML_Content.insert<Texture>(name, temp);
						}
						delete temp;
					}
					else if (const String file = md.getData("image"))
					{
						if (const Image * img = ML_Content.get<Image>(file))
						{
							auto temp = new value_type(
								target, 
								format,
								format,
								smooth,
								repeat,
								mipmap,
								level, 
								pix_ty
							);
							if (temp->loadFromImage(*img))
							{
								return ML_Content.insert(name, temp);
							}
							delete temp;
						}
					}
					else
					{
						return ML_Content.insert(name, new value_type());
					}
				}
			}
		}
		return nullptr;
	}


	// Uniform Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Uniform * UniformAssetImporter::operator()(const Metadata & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<value_type>(name))
				{
				}
			}
		}
		return nullptr;
	}
}