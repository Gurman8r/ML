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
#include <ML/Script/Script.hpp>
#include <ML/Core/Debug.hpp>


namespace ml
{
	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Entity * EntityAssetImporter::operator()(const MetaData & md) const
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
	Font * FontAssetImporter::operator()(const MetaData & md) const
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
	Image * ImageAssetImporter::operator()(const MetaData & md) const
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
						if (temp->loadFromFile(ML_FS.pathTo(file)))
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
	Material * MaterialAssetImporter::operator()(const MetaData & md) const
	{
		// Erase and return 'begin'
		/* * * * * * * * * * * * * * * * * * * * */
		auto pop_front = [](List<String> & toks)
		{
			if (toks.empty()) return String();
			String temp = toks.front();
			toks.erase(toks.begin());
			return temp;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Material>(name))
				{
					// Uniform List
					/* * * * * * * * * * * * * * * * * * * * */
					List<Uniform *> uniforms;

					// Load Default Uniforms
					/* * * * * * * * * * * * * * * * * * * * */
					if (md.getData("defaults", false))
					{
						if (auto u = Uniform::duplicate<uni_vec2_ref>(
							ML_Content.get<uni_vec2_ref>("%CURSOR_POS%")
							)) uniforms.push_back(u);

						if (auto u = Uniform::duplicate<uni_int1_ref>(
							ML_Content.get<uni_int1_ref>("%FRAME_COUNT%")
							)) uniforms.push_back(u);

						if (auto u = Uniform::duplicate<uni_flt1_ref>(
							ML_Content.get<uni_flt1_ref>("%DELTA_TIME%")
							)) uniforms.push_back(u);

						if (auto u = Uniform::duplicate<uni_vec2_ref>(
							ML_Content.get<uni_vec2_ref>("%RESOLUTION%")
							)) uniforms.push_back(u);

						if (auto u = Uniform::duplicate<uni_flt1_ref>(
							ML_Content.get<uni_flt1_ref>("%TOTAL_TIME%")
							)) uniforms.push_back(u);
					}

					// Load User Uniforms from File
					/* * * * * * * * * * * * * * * * * * * * */
					if (Ifstream file { ML_FS.pathTo(md.getData("uniforms")) })
					{
						// following should probably be in UniformAssetImporter
						
						String line;
						while (std::getline(file, line))
						{
							// Skip if empty or comment
							/* * * * * * * * * * * * * * * * * * * * */
							if (line.empty() || line.trim().front() == '#')
								continue;

							// Parse tokens from line
							/* * * * * * * * * * * * * * * * * * * * */
							List<String> tokens = ([](String line)
							{
								List<String> toks;
								if (!line) return toks;
								line.trim()
									.replaceAll("\t", " ")
									.replaceAll(",", "");
								size_t idx = 0;
								while ((idx = line.find(" ")) != String::npos)
								{
									String temp = line.substr(0, idx);
									if (temp) toks.push_back(temp);
									line.erase(0, idx + 1);
								}
								if (line) toks.push_back(line);
								return toks;
							})(line);

							// Parse uniform from tokens
							/* * * * * * * * * * * * * * * * * * * * */
							if (tokens && (pop_front(tokens) == "uniform"))
							{
								// Uniform Type
								/* * * * * * * * * * * * * * * * * * * * */
								const int32_t u_type = ([](C_String type)
								{
									if (!type) return -1;
									for (size_t i = 0; i < Uniform::MAX_UNI_TYPES; i++)
										if (std::strcmp(type, Uniform::TypeNames[i]) == 0)
											return (int32_t)i;
									return -1;
								})(pop_front(tokens).c_str());
								
								// Uniform Name
								/* * * * * * * * * * * * * * * * * * * * */
								const String u_name = pop_front(tokens);

								// Uniform Data
								/* * * * * * * * * * * * * * * * * * * * */
								SStream u_data = ([](List<String> & data)
								{
									SStream out;
									if ((data.size() > 2) &&
										(data.front() == "{") &&
										(data.back() == "}"))
									{
										data.erase(data.begin());
										String temp;
										while (data && ((temp = data.front()) != "}"))
										{
											out << temp << ' ';
											data.erase(data.begin());
										}
									}
									return out;
								})(tokens);

								// Generate Uniform
								/* * * * * * * * * * * * * * * * * * * * */
								if (Uniform * u = ([](int32_t type, const String & name, SStream & ss)
								{
									Uniform * out = nullptr;
									if (type == -1 || !name || !(String)ss.str()) 
										return out;
									switch (type)
									{
									case Uniform::Int1:
									{
										int32_t temp; ss >> temp;
										return out = new uni_int1(name, temp);
									}
									case Uniform::Flt1:
									{
										float_t temp; ss >> temp;
										return out = new uni_flt1(name, temp);
									}
									case Uniform::Vec2:
									{
										vec2 temp; ss >> temp;
										return out = new uni_vec2(name, temp);
									}
									case Uniform::Vec3:
									{
										vec3 temp; ss >> temp;
										return out = new uni_vec3(name, temp);
									}
									case Uniform::Vec4:
									{
										vec4 temp; ss >> temp;
										return out = new uni_vec4(name, temp);
									}
									case Uniform::Col4:
									{
										vec4 temp; ss >> temp;
										return out = new uni_col4(name, temp);
									}
									case Uniform::Mat3:
									{
										mat3 temp; ss >> temp;
										return out = new uni_mat3(name, temp);
									}
									case Uniform::Mat4:
									{
										mat4 temp; ss >> temp;
										return out = new uni_mat4(name, temp);
									}
									case Uniform::Tex2:
									{
										return out = new uni_tex2(name, 
											ML_Content.get<Texture>(String(ss.str()).trim())
										);
									}
									default: return out;
									}
								})(u_type, u_name, u_data))
								{
									uniforms.push_back(u);
								}
							}
						}
						file.close();
					}

					return ML_Content.insert(
						name, new Material {
							ML_Content.get<Shader>(md.getData("shader")),
							uniforms
						});
				}
			}
		}
		return nullptr;
	}


	// Mesh Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Mesh * MeshAssetImporter::operator()(const MetaData & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Mesh>(name))
				{
					if (const String file = md.getData("file"))
					{
						auto temp = new Mesh();
						if (temp->loadFromFile(ML_FS.pathTo(file)))
						{
							return ML_Content.insert(name, temp);
						}
						delete temp;
					}
					else
					{
						return ML_Content.insert(name, new Mesh());
					}
				}
			}
		}
		return nullptr;
	}


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Model * ModelAssetImporter::operator()(const MetaData & md) const
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
	Script * ScriptAssetImporter::operator()(const MetaData & md) const
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
	Shader * ShaderAssetImporter::operator()(const MetaData & md) const
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
	Sound * SoundAssetImporter::operator()(const MetaData & md) const
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
	Sprite * SpriteAssetImporter::operator()(const MetaData & md) const
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
	Surface * SurfaceAssetImporter::operator()(const MetaData & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Surface>(name))
				{
					const String  m = md.getData("model");
					const String  s = md.getData("shader");
					const int32_t w = md.getData("width", 1920);
					const int32_t h = md.getData("height", 1080);
					if (m && s && w && h)
					{
						auto temp = new Surface();
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
						return ML_Content.insert(name, new Surface());
					}
				}
			}
		}
		return nullptr;
	}


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	Texture * TextureAssetImporter::operator()(const MetaData & md) const
	{
		if (md.getData("type").asString() == this->getTag())
		{
			if (const String name = md.getData("name"))
			{
				if (!ML_Content.get<Texture>(name))
				{
					const bool smooth = md.getData("smooth", true);
					const bool repeat = md.getData("repeat", false);
					const bool mipmap = md.getData("mipmap", false);
				
					const int32_t level = md.getData("level", 0);
				
					const GL::Target target = md.getData("target", GL::Texture2D, {
						{ "texture_2d",		GL::Texture2D },
						{ "texture_3d",		GL::Texture3D },
						{ "texture_cube",	GL::TextureCubeMap },
					});
				
					const GL::Format format = md.getData("format", GL::RGBA, {
						{ "red",			GL::Red	},
						{ "green",			GL::Green },
						{ "blue",			GL::Blue },
						{ "rgb",			GL::RGB	},
						{ "rgba",			GL::RGBA },
					});
				
					const GL::Type pix_ty = md.getData("pixfmt", GL::UnsignedByte, {
						{ "byte",			GL::Byte },
						{ "ubyte",			GL::UnsignedByte },
						{ "short",			GL::Short },
						{ "ushort",			GL::UnsignedShort },
						{ "int",			GL::Int },
						{ "uint",			GL::UnsignedInt },
						{ "float",			GL::Float },
						{ "half_float",		GL::HalfFloat },
					});

					if (const String file = md.getData("file"))
					{
						auto temp = new Texture(
							target, 
							format,
							format,
							smooth,
							repeat,
							mipmap,
							level, 
							pix_ty
						);
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
							auto temp = new Texture(
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
						return ML_Content.insert(name, new Texture());
					}
				}
			}
		}
		return nullptr;
	}
}