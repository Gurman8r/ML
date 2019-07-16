/* * * * * * * * * * * * * * * * * * * * */

#include "Noobs.hpp"
#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::Noobs(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(EventSystem & eventSystem)
		: EditorPlugin(eventSystem)
	{
		eventSystem.addListener(KeyEvent::ID, this);
		eventSystem.addListener(MainMenuBarEvent::ID, this);
		eventSystem.addListener(BuildDockspaceEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEvent(const Event * value)
	{
		// Handle base events
		EditorPlugin::onEvent(value);

		switch (*value)
		{
			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				if (ev->getPress(KeyCode::L, { 1, 1, 1, 0 }))
				{
					loader.trigger.ready();
				}
			}
			break;

			// Main Menu Bar Event
			/* * * * * * * * * * * * * * * * * * * * */
		case MainMenuBarEvent::ID:
			if (auto ev = value->as<MainMenuBarEvent>())
			{
				switch (ev->menu)
				{
				case MainMenuBarEvent::File:
					break;
				case MainMenuBarEvent::Edit:
					break;
				case MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Noobs Scene", "", &noobs.showScene);
					ImGui::MenuItem("Noobs Editor", "", &noobs.showBuilder);
					break;
				case MainMenuBarEvent::Help:
					break;
				case MainMenuBarEvent::None:
					break;
				}
			}
			break;

			// Build Dockspace Event
			/* * * * * * * * * * * * * * * * * * * * */
		case BuildDockspaceEvent::ID:
			if (auto ev = value->as<BuildDockspaceEvent>())
			{
				Dockspace & d = ev->dockspace;
				d.dockWindow("Noobs Scene", d.getNode(d.LeftUp));
				d.dockWindow("Noobs Editor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEnter(const EnterEvent & ev)
	{
		Debug::log("Loaded {0}", (*this));
	}

	void Noobs::onStart(const StartEvent & ev)
	{
		// Get Surfaces
		noobs.surf_main = ML_Content.get<Surface>("noobs_surf_main");
		noobs.surf_post = ML_Content.get<Surface>("noobs_surf_post");

		// Create Entity
		if (noobs.entity = ML_Content.create<Entity>("noobs_entity_0"))
		{
			// Attach Renderer
			noobs.renderer = noobs.entity->add<Renderer>(
				ML_Content.get<Model>("sphere32x24"),
				noobs.material = ML_Content.get<Material>("noobs_material_0")
			);
		}

		// Generate Source Editors
		if (noobs.material && noobs.material->shader())
		{
			// Create Main File
			noobs.files.push_back(new NoobsFile(
				"Main", 
				String()
			));

			// Create Vertex File
			if (noobs.material->shader()->vertSrc())
			{
				noobs.files.push_back(new NoobsFile(
					"Vertex",
					noobs.material->shader()->vertSrc()
				));
				noobs.files.front()->text.SetText(
					noobs.files.front()->text.GetText() +
					"// Vertex Shader\n"
					"#shader vertex\n"
					"#include \"Vertex\"\n"
				);
			}

			// Create Fragment File
			if (noobs.material->shader()->fragSrc())
			{
				noobs.files.push_back(new NoobsFile(
					"Fragment",
					noobs.material->shader()->fragSrc()
				));
				noobs.files.front()->text.SetText(
					noobs.files.front()->text.GetText() +
					"// Fragment Shader\n"
					"#shader fragment\n"
					"#include \"Fragment\"\n"
				);
			}

			// Create Geometry File
			if (noobs.material->shader()->geomSrc())
			{
				noobs.files.push_back(new NoobsFile(
					"Geometry",
					noobs.material->shader()->geomSrc()
				));
				noobs.files.front()->text.SetText(
					noobs.files.front()->text.GetText() +
					"// Geometry Shader\n"
					"#shader geometry\n"
					"#include \"Geometry\"\n"
				);
			}
		}
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		// Update Surfaces
		if (noobs.freeAspect)
			noobs.resolution = ev.window.getFrameSize();
		noobs.surf_main->resize(noobs.resolution);
		noobs.surf_post->resize(noobs.resolution);
	}

	void Noobs::onDraw(const DrawEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Bind Main Surface
		noobs.surf_main->bind();
		{
			// Clear Screen
			ev.window.clear(noobs.clearColor);

			// Draw Renderer
			ev.window.draw(noobs.entity->get<Renderer>());
		}
		// Unbind Main Surface
		noobs.surf_main->unbind();

		/* * * * * * * * * * * * * * * * * * * * */

		// Reset States
		static RenderStates states {
			AlphaMode	{ true, GL::Greater, 0.01f },
			BlendMode	{ true	},
			CullingMode	{ false },
			DepthMode	{ false },
			TextureMode	{ true, GL::Texture2D, GL::Texture0 },
			MiscStates	{ false, false }
		};
		states.apply();

		/* * * * * * * * * * * * * * * * * * * * */

		// Bind Post Surface
		noobs.surf_post->bind();
		{
			// Draw Main Surface
			noobs.surf_main->shader()->setUniform("Effect.mode", 0);
			
			ev.window.draw(noobs.surf_main);
		}
		// Unbind Post Surface
		noobs.surf_post->unbind();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onGui(const GuiEvent & ev)
	{
		// Noobs Scene
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Noobs Scene",
			noobs.showScene,
			ImGuiWindowFlags_MenuBar,
			([&]()
		{

			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Noobs Scene"))
				{
					// Resolution Names
					static const auto & res_values = VideoSettings::resolutions();
					static List<String> res_names;
					if (res_names.empty())
					{
						res_names.push_back("Auto");
						for (const auto & video : res_values)
						{
							SStream ss;
							ss << video.resolution;
							res_names.push_back(ss.str());
						}
					}

					// Resolution
					static int32_t index = 0;
					if (ImGui::Combo(
						"Resolution##Noobs",
						&index,
						ML_EditorUtility.vector_getter,
						(void *)&res_names,
						(int32_t)res_names.size()
					))
					{
						noobs.freeAspect = (index == 0);
					}
					if (!(noobs.freeAspect))
					{
						noobs.resolution = (vec2i)res_values[index - 1].resolution;
					}

					// Clear Color
					ImGui::ColorEdit4("Clear Color##Noobs", &noobs.clearColor[0]);

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Texture * texture = &noobs.surf_post->texture();
			if (texture && (*texture))
			{
				auto scaleToFit = [](const vec2 & src, const vec2 & dst)
				{
					const vec2
						hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
						vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
					return (src * (((hs) < (vs)) ? (hs) : (vs)));
				};

				const vec2 src = texture->size();
				const vec2 dst = ML_EditorUtility.getWindowSize();
				const vec2 scl = scaleToFit(src, dst);
				const vec2 pos = ((dst - scl) * 0.5f);

				ImGui::BeginChild("Viewport", { -1, -1 });
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(texture->get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}));


		// Noobs Editor
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Noobs Editor",
			noobs.showBuilder,
			ImGuiWindowFlags_None,
			([&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginTabBar("Noobs Editor Tabs"))
			{
				// Sources
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Sources##Files##Shader##Noobs"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::Button("New##File##Editor##Noobs"))
					{
						ImGui::OpenPopup("New File##Popup##Editor##Noobs");
					}
					ImGui::SameLine();
					if (ImGui::Button("Compile##Editor##Noobs"))
					{
						for (auto & file : noobs.files)
						{
							file->dirty = false;
						}

						// custom shader parser
						struct NoobParser
						{
							inline String operator()(const FileList & file_list, const String & src) const
							{
								SStream out;
								SStream ss(src);
								String	line;
								while (std::getline(ss, line))
								{
									if (line.find("#include") != String::npos)
									{
										bool found = false;
										String name;
										if (ShaderParser::parseWrapped(
											line, '\"', '\"', name
										))
										{
											for (const auto & e : file_list)
											{
												if (e->name == name)
												{
													out << (*this)(
														file_list,
														e->text.GetText()
													);
													found = true;
													break;
												}
											}
										}
										if (!found)
										{
											out << line << endl;
										}
									}
									else
									{
										out << line << endl;
									}
								}
								return (String)out.str();
							}
						};

						const String source = NoobParser()(
							noobs.files,
							noobs.files.front()->text.GetText()
						);
						Shader * shader = std::remove_cv_t<Shader *>(
							noobs.material->shader()
						);
						if (shader && shader->loadFromMemory(source))
						{
							Debug::log("Compiled Shader");
						}
						else
						{
							Debug::logError("Failed Compiling Shader");
						}
					}
					ImGui::Separator();

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::BeginPopupModal(
						"New File##Popup##Editor##Noobs",
						nullptr,
						ImGuiWindowFlags_AlwaysAutoResize
					))
					{
						static char name[NoobsFile::MaxName] = "New File";

						// reset popup
						auto resetPopup = [&]()
						{
							std::strcpy(name, "New File");
							ImGui::CloseCurrentPopup();
						};

						// add new file
						auto addNewFile = [&]()
						{
							if (!String(name))
							{
								Debug::logError("Name cannot be empty");
								return;
							}
							for (auto file : noobs.files)
							{
								if (file->name == name)
								{
									Debug::logError("File with name \'{0}\' already exists", name);
									return;
								}
							}
							noobs.files.push_back(new NoobsFile(name, String()));
						};

						// Input
						bool enterPress = ImGui::InputText(
							"Name##Editor##Noobs",
							name,
							IM_ARRAYSIZE(name),
							ImGuiInputTextFlags_EnterReturnsTrue
						);

						// Submit / Cancel
						if (enterPress || ImGui::Button("Submit##Editor##Noobs"))
						{
							addNewFile();
							resetPopup();
						}
						ImGui::SameLine();
						if (ImGui::Button("Cancel##Editor##Noobs"))
						{
							resetPopup();
						}

						ImGui::EndPopup();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::BeginTabBar("Source##Tabs##Shader##Noobs"))
					{
						/* * * * * * * * * * * * * * * * * * * * */

						List<FileList::iterator> toRemove;

						for (auto it = noobs.files.begin(); it != noobs.files.end(); it++)
						{
							const size_t i = (it - noobs.files.begin());

							const String label {
								"[" + std::to_string(i) + "] " + (*it)->name
							};

							if (ImGui::BeginTabItem(
								label.c_str(),
								(i > 0 ? &(*it)->open : nullptr),
								(*it)->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0
							))
							{
								if (i == 0)
								{
									ML_EditorUtility.HelpMarker(
										"This is the \'Main\' file of your shader.\n"
										"You can either write all of your shader\n"
										"code here, or create multiple files and\n"
										"link them together in Main.\n"
										"\n"
										"#shader vertex / fragment / geometry\n"
										"#include \'...\' / \"...\" / <...> \n"
									);
								}

								// Input Text Content Area
								ImGui::BeginChild(
									"InputTextContentArea",
									{ 0, 0 },
									true,
									ImGuiWindowFlags_None
								);

								/* * * * * * * * * * * * * * * * * * * * */

								// Disallow editing Main's name
								if (i > 0)
								{
									char buf[NoobsFile::MaxName];
									std::strcpy(buf, (*it)->name.c_str());
									if (ImGui::InputText(
										"Name",
										buf,
										NoobsFile::MaxName,
										ImGuiInputTextFlags_EnterReturnsTrue
									))
									{
										(*it)->name = buf;
									}
								}

								(*it)->text.Render(
									String("##File" + (*it)->name + "##Text").c_str()
								);

								if ((*it)->text.IsTextChanged())
									(*it)->dirty = true;

								/* * * * * * * * * * * * * * * * * * * * */

								ImGui::EndChild();
								ImGui::EndTabItem();
							}

							if (!(*it)->open)
							{
								toRemove.push_back(it);
							}
						}

						for (auto it : toRemove)
						{
							delete (*it);
							noobs.files.erase(it);
						}

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::EndTabBar();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndTabItem();

					/* * * * * * * * * * * * * * * * * * * * */
				}

				// Uniforms
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Uniforms##Material##Noobs"))
				{
					// new uniform editor
					Uniform * u = nullptr;
					if (UniformPropertyDrawer()("##NewUniform##Material##Noobs", u))
					{
						if (noobs.material->hasUniform(u->name) ||
							!noobs.material->uniforms().insert({ u->name, u }).first->second)
						{
							delete u;
							Debug::logError("A uniform with that name already exists");
						}
					}

					// do nothing if empty
					if (!noobs.material->uniforms().empty())
						ImGui::Separator();

					// to remove
					List<Map<String, Uniform *>::iterator> toRemove;

					for (auto it = noobs.material->uniforms().rbegin();
						it != noobs.material->uniforms().rend();
						it++)
					{
						// label
						const String label("##Uni##" + it->first + "##Material##Noobs");

						// Uniform Header
						ImGui::PushStyleColor(
							ImGuiCol_Header,
							{ 0.367f, 0.258f, 0.489f, 0.580f }
						);

						if (ImGui::CollapsingHeader((it->first + label).c_str()))
						{
							ImGui::PopStyleColor();

							if (it->second)
							{
								float_t height = 1;
								if (it->second->type == uni_mat3::ID) { height = 3; }
								else if (it->second->type == uni_mat4::ID) { height = 4; }

								ImGui::PushID(label.c_str());
								ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
								ImGui::BeginChild(
									("UniformChild##" + label).c_str(),
									{ -1, (32 * height) + (height == 1 ? 8 : -8) },
									true,
									ImGuiWindowFlags_NoScrollWithMouse
								);

								if (UniformPropertyDrawer()(label, (Uniform &)(*it->second)))
								{
									ImGui::SameLine();
									if (ImGui::Button(("Remove##" + label).c_str()))
									{
										toRemove.push_back(std::next(it).base());
									}
								}

								ImGui::EndChild();
								ImGui::PopStyleVar();
								ImGui::PopID();
							}
						}
						else
						{
							ImGui::PopStyleColor();
						}

						ImGui::Separator();
					}

					for (auto it : toRemove)
					{
						Uniform * u = it->second;
						noobs.material->uniforms().erase(it);
						delete u;
					}

					ImGui::EndTabItem();
				}

				// Settings
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Settings##Noobs"))
				{
					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Alpha Testing"))
					{
						bool & enabled = noobs.renderer->states().alpha.enabled;
						ImGui::Checkbox((enabled
							? "Enabled ##Alpha Testing##Renderer##Noobs"
							: "Disabled##Alpha Testing##Renderer##Noobs"
							),
							&enabled
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t i = GL::indexOf(noobs.renderer->states().alpha.comp);
						if (ImGui::Combo(
							"Comparison##Alpha Testing##Renderer##Noobs",
							&i,
							GL::Comp_names,
							IM_ARRAYSIZE(GL::Comp_names)
						))
						{
							GL::valueAt(i, noobs.renderer->states().alpha.comp);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::DragFloat("Coeff##Alpha Testing##Renderer##Noobs", &noobs.renderer->states().alpha.coeff);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Blending"))
					{
						bool & enabled = noobs.renderer->states().blend.enabled;
						ImGui::Checkbox((enabled
							? "Enabled ##Blending##Renderer##Noobs"
							: "Disabled##Blending##Renderer##Noobs"
							),
							&enabled
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						auto factor_combo = [](C_String label, int32_t & i)
						{
							return ImGui::Combo(
								label,
								&i,
								GL::Factor_names,
								IM_ARRAYSIZE(GL::Factor_names)
							);
						};

						int32_t srcRGB = GL::indexOf(noobs.renderer->states().blend.srcRGB);
						if (factor_combo("Src RGB##Blending##Renderer##Noobs", srcRGB))
						{
							GL::valueAt(srcRGB, noobs.renderer->states().blend.srcRGB);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t srcAlpha = GL::indexOf(noobs.renderer->states().blend.srcAlpha);
						if (factor_combo("Src Alpha##Blending##Renderer##Noobs", srcAlpha))
						{
							GL::valueAt(srcAlpha, noobs.renderer->states().blend.srcAlpha);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t dstRGB = GL::indexOf(noobs.renderer->states().blend.dstRGB);
						if (factor_combo("Dst RGB##Blending##Renderer##Noobs", dstRGB))
						{
							GL::valueAt(dstRGB, noobs.renderer->states().blend.dstRGB);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t dstAlpha = GL::indexOf(noobs.renderer->states().blend.dstAlpha);
						if (factor_combo("Dst Alpha##Blending##Renderer##Noobs", dstAlpha))
						{
							GL::valueAt(dstAlpha, noobs.renderer->states().blend.dstAlpha);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Culling"))
					{
						bool & enabled = noobs.renderer->states().culling.enabled;
						ImGui::Checkbox((enabled
							? "Enabled ##Culling##Renderer##Noobs"
							: "Disabled##Culling##Renderer##Noobs"
							),
							&enabled
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t i = GL::indexOf(noobs.renderer->states().culling.face);
						if (ImGui::Combo(
							"Face##Culling##Renderer##Noobs",
							&i,
							GL::Face_names,
							IM_ARRAYSIZE(GL::Face_names)
						))
						{
							GL::valueAt(i, noobs.renderer->states().culling.face);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Depth Testing"))
					{
						bool & enabled = noobs.renderer->states().depth.enabled;
						ImGui::Checkbox((enabled
							? "Enabled ##Depth Testing##Renderer##Noobs"
							: "Disabled##Depth Testing##Renderer##Noobs"
							),
							&enabled
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t i = GL::indexOf(noobs.renderer->states().depth.comp);
						if (ImGui::Combo(
							"Comparison##Depth Testing##Renderer##Noobs",
							&i,
							GL::Comp_names,
							IM_ARRAYSIZE(GL::Comp_names)
						))
						{
							GL::valueAt(i, noobs.renderer->states().depth.comp);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Texture"))
					{
						bool & enabled = noobs.renderer->states().texture.enabled;
						ImGui::Checkbox((enabled
							? "Enabled ##Texture##Renderer##Noobs"
							: "Disabled##Texture##Renderer##Noobs"
							),
							&enabled
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t i = GL::indexOf(noobs.renderer->states().texture.target);
						if (ImGui::Combo(
							"Target##Texture##Renderer##Noobs",
							&i,
							"Texture 2D\0"
							"Texture 3D\0"
							"Texture Cube Map\0"))
						{
							GL::valueAt(i, noobs.renderer->states().texture.target);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("MiscStates"))
					{
						ImGui::Checkbox(
							"Multisample##MiscStates##Renderer##Noobs",
							&noobs.renderer->states().misc.multisample
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::Checkbox(
							"Framebuffer SRGB##MiscStates##Renderer##Noobs",
							&noobs.renderer->states().misc.framebufferSRGB
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					// Shader
					const Shader * shader = noobs.material->shader();
					if (ShaderPropertyDrawer()("Shader##Material##Noobs", shader))
					{
						noobs.material->shader() = shader;
					}
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("The shader to be used.");

					/* * * * * * * * * * * * * * * * * * * * */

					// Model
					const Model * model = (const Model *)noobs.renderer->drawable();
					if (ModelPropertyDrawer()("Model##Renderer##Noobs", model))
					{
						noobs.renderer->drawable() = model;
					}
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("The model to be drawn.");

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}));


		// Progress Popup
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		{
			// Trigger Loader
			if (loader.trigger.consume())
			{
				// Not already loading
				if (!loader.isWorking() && !loader.thr.alive())
				{
					// Open Popup
					ImGui::OpenPopup("Progress##Popup##Noobs");

					// Launch Thread
					loader.thr.launch([&]()
					{
						/* * * * * * * * * * * * * * * * * * * * */

						Debug::log("Loading...");

						loader.reset().maxElement = 100;

						while (loader.isWorking())
						{
							auto dummy_load = [&](const String & filename)
							{ 
								loader.thr.sleep(50_ms); 
								return true;
							};
							
							loader.attempt(dummy_load("Example.txt"));
						}

						Debug::log("Done loading.");

						/* * * * * * * * * * * * * * * * * * * * */
					});
				}
				else
				{
					Debug::logError("Already loading...");
				}
			}

			// Draw Loader
			if (ImGui::BeginPopupModal(
				"Progress##Popup##Noobs", 
				nullptr, 
				ImGuiWindowFlags_AlwaysAutoResize
			))
			{
				if (loader.isWorking())
				{
					auto str = String("Loading {0}/{1}").format(
						loader.numAttempt, 
						loader.maxElement
					);
					ImGui::Text("Testing Parallel Loading");
					ImGui::ProgressBar(loader.progress(), { 0.0f, 0.0f }, str.c_str());
				}
				else if (loader.isDone())
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Dispose Loader
			if (loader.isDone())
			{
				if (loader.dispose())
				{
					Debug::log("Loader disposed.");
				}
				else
				{ 
					Debug::logError("Failed disposing loader?");
				}
			}
		};
	}

	void Noobs::onExit(const ExitEvent & ev)
	{
		// Cleanup Files
		for (auto & e : noobs.files)
			if (e) delete e;
		noobs.files.clear();

		// Cleanup Load Thread
		loader.thr.dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}