/* * * * * * * * * * * * * * * * * * * * */

#include "Noobs.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new DEMO::Noobs(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(ml::EventSystem & eventSystem)
		: EditorPlugin(eventSystem)
	{
		eventSystem.addListener(ml::KeyEvent::ID, this);
		eventSystem.addListener(ml::MainMenuBarEvent::ID, this);
		eventSystem.addListener(ml::BuildDockspaceEvent::ID, this);
	}

	Noobs::~Noobs() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEvent(const ml::Event * value)
	{
		// Handle base events
		ml::EditorPlugin::onEvent(value);

		switch (*value)
		{
			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::KeyEvent::ID:
			if (auto ev = value->as<ml::KeyEvent>())
			{
			}
			break;

			// Main Menu Bar Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::MainMenuBarEvent::ID:
			if (auto ev = value->as<ml::MainMenuBarEvent>())
			{
				switch (ev->menu)
				{
				case ml::MainMenuBarEvent::File:
					break;
				case ml::MainMenuBarEvent::Edit:
					break;
				case ml::MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Noobs Scene", "", &noobs.showScene);
					ImGui::MenuItem("Noobs Builder", "", &noobs.showBuilder);
					break;
				case ml::MainMenuBarEvent::Help:
					break;
				case ml::MainMenuBarEvent::None:
					break;
				}
			}
			break;

			// Build Dockspace Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::BuildDockspaceEvent::ID:
			if (auto ev = value->as<ml::BuildDockspaceEvent>())
			{
				ml::DockspaceGui & d = ev->dockspace;
				d.dockWindow("Noobs Scene", d.getNode(d.MidUp));
				d.dockWindow("Noobs Builder", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEnter(const ml::EnterEvent & ev)
	{
		// Hello!
		ml::Debug::log("Hello from {0}!", (*this));
	}

	void Noobs::onStart(const ml::StartEvent & ev)
	{
		// Get Surfaces
		noobs.surf_main = ev.resources.surfaces.get("noobs_surf_main");
		noobs.surf_post = ev.resources.surfaces.get("noobs_surf_post");

		// Orthographic Projection
		ml::Transform ortho = ml::Transform::Orthographic({
			{ 0.f, 0.f }, 
			(ml::vec2f)noobs.res
		});

		// Perspective Projection
		ml::Transform persp = ml::Transform::Perspective(
			45.f, 
			ML_ASPECT((float)noobs.res[0], (float)noobs.res[1]),
			0.001f,
			1000.f
		);

		// Camera Transform
		ml::Transform camera = ml::Transform(
			{ 0.0f, 0.0f, 10.0f }, 
			{ 1.0f }, 
			{ } 
		);
		camera.lookAt(
			camera.getPos(),
			(camera.getPos() - ml::vec3::Back).normalized()
		);

		// Light Transform
		ml::Transform light = ml::Transform(
			{ 0.0f, 0.0f, 30.0f },
			{ 1.0f },
			{ }
		);

		// Model Transform
		ml::Transform model = ml::Transform(
			{ 0.0f }, 
			{ 1.0f }, 
			{ }
		);

		// Create Material
		ev.resources.materials.create(
			"noobs_material_0",
			ev.resources.shaders.get("noobs_shader_0"),
			ml::List<ml::uni_base *>({
				new ml::uni_mat4("Vert.proj",		persp.getMat()),
				new ml::uni_mat4("Vert.view",		camera.getMat()),
				new ml::uni_mat4("Vert.model",		model.getMat()),
				new ml::uni_vec3("Frag.cameraPos",	camera.getPos()),
				new ml::uni_vec3("Frag.lightPos",	light.getPos()),
				new ml::uni_col4("Frag.diffuse",	ml::Color::LightYellow),
				new ml::uni_col4("Frag.mainCol",	ml::Color::White),
				new ml::uni_tex2("Frag.mainTex",	ev.resources.textures.get("earth_dm")),
				new ml::uni_tex2("Frag.specTex",	ev.resources.textures.get("earth_sm")),
				new ml::uni_flt	("Frag.ambient",	0.01f),
				new ml::uni_flt	("Frag.specular",	0.1f),
				new ml::uni_int	("Frag.shininess",	8),
				}));

		// Create Entity
		if (noobs.ent_main = ev.resources.entities.load("noobs_entity_0"))
		{
			// Entity Renderer
			noobs.ent_main->add<ml::Renderer>(
				ev.resources.models.get("sphere32x24"),
				ev.resources.materials.get("noobs_material_0")
			);
		}
	}

	void Noobs::onUpdate(const ml::UpdateEvent & ev)
	{
		// Update Surface Sizes
		if (noobs.freeAspect)
		{
			noobs.surf_main->resize(ev.window.getFrameSize());
			noobs.surf_post->resize(ev.window.getFrameSize());
		}
		else
		{
			noobs.surf_main->resize(noobs.res);
			noobs.surf_post->resize(noobs.res);
		}
	}

	void Noobs::onDraw(const ml::DrawEvent & ev)
	{
		// Bind Main Surface
		noobs.surf_main->bind();
		{
			// Clear Screen
			ev.window.clear(noobs.clearColor);

			// Draw Renderer
			ev.window.draw(noobs.ent_main->get<ml::Renderer>());
		}
		// Unbind Main Surface
		noobs.surf_main->unbind();

		// Reset States
		static ml::RenderStates states(
			{ true, ml::GL::Greater, 0.01f },
			{ true },
			{ false },
			{ false },
			{ true, ml::GL::Texture2D, ml::GL::Texture0 },
			{ false, false }
		);
		states.apply();

		// Bind Post Surface
		noobs.surf_post->bind();
		{
			// Draw Main Surface
			if (const ml::Shader * shader = noobs.surf_main->shader())
			{
				shader->setUniform("Effect.mode", 0);
			}
			ev.window.draw(noobs.surf_main);
		}
		// Unbind Post Surface
		noobs.surf_post->unbind();
	}

	void Noobs::onGui(const ml::GuiEvent & ev)
	{
		// Noobs Scene
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Noobs Scene",
			noobs.showScene,
			ImGuiWindowFlags_MenuBar,
			[&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Noobs Scene"))
				{
					// Resolution Names
					static const auto & res_values = ml::VideoSettings::resolutions();
					static ml::List<ml::String> res_names;
					if (res_names.empty())
					{
						res_names.push_back("Free");
						for (const auto & video : res_values)
						{
							res_names.push_back(video.resolution.ToString());
						}
					}

					// Resolution
					static int32_t index = 0;
					if (ImGui::Combo(
						"Resolution##Noobs",
						&index, 
						ml::EditorUtility::vector_getter,
						(void *)&res_names,
						(int32_t)res_names.size()
					))
					{
						if (!(noobs.freeAspect = (index == 0)))
						{
							noobs.res = (ml::vec2i)res_values[index - 1].resolution;
							ml::Debug::log(noobs.res.ToString());
						}
						else
						{
							ml::Debug::log("Free");
						}
					}

					// Clear Color
					ImGui::ColorEdit4("Clear Color##Noobs", &noobs.clearColor[0]);

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ml::Texture * texture = &noobs.surf_post->texture();
			if (texture && (*texture))
			{
				auto scaleToFit = [](const ml::vec2 & src, const ml::vec2 & dst)
				{
					const ml::vec2
						hs = (dst[0] / src[0]),
						vs = (dst[1] / src[1]);
					return (src * (((hs) < (vs)) ? (hs) : (vs)));
				};

				const ml::vec2 src = texture->size();
				const ml::vec2 dst = ml::EditorUtility::getWindowSize();
				const ml::vec2 scl = scaleToFit(src, dst);
				const ml::vec2 pos = ((dst - scl) * 0.5f);

				ImGui::BeginChild("Viewport", { -1, -1 });
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(texture->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});

		// Noobs Builder
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Noobs Builder",
			noobs.showBuilder,
			ImGuiWindowFlags_None,
			[&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			// Tabs
			const ml::String mat_name = "noobs_material_0";
			if (ml::Material * mat = ev.resources.materials.get(mat_name))
			{
				if (ImGui::BeginTabBar("Noobs Builder Tabs"))
				{
					// Settings
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Settings##Noobs"))
					{
						// Shader
						int32_t mat_shader = ev.resources.shaders.getIndexOf(mat->shader());
						if (ml::ResourceGui::StringCombo(
							"Shader##Material##Noobs",
							mat_shader,
							ev.resources.shaders.keys()
						))
						{
							mat->shader() = ev.resources.shaders.getByIndex(mat_shader);
						}

						// Renderer
						if (ml::Renderer * r = noobs.ent_main->get<ml::Renderer>())
						{
							ml::RenderStates & states = r->states();

							/* * * * * * * * * * * * * * * * * * * * */

							int32_t ent_model = ev.resources.models.getIndexOf((ml::Model *)r->drawable());
							if (ml::ResourceGui::StringCombo(
								"Model##Renderer##Noobs",
								ent_model,
								ev.resources.models.keys()
							))
							{
								r->drawable() = ev.resources.models.getByIndex(ent_model);
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Alpha Testing"))
							{
								ImGui::Checkbox(
									"##Enabled##Alpha Testing##Renderer##Noobs", 
									&states.alpha.enabled
								);

								int32_t index = ml::GL::indexOf(states.alpha.comp);
								if (ImGui::Combo(
									"Comparison##Alpha Testing##Renderer##Noobs",
									&index,
									ml::GL::Comp_names,
									IM_ARRAYSIZE(ml::GL::Comp_names)
								))
								{
									ml::GL::valueAt(index, states.alpha.comp);
								}

								ImGui::DragFloat("Coeff##Alpha Testing##Renderer##Noobs", &states.alpha.coeff);

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Blending"))
							{
								ImGui::Checkbox(
									"Enabled##Blending##Renderer##Noobs", 
									&states.blend.enabled
								);

								auto factor_combo = [](ml::CString label, int32_t & index)
								{
									return ImGui::Combo(
										label,
										&index,
										ml::GL::Factor_names,
										IM_ARRAYSIZE(ml::GL::Factor_names)
									);
								};

								int32_t srcRGB = ml::GL::indexOf(states.blend.srcRGB);
								if (factor_combo("Src RGB##Blending##Renderer##Noobs", srcRGB))
								{
									ml::GL::valueAt(srcRGB, states.blend.srcRGB);
								}

								int32_t srcAlpha = ml::GL::indexOf(states.blend.srcAlpha);
								if (factor_combo("Src Alpha##Blending##Renderer##Noobs", srcAlpha))
								{
									ml::GL::valueAt(srcAlpha, states.blend.srcAlpha);
								}

								int32_t dstRGB = ml::GL::indexOf(states.blend.dstRGB);
								if (factor_combo("Dst RGB##Blending##Renderer##Noobs", dstRGB))
								{
									ml::GL::valueAt(dstRGB, states.blend.dstRGB);
								}

								int32_t dstAlpha = ml::GL::indexOf(states.blend.dstAlpha);
								if (factor_combo("Dst Alpha##Blending##Renderer##Noobs", dstAlpha))
								{
									ml::GL::valueAt(dstAlpha, states.blend.dstAlpha);
								}

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Culling"))
							{
								ImGui::Checkbox(
									"Enabled##Culling##Renderer##Noobs",
									&states.culling.enabled
								);

								int32_t index = ml::GL::indexOf(states.culling.face);
								if (ImGui::Combo(
									"Face##Culling##Renderer##Noobs",
									&index,
									ml::GL::Face_names,
									IM_ARRAYSIZE(ml::GL::Face_names)
								))
								{
									ml::GL::valueAt(index, states.culling.face);
								}

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Depth Testing"))
							{
								ImGui::Checkbox(
									"Enabled##Depth Testing##Renderer##Noobs",
									&states.depth.enabled
								);

								int32_t index = ml::GL::indexOf(states.depth.comp);
								if (ImGui::Combo(
									"Comparison##Depth Testing##Renderer##Noobs",
									&index,
									ml::GL::Comp_names,
									IM_ARRAYSIZE(ml::GL::Comp_names)
								))
								{
									ml::GL::valueAt(index, states.depth.comp);
								}

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Texture"))
							{
								ImGui::Checkbox(
									"Enabled##Texture##Renderer##Noobs", 
									&states.texture.enabled
								);

								int32_t index = ml::GL::indexOf(states.texture.target);
								if (ImGui::Combo(
									"Target##Texture##Renderer##Noobs",
									&index,
									"Texture 2D\0"
									"Texture 3D\0"
									"Texture Cube Map\0"))
								{
									ml::GL::valueAt(index, states.texture.target);
								}

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::TreeNode("Misc"))
							{
								ImGui::Checkbox(
									"Multisample##Misc##Renderer##Noobs",
									&states.misc.multisample
								);

								ImGui::Checkbox(
									"Framebuffer SRGB##Misc##Renderer##Noobs",
									&states.misc.framebufferSRGB
								);

								ImGui::TreePop();
							}

							/* * * * * * * * * * * * * * * * * * * * */
						}

						ImGui::EndTabItem();
					}

					// Uniforms
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Uniforms##Material##Noobs"))
					{
						// New Uniform
						ml::ResourceGui::NewUniformPopup(mat);

						// Display List
						if (!mat->uniforms().empty())
						{
							ImGui::Separator();
						}

						static int32_t mode = 0;

						ImGui::Combo(
							"Draw Mode", 
							&mode, 
							"Tree Node\0"
							"Collapsing Header\0"
						);
						
						ml::List<ml::Material::UniformMap::iterator> toRemove;
						
						for (auto it = mat->uniforms().begin(); it != mat->uniforms().end(); it++)
						{
							const ml::String label("##" + mat_name + "##Uni##" + it->first + "##Material##Noobs");
							
							switch (mode)
							{
							case 0: { 
								if (!ImGui::TreeNode(it->first.c_str())) continue; 
							} break;

							case 1: { 
								if (!ImGui::CollapsingHeader(it->first.c_str())) continue; 
								ImGui::PushID(label.c_str());
							} break;
							}
							
							//ImGui::Columns(2, "uniform_columns");

							ml::ResourceGui::UniformField(ev.resources, label, it->second);

							//ImGui::NextColumn();
							ImGui::SameLine();
							if (ImGui::Button(ml::String("Rename" + label).c_str()))
							{
								ml::Debug::logWarning("Not Yet Implemented");
							}
							ImGui::SameLine();
							if (ImGui::Button(ml::String("Delete" + label).c_str()))
							{
								toRemove.push_back(it);
							}

							//ImGui::Columns(1);

							switch (mode)
							{
							case 0: ImGui::TreePop(); break;
							case 1: ImGui::PopID(); break;
							}
						}

						for (auto it : toRemove)
						{
							ml::uni_base * u = it->second;
							mat->uniforms().erase(it);
							delete u;
						}

						ImGui::EndTabItem();
					}

					// Vertex Source
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginTabItem("Vertex##Shader##Noobs"))
					{
						if (ImGui::BeginChild(
							"Vertex##Content##Shader",
							{ -1.f, -1.f },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (mat->shader()->vertSrc())
							{
								ImGui::TextUnformatted(
									&mat->shader()->vertSrc()[0],
									&mat->shader()->vertSrc()[mat->shader()->vertSrc().size()]
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
					if (ImGui::BeginTabItem("Fragment##Shader##Noobs"))
					{
						if (ImGui::BeginChild(
							"Fragment##Content##Shader",
							{ -1.f, -1.f },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (!mat->shader()->fragSrc().empty())
							{
								ImGui::TextUnformatted(
									&mat->shader()->fragSrc()[0],
									&mat->shader()->fragSrc()[mat->shader()->fragSrc().size()]
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
					if (ImGui::BeginTabItem("Geometry##Shader##Noobs"))
					{
						if (ImGui::BeginChild(
							"Geometry##Content##Shader",
							{ -1.f, -1.f },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (!mat->shader()->geomSrc().empty())
							{
								ImGui::TextUnformatted(
									&mat->shader()->geomSrc()[0],
									&mat->shader()->geomSrc()[mat->shader()->geomSrc().size()]
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
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	void Noobs::onExit(const ml::ExitEvent & ev)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}