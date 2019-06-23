#include <ML/Editor/ResourceGui.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ResourceGui::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline static const T * ResourceDropdown(CString label, const T * cur)
		{
			int32_t index = ML_Content.getIndexOf<T>(cur);
			return (ML_EditorUtility.StringCombo(label, index, ML_Content.keys<T>())
				? ML_Content.getByIndex<T>(index)
				: nullptr
			);
		}


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline static void Header(CString label, Fun && fun, Args && ... args)
		{
			if (!ImGui::CollapsingHeader(label))
				return;

			fun((args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ResourceGui::ResourceGui(EventSystem & eventSystem)
		: EditorGui(eventSystem, "Resources")
	{
	}

	ResourceGui::~ResourceGui()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ResourceGui::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s", getTitle());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Layout::Header("Entities", [&]()
			{
				draw_entity_registry();
			});

			Layout::Header("Fonts", [&]()
			{
				draw_font_registry();
			});

			Layout::Header("Images", [&]()
			{
				draw_image_registry();
			});

			Layout::Header("Materials", [&]()
			{
				draw_material_registry();
			});

			Layout::Header("Meshes", [&]()
			{
				draw_mesh_registry();
			});

			Layout::Header("Models", [&]()
			{
				draw_model_registry();
			});

			Layout::Header("Shaders", [&]()
			{
				draw_shader_registry();
			});

			Layout::Header("Sprites", [&]()
			{
				draw_sprite_registry();
			});

			Layout::Header("Surfaces", [&]()
			{
				draw_surface_registry();
			});

			Layout::Header("Textures", [&]()
			{
				draw_texture_registry();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_entity_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Entity>())
		{
			Entity * ent = reinterpret_cast<Entity *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				/* * * * * * * * * * * * * * * * * * * * */

				if (Transform * t = ent->get<Transform>())
				{
					Layout::Header("Transform", [&]()
					{

					});
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (Renderer * r = ent->get<Renderer>())
				{
					RenderStates & states = r->states();
					Layout::Header("Renderer", [&]()
					{
						/* * * * * * * * * * * * * * * * * * * * */

						if (const Model * model = Layout::ResourceDropdown(
							"Model##Renderer",
							(Model *)r->drawable()))
						{
							r->drawable() = model;
						}

						if (const Material * material = Layout::ResourceDropdown(
							"Material##Renderer",
							r->material()))
						{
							r->material() = material;
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Alpha Testing"))
						{
							ImGui::Checkbox("##Enabled##Alpha Testing", &states.alpha.enabled);

							int32_t index = GL::indexOf(states.alpha.comp);
							if (ImGui::Combo(
								"Comparison##Alpha Testing",
								&index,
								GL::Comp_names,
								IM_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(index, states.alpha.comp);
							}

							ImGui::DragFloat("Coeff##Alpha Testing", &states.alpha.coeff);

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Blending"))
						{
							ImGui::Checkbox("Enabled##Blending", &states.blend.enabled);

							auto factor_combo = [](CString label, int32_t & index)
							{
								return ImGui::Combo(
									label,
									&index,
									GL::Factor_names,
									IM_ARRAYSIZE(GL::Factor_names)
								);
							};

							int32_t srcRGB = GL::indexOf(states.blend.srcRGB);
							if (factor_combo("Src RGB##Blending", srcRGB))
							{
								GL::valueAt(srcRGB, states.blend.srcRGB);
							}

							int32_t srcAlpha = GL::indexOf(states.blend.srcAlpha);
							if (factor_combo("Src Alpha##Blending", srcAlpha))
							{
								GL::valueAt(srcAlpha, states.blend.srcAlpha);
							}

							int32_t dstRGB = GL::indexOf(states.blend.dstRGB);
							if (factor_combo("Dst RGB##Blending", dstRGB))
							{
								GL::valueAt(dstRGB, states.blend.dstRGB);
							}

							int32_t dstAlpha = GL::indexOf(states.blend.dstAlpha);
							if (factor_combo("Dst Alpha##Blending", dstAlpha))
							{
								GL::valueAt(dstAlpha, states.blend.dstAlpha);
							}

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Culling"))
						{
							ImGui::Checkbox("Enabled##Culling", &states.culling.enabled);

							int32_t index = GL::indexOf(states.culling.face);
							if (ImGui::Combo(
								"Face##Culling",
								&index,
								GL::Face_names,
								IM_ARRAYSIZE(GL::Face_names)
							))
							{
								GL::valueAt(index, states.culling.face);
							}

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Depth Testing"))
						{
							ImGui::Checkbox("Enabled##Depth Testing", &states.depth.enabled);

							int32_t index = GL::indexOf(states.depth.comp);
							if (ImGui::Combo(
								"Comparison##Depth Testing",
								&index,
								GL::Comp_names,
								IM_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(index, states.depth.comp);
							}

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Texture"))
						{
							ImGui::Checkbox("Enabled##Texture", &states.texture.enabled);

							int32_t index = GL::indexOf(states.texture.target);
							if (ImGui::Combo(
								"Target##Texture",
								&index,
								"Texture 2D\0"
								"Texture 3D\0"
								"Texture Cube Map\0"))
							{
								GL::valueAt(index, states.texture.target);
							}

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::TreeNode("Misc"))
						{
							ImGui::Checkbox("Multisample##Misc", &states.misc.multisample);

							ImGui::Checkbox("Framebuffer SRGB##Misc", &states.misc.framebufferSRGB);

							ImGui::TreePop();
						}

						/* * * * * * * * * * * * * * * * * * * * */
					});
				}

				/* * * * * * * * * * * * * * * * * * * * */
				
				ImGui::PopID();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_font_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Font>())
		{
			Font * font = reinterpret_cast<Font *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::Text("%s", font->getInfo().family.c_str());
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_image_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Image>())
		{
			Image * img = reinterpret_cast<Image *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::Text("Size: %i x %i", img->width(), img->height());
				ImGui::Text("Channels: %i", img->channels());
				ImGui::TreePop();
			}
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_material_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Material>())
		{
			Material * mat = reinterpret_cast<Material *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				/* * * * * * * * * * * * * * * * * * * * */

				if (const Shader * shader = Layout::ResourceDropdown(
					"Shader##Material", 
					mat->shader()))
				{
					mat->shader() = shader;
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Uniforms"))
				{
					// New Uniform
					/* * * * * * * * * * * * * * * * * * * * */

					NewUniformPopup(mat);

					// Display List
					/* * * * * * * * * * * * * * * * * * * * */

					if (!mat->uniforms().empty())
					{
						ImGui::Separator();
					}

					List<Map<String, Uniform *>::iterator> toRemove;

					for (auto it = mat->uniforms().begin(); it != mat->uniforms().end(); it++)
					{
						if (ImGui::TreeNode(it->first.c_str()))
						{
							const String label("##" + pair.first + "##Uni##" + it->first);

							UniformField(label, it->second);

							ImGui::SameLine();

							if (ImGui::Button(String("Delete" + label).c_str()))
							{
								toRemove.push_back(it);
							}

							ImGui::TreePop();
						}
					}

					for (auto it : toRemove)
					{
						Uniform * u = it->second;
						mat->uniforms().erase(it);
						delete u;
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::PopID();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_mesh_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Mesh>())
		{
			Mesh * mesh = reinterpret_cast<Mesh *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::Text("Vertices: %u",	mesh->vertices().size());
				ImGui::Text("Indices: %u", mesh->indices().size());
				ImGui::Text("Contiguous: %u", mesh->contiguous().size());
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_model_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Model>())
		{
			Model * model = reinterpret_cast<Model *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_script_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Script>())
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_shader_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Shader>())
		{
			Shader * shader = reinterpret_cast<Shader *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				// Source Tabs
				if (ImGui::BeginTabBar("SourceTabs"))
				{
					// Vertex Source
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Vertex##Shader"))
					{
						if (ImGui::BeginChild(
							"Vertex##Content##Shader", 
							{ -1.f, -1.f }, 
							true, 
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (shader->vertSrc())
							{
								ImGui::TextUnformatted(
									&shader->vertSrc()[0],
									&shader->vertSrc()[shader->vertSrc().size()]
								);
							}
							else
							{
								ImGui::Text("Empty");
							}
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}

					// Fragment Source
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Fragment##Shader"))
					{
						if (ImGui::BeginChild(
							"Fragment##Content##Shader",
							{ -1.f, -1.f },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (!shader->fragSrc().empty())
							{
								ImGui::TextUnformatted(
									&shader->fragSrc()[0],
									&shader->fragSrc()[shader->fragSrc().size()]
								);
							}
							else
							{
								ImGui::Text("Empty");
							}
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}

					// Geometry Source
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Geometry##Shader"))
					{
						if (ImGui::BeginChild(
							"Geometry##Content##Shader",
							{ -1.f, -1.f },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (!shader->geomSrc().empty())
							{
								ImGui::TextUnformatted(
									&shader->geomSrc()[0],
									&shader->geomSrc()[shader->geomSrc().size()]
								);
							}
							else
							{
								ImGui::Text("Empty");
							}
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();

					/* * * * * * * * * * * * * * * * * * * * */
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_sprite_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Sprite>())
		{
			Sprite * spr = reinterpret_cast<Sprite *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				vec4 color = spr->color();
				if (ImGui::ColorEdit4("Color##Sprite", &color[0]))
				{
					spr->setColor(color);
				}

				vec2 origin = spr->origin();
				if (ImGui::InputFloat2("Origin##Sprite", &origin[0], 1))
				{
					spr->setOrigin(origin);
				}

				vec2 position = spr->position();
				if (ImGui::InputFloat2("Position##Sprite", &position[0], 1))
				{
					spr->setPosition(position);
				}

				float rotation = spr->rotation();
				if (ImGui::InputFloat("Rotation##Sprite", &rotation, 1))
				{
					spr->setRotation(rotation);
				}

				vec2 scale = spr->scale();
				if (ImGui::InputFloat2("Scale##Sprite", &scale[0], 1))
				{
					spr->setScale(scale);
				}

				if (const Texture * tex = Layout::ResourceDropdown(
					"Texture##Sprite",
					spr->texture()
				))
				{
					spr->setTexture(tex);
				}

				ImGui::PopID();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_surface_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Surface>())
		{
			Surface * surf = reinterpret_cast<Surface *>(pair.second);

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				if (const Shader * shader = Layout::ResourceDropdown(
					"Shader##Surface",
					surf->shader()))
				{
					surf->shader() = shader;
				}

				if (ImGui::TreeNode("Preview"))
				{
					const ImGuiIO & io = ImGui::GetIO();

					auto scaleToFit = [](const vec2 & src, const vec2 & dst)
					{
						const vec2
							hs = (dst[0] / src[0]),
							vs = (dst[1] / src[1]);
						return (src * (((hs) < (vs)) ? (hs) : (vs)));
					};

					vec2 src = surf->texture().size();
					vec2 pos = ML_EditorUtility.getCursorPos();
					vec2 dst = { 256, 256 };
					vec2 scl = scaleToFit(src, dst);

					ImGui::Text("%f %f", src[0], src[1]);

					ImGui::Image(
						surf->texture().get_address(),
						{ scl[0], scl[1] },
						{ 0, 1 },
						{ 1, 0 },
						{ 255, 255, 255, 255 },
						{ 255, 255, 255, 128 }
					);

					ImGui::TreePop();
				}

				ImGui::PopID();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_texture_registry()
	{
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<Texture>())
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				Texture * tex = reinterpret_cast<Texture *>(pair.second);

				ImGui::Columns(2, "texture_data_columns");

				vec2u size = tex->size();
				ImGui::Selectable("Size:");
				ImGui::NextColumn();
				ImGui::Text("%u x %u", size[0], size[1]);
				ImGui::NextColumn();

				vec2u realSize = tex->realSize();
				ImGui::Selectable("Real Size:");
				ImGui::NextColumn();
				ImGui::Text("%u x %u", realSize[0], realSize[1]);
				ImGui::NextColumn();

				bool smooth = tex->smooth();
				ImGui::Selectable("Smooth:");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##Smooth##Texture", &smooth))
				{
					tex->setSmooth(smooth);
				}
				ImGui::NextColumn();

				bool repeated = tex->repeated();
				ImGui::Selectable("Repeated:");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##Repeated##Texture", &repeated))
				{
					tex->setRepeated(repeated);
				}
				ImGui::NextColumn();

				bool mipmapped = tex->mipmapped();
				ImGui::Selectable("Mipmapped:");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##Mipmapped##Texture", &mipmapped))
				{
					tex->setMipmapped(mipmapped);
				}
				ImGui::NextColumn();

				int32_t level = tex->level();
				ImGui::Selectable("Level:");
				ImGui::NextColumn();
				ImGui::Text("%i", level);
				ImGui::NextColumn();

				GL::Target target = tex->target();
				ImGui::Selectable("Target:");
				ImGui::NextColumn();
				ImGui::Text("%s", GL::nameOf(target));
				ImGui::NextColumn();

				GL::Format colorFormat = tex->colorFormat();
				ImGui::Selectable("Color Format:");
				ImGui::NextColumn();
				ImGui::Text("%s", GL::nameOf(colorFormat));
				ImGui::NextColumn();

				GL::Format internalFormat = tex->internalFormat();
				ImGui::Selectable("Internal Format:");
				ImGui::NextColumn();
				ImGui::Text("%s", GL::nameOf(internalFormat));
				ImGui::NextColumn();

				GL::Type pixType = tex->type();
				ImGui::Selectable("Pixel Type:");
				ImGui::NextColumn();
				ImGui::Text("%s", GL::nameOf(pixType));
				ImGui::NextColumn();

				ImGui::Columns(1);

				if (ImGui::TreeNode("Preview"))
				{
					const ImGuiIO & io = ImGui::GetIO();

					auto scaleToFit = [](const vec2 & src, const vec2 & dst)
					{
						const vec2
							hs = (dst[0] / src[0]),
							vs = (dst[1] / src[1]);
						return (src * (((hs) < (vs)) ? (hs) : (vs)));
					};

					vec2 src = tex->size();
					vec2 pos = ML_EditorUtility.getCursorPos();
					vec2 dst = { 256, 256 };
					vec2 scl = scaleToFit(src, dst);

					ImGui::Image(
						tex->get_address(),
						{ scl[0], scl[1] },
						{ 0, 1 },
						{ 1, 0 },
						{ 255, 255, 255, 255 },
						{ 255, 255, 255, 128 }
					);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::NewUniformPopup(Material * mat)
	{
		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New Uniform##ResourceGui");
		}
		if (ImGui::BeginPopupModal("New Uniform##ResourceGui", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static int32_t	type		= 0;
			static char		name[32]	= "my_uniform\0";
			auto			resetPopup	= [&]()
			{
				type = 0;
				std::strcpy(name, "my_uniform\0");
				ImGui::CloseCurrentPopup();
			};

			ImGui::Combo(
				"Type",
				&type,
				Uniform::TypeNames,
				Uniform::MAX_UNI_TYPES
			);

			bool enterPress = ImGui::InputText(
				"Name",
				name,
				IM_ARRAYSIZE(name),
				ImGuiInputTextFlags_EnterReturnsTrue
			);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			if (enterPress || ImGui::Button("Submit"))
			{
				if (String(name) && !mat->hasUniform(name))
				{
					Uniform * u = nullptr;
					switch (type)
					{
					case uni_flt::ID	: u = new uni_flt (name, 0);	break;
					case uni_int::ID	: u = new uni_int (name, 0);	break;
					case uni_vec2::ID	: u = new uni_vec2(name, 0);	break;
					case uni_vec3::ID	: u = new uni_vec3(name, 0);	break;
					case uni_vec4::ID	: u = new uni_vec4(name, 0);	break;
					case uni_col4::ID	: u = new uni_col4(name, 0);	break;
					case uni_mat3::ID	: u = new uni_mat3(name, 0);	break;
					case uni_mat4::ID	: u = new uni_mat4(name, 0);	break;
					case uni_tex2::ID	: u = new uni_tex2(name, 0);	break;
					}
					if (u && (u = mat->uniforms().insert({ name, u }).first->second))
					{
						resetPopup();
					}
				}
				else
				{
					Debug::logError("A uniform with that name already exists!");
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				resetPopup();
			}

			ImGui::EndPopup();
		}
	}

	int32_t ResourceGui::UniformField(const String & label, Uniform * value, bool drag)
	{
		if (!value) { return 0; }

		switch (value->type)
		{
			// Flt
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt::ID:
			if (float * temp = impl::toFloat(value))
			{
				const String name = "##" + label + "##Float##Uni" + value->name;
				if (drag) ImGui::DragFloat(name.c_str(), temp, 0.1f);
				else ImGui::InputFloat(name.c_str(), temp, 0.1f);
				if (auto u = value->as<uni_flt>()) { u->data = (*temp); return 1; }
				else return -1;
			}
			break;

			// Int
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int::ID:
			if (int32_t * temp = impl::toInt(value))
			{
				const String name = "##" + label + "##Int##Uni" + value->name;
				if (drag) ImGui::DragInt(name.c_str(), temp, 0.1f);
				else ImGui::InputInt(name.c_str(), temp, 1);
				if (auto u = value->as<uni_int>()) { u->data = (*temp); return 1; }
				else return -1;
			}
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (vec2 * temp = impl::toVec2(value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value->name;
				if (drag) ImGui::DragFloat2(name.c_str(), &(*temp)[0], 0.1f);
				else ImGui::InputFloat2(name.c_str(), &(*temp)[0], 1);
				if (auto u = value->as<uni_vec2>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (vec3 * temp = impl::toVec3(value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value->name;
				if (drag) ImGui::DragFloat3(name.c_str(), &(*temp)[0], 0.1f);
				else ImGui::InputFloat3(name.c_str(), &(*temp)[0], 1);
				if (auto u = value->as<uni_vec3>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (vec4 * temp = impl::toVec4(value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value->name;
				if (drag) ImGui::DragFloat4(name.c_str(), &(*temp)[0], 0.1f);
				else ImGui::InputFloat4(name.c_str(), &(*temp)[0], 1);
				if (auto u = value->as<uni_vec4>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (vec4 * temp = impl::toCol4(value))
			{
				const String name = "##" + label + "##Col4##Uni" + value->name;
				ImGui::ColorEdit4(name.c_str(), &(*temp)[0]);
				if (auto u = value->as<uni_col4>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (mat3 * temp = impl::toMat3(value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value->name;
				if (drag)
				{
					ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0], 3);
					ImGui::DragFloat4((name + "##03").c_str(), &(*temp)[3], 3);
					ImGui::DragFloat4((name + "##06").c_str(), &(*temp)[6], 3);
				}
				else
				{
					ImGui::InputFloat4((name + "##00").c_str(), &(*temp)[0], 3);
					ImGui::InputFloat4((name + "##03").c_str(), &(*temp)[3], 3);
					ImGui::InputFloat4((name + "##06").c_str(), &(*temp)[6], 3);
				}
				if (auto u = value->as<uni_mat3>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (mat4 * temp = impl::toMat4(value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value->name;
				if (drag)
				{
					ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0], 3);
					ImGui::DragFloat4((name + "##04").c_str(), &(*temp)[4], 3);
					ImGui::DragFloat4((name + "##08").c_str(), &(*temp)[8], 3);
					ImGui::DragFloat4((name + "##12").c_str(), &(*temp)[12], 3);
				}
				else
				{
					ImGui::InputFloat4((name + "##00").c_str(), &(*temp)[0], 3);
					ImGui::InputFloat4((name + "##04").c_str(), &(*temp)[4], 3);
					ImGui::InputFloat4((name + "##08").c_str(), &(*temp)[8], 3);
					ImGui::InputFloat4((name + "##12").c_str(), &(*temp)[12], 3);
				}
				if (auto u = value->as<uni_mat4>()) { u->data = (*temp); return 1; }
				else return -1;
			}

			// Tex
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto u = value->as<uni_tex2>())
			{
				if (const Texture * tex = Layout::ResourceDropdown(
					"##Texture##Uni",
					u->data))
				{
					u->data = tex;
				}
				return 1;
			}
			
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}