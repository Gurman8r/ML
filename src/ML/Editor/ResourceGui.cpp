#include <ML/Editor/ResourceGui.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Core/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Engine/Rigidbody.hpp>
#include <ML/Engine/Particle.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ResourceGui::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline static const T * ResourceDropdown(
			CString label, const T * current, Registry<T> & reg
		)
		{
			int32_t index = reg.getIndexOf(current);
			return (ResourceGui::StringCombo(label, index, reg.keys())
				? reg.getByIndex(index)
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
				draw_entity_registry(ev.resources, ev.resources.entities);
			});

			Layout::Header("Fonts", [&]()
			{
				draw_font_registry(ev.resources, ev.resources.fonts);
			});

			Layout::Header("Images", [&]()
			{
				draw_image_registry(ev.resources, ev.resources.images);
			});

			Layout::Header("Materials", [&]()
			{
				draw_material_registry(ev.resources, ev.resources.materials);
			});

			Layout::Header("Meshes", [&]()
			{
				draw_mesh_registry(ev.resources, ev.resources.meshes);
			});

			Layout::Header("Models", [&]()
			{
				draw_model_registry(ev.resources, ev.resources.models);
			});

			Layout::Header("Shaders", [&]()
			{
				draw_shader_registry(ev.resources, ev.resources.shaders);
			});

			Layout::Header("Sprites", [&]()
			{
				draw_sprite_registry(ev.resources, ev.resources.sprites);
			});

			Layout::Header("Surfaces", [&]()
			{
				draw_surface_registry(ev.resources, ev.resources.surfaces);
			});

			Layout::Header("Textures", [&]()
			{
				draw_texture_registry(ev.resources, ev.resources.textures);
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_entity_registry(Resources & res, Registry<Entity> & entities)
	{
		ImGui::BeginGroup();
		for (auto & pair : entities)
		{
			Entity * ent = pair.second;

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
							(Model *)r->drawable(),
							res.models))
						{
							r->drawable() = model;
						}

						if (const Material * material = Layout::ResourceDropdown(
							"Material##Renderer",
							r->material(),
							res.materials))
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

	void ResourceGui::draw_font_registry(Resources & res, Registry<Font> & fonts)
	{
		ImGui::BeginGroup();
		for (auto & pair : fonts)
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

	void ResourceGui::draw_image_registry(Resources & res, Registry<Image> & images)
	{
		ImGui::BeginGroup();
		for (auto & pair : images)
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::Text("Size: %i x %i", pair.second->width(), pair.second->height());
				ImGui::Text("Channels: %i", pair.second->channels());
				ImGui::TreePop();
			}
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_material_registry(Resources & res, Registry<Material> & materials)
	{
		ImGui::BeginGroup();
		for (auto & pair : materials)
		{
			Material * mat = pair.second;

			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				/* * * * * * * * * * * * * * * * * * * * */

				if (const Shader * shader = Layout::ResourceDropdown(
					"Shader##Material", 
					mat->shader(), 
					res.shaders))
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

					List<Material::UniformMap::iterator> toRemove;

					for (auto it = mat->uniforms().begin(); it != mat->uniforms().end(); it++)
					{
						if (ImGui::TreeNode(it->first.c_str()))
						{
							const String label("##" + pair.first + "##Uni##" + it->first);

							UniformField(res, label, it->second);

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
						uni_base * u = it->second;
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

	void ResourceGui::draw_mesh_registry(Resources & res, Registry<Mesh> & meshes)
	{
		ImGui::BeginGroup();
		for (auto & pair : meshes)
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::Text("Vertices: %u", pair.second->vertices().size());
				ImGui::Text("Indices: %u", pair.second->indices().size());
				ImGui::Text("Contiguous: %u", pair.second->contiguous().size());
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_model_registry(Resources & res, Registry<Model> & models)
	{
		ImGui::BeginGroup();
		for (auto & pair : models)
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

	void ResourceGui::draw_script_registry(Resources & res, Registry<Script> & scripts)
	{
		ImGui::BeginGroup();
		for (auto & pair : scripts)
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

	void ResourceGui::draw_shader_registry(Resources & res, Registry<Shader> & shaders)
	{
		ImGui::BeginGroup();
		for (auto & pair : shaders)
		{
			Shader * shader = pair.second;
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				// Reload
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::Button("Reload"))
				{
					if (shaders.reload(pair.first))
					{
						Debug::log("Reloaded Shader: {0}", pair.first);
					}
					else
					{
						Debug::logError("Failed Reloading Shader: {0}", pair.first);
					}
				}

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

	void ResourceGui::draw_sprite_registry(Resources & res, Registry<Sprite> & sprites)
	{
		ImGui::BeginGroup();
		for (auto & pair : sprites)
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				vec4 color = pair.second->color();
				if (ImGui::ColorEdit4("Color##Sprite", &color[0]))
				{
					pair.second->setColor(color);
				}

				vec2 origin = pair.second->origin();
				if (ImGui::InputFloat2("Origin##Sprite", &origin[0], 1))
				{
					pair.second->setOrigin(origin);
				}

				vec2 position = pair.second->position();
				if (ImGui::InputFloat2("Position##Sprite", &position[0], 1))
				{
					pair.second->setPosition(position);
				}

				float rotation = pair.second->rotation();
				if (ImGui::InputFloat("Rotation##Sprite", &rotation, 1))
				{
					pair.second->setRotation(rotation);
				}

				vec2 scale = pair.second->scale();
				if (ImGui::InputFloat2("Scale##Sprite", &scale[0], 1))
				{
					pair.second->setScale(scale);
				}

				if (const Texture * tex = Layout::ResourceDropdown(
					"Texture##Sprite",
					pair.second->texture(),
					res.textures
				))
				{
					pair.second->setTexture(tex);
				}

				ImGui::PopID();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ResourceGui::draw_surface_registry(Resources & res, Registry<Surface> & surfaces)
	{
		ImGui::BeginGroup();
		for (auto & pair : surfaces)
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				ImGui::PushID(pair.first.c_str());

				if (const Shader * shader = Layout::ResourceDropdown(
					"Shader##Surface",
					pair.second->shader(),
					res.shaders))
				{
					pair.second->shader() = shader;
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

					vec2 src = pair.second->texture().size();
					vec2 pos = ML_EditorUtility.getCursorPos();
					vec2 dst = { 256, 256 };
					vec2 scl = scaleToFit(src, dst);

					ImGui::Text("%s", src.ToString().c_str());

					ImGui::Image(
						pair.second->texture().get_address(),
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

	void ResourceGui::draw_texture_registry(Resources & res, Registry<Texture> & textures)
	{
		ImGui::BeginGroup();
		for (auto & pair : textures)
		{
			if (ImGui::TreeNode(pair.first.c_str()))
			{
				Texture * tex = pair.second;

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

	bool ResourceGui::StringCombo(CString label, int32_t & index, const List<String> & keys)
	{
		return ImGui::Combo(
			label,
			&index,
			EditorUtility::vector_getter,
			static_cast<void *>(&std::remove_cv_t<List<String> &>(keys)),
			(int32_t)(keys.size())
		);
	}

	void ResourceGui::NewUniformPopup(Material * mat)
	{
		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New Uniform Editor");
		}
		if (ImGui::BeginPopupModal("New Uniform Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static int32_t	type = 0;
			static char		name[32] = "NewUniform\0";

			auto closePopup = [&]()
			{
				type = 0;
				std::strcpy(name, "NewUniform\0");
				ImGui::CloseCurrentPopup();
			};

			ImGui::Combo(
				"Type",
				&type,
				"Float\0"
				"Integer\0"
				"Vector 2\0"
				"Vector 3\0"
				"Vector 4\0"
				"Color 4\0"
				"Matrix 3x3\0"
				"Matrix 4x4\0"
				"Sampler 2D\0"
			);

			ImGui::InputText(
				"Name",
				name,
				IM_ARRAYSIZE(name),
				ImGuiInputTextFlags_EnterReturnsTrue
			);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			if (ImGui::Button("Submit"))
			{
				if (String(name) && !mat->find_any(name))
				{
					uni_base * u = nullptr;
					switch (type)
					{
					case uni_base::Flt:  u = new uni_flt(name, 0);	break;
					case uni_base::Int:  u = new uni_int(name, 0);	break;
					case uni_base::Vec2: u = new uni_vec2(name, 0);	break;
					case uni_base::Vec3: u = new uni_vec3(name, 0);	break;
					case uni_base::Vec4: u = new uni_vec4(name, 0);	break;
					case uni_base::Col4: u = new uni_col4(name, 0);	break;
					case uni_base::Mat3: u = new uni_mat3(name, 0);	break;
					case uni_base::Mat4: u = new uni_mat4(name, 0);	break;
					case uni_base::Tex:  u = new uni_tex2(name, 0);	break;
					}
					if (u && (u = mat->uniforms().insert({ name, u }).first->second))
					{
						closePopup();
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
				closePopup();
			}

			ImGui::EndPopup();
		}
	}

	bool ResourceGui::UniformField(Resources & resources, const String & label, uni_base * value)
	{
		switch (value->type)
		{
			// Flt
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt::ID:
			if (auto u = dynamic_cast<uni_flt *>(value))
			{
				const String name = "##" + label + "##Float##Uni" + value->name;
				ImGui::InputFloat(name.c_str(), &u->data, 0.1f);
				return true;
			}

			// Int
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int::ID:
			if (auto u = dynamic_cast<uni_int *>(value))
			{
				const String name = "##" + label + "##Int##Uni" + value->name;
				ImGui::InputInt(name.c_str(), &u->data, 1);
				return true;
			}

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (auto u = dynamic_cast<uni_vec2 *>(value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value->name;
				ImGui::InputFloat2(name.c_str(), &u->data[0], 1);
				return true;
			}

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (auto u = dynamic_cast<uni_vec3 *>(value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value->name;
				ImGui::InputFloat3(name.c_str(), &u->data[0], 1);
				return true;
			}

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (auto u = dynamic_cast<uni_vec4 *>(value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value->name;
				ImGui::InputFloat4(name.c_str(), &u->data[0], 1);
				return true;
			}

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (auto u = dynamic_cast<uni_col4 *>(value))
			{
				const String name = "##" + label + "##Color##Uni" + value->name;
				ImGui::ColorEdit4(name.c_str(), &u->data[0]);
				return true;
			}

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (auto u = dynamic_cast<uni_mat3 *>(value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value->name;
				ImGui::InputFloat4((name + "##0").c_str(), &u->data[0], 3);
				ImGui::InputFloat4((name + "##3").c_str(), &u->data[3], 3);
				ImGui::InputFloat4((name + "##6").c_str(), &u->data[6], 3);
				return true;
			}

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (auto u = dynamic_cast<uni_mat4 *>(value))
			{
				const String name = "##" + label + "##Mat4##Uni" + value->name;
				ImGui::InputFloat4((name + "##0").c_str(), &u->data[0],   3);
				ImGui::InputFloat4((name + "##4").c_str(), &u->data[4],   3);
				ImGui::InputFloat4((name + "##8").c_str(), &u->data[8],   3);
				ImGui::InputFloat4((name + "##12").c_str(), &u->data[12], 3);
				return true;
			}

			// Tex
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto u = dynamic_cast<uni_tex2 *>(value))
			{
				if (const Texture * tex = Layout::ResourceDropdown(
					"##Texture##Uni",
					u->data,
					resources.textures))
				{
					u->data = tex;
				}
				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}