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

#include <ML/Core/GLM.hpp>

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
					ImGui::MenuItem("Noobs Editor", "", &noobs.showBuilder);
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
				ml::Dockspace & d = ev->dockspace;
				d.dockWindow("Noobs Scene", d.getNode(d.LeftUp));
				d.dockWindow("Noobs Editor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEnter(const ml::EnterEvent & ev)
	{
		ml::Debug::log("Loaded {0}", (*this));
	}

	void Noobs::onStart(const ml::StartEvent & ev)
	{
		// Get Surfaces
		noobs.surf_main = ML_Content.get<ml::Surface>("noobs_surf_main");
		noobs.surf_post = ML_Content.get<ml::Surface>("noobs_surf_post");

		// Create Entity
		noobs.entity = ML_Content.create<ml::Entity>("noobs_entity_0");

		// Attach Renderer
		noobs.renderer = noobs.entity->add<ml::Renderer>(
			ML_Content.get<ml::Model>("sphere32x24"),
			noobs.material = ML_Content.get<ml::Material>("noobs_material_0")
		);

		// Generate Source Editors
		if (noobs.material && noobs.material->shader())
		{
			// Create Main File
			noobs.file_list.push_back(new NoobFile(
				"Main", 
				ml::String()
			));

			// Create Vertex File
			if (noobs.material->shader()->vertSrc())
			{
				noobs.file_list.push_back(new NoobFile(
					"Vertex",
					noobs.material->shader()->vertSrc()
				));
				noobs.file_list.front()->text.SetText(
					noobs.file_list.front()->text.GetText() +
					"// Vertex Shader\n"
					"#shader vertex\n"
					"#include \"Vertex\"\n"
				);
			}

			// Create Fragment File
			if (noobs.material->shader()->fragSrc())
			{
				noobs.file_list.push_back(new NoobFile(
					"Fragment",
					noobs.material->shader()->fragSrc()
				));
				noobs.file_list.front()->text.SetText(
					noobs.file_list.front()->text.GetText() +
					"// Fragment Shader\n"
					"#shader fragment\n"
					"#include \"Fragment\"\n"
				);
			}

			// Create Geometry File
			if (noobs.material->shader()->geomSrc())
			{
				noobs.file_list.push_back(new NoobFile(
					"Geometry",
					noobs.material->shader()->geomSrc()
				));
				noobs.file_list.front()->text.SetText(
					noobs.file_list.front()->text.GetText() +
					"// Geometry Shader\n"
					"#shader geometry\n"
					"#include \"Geometry\"\n"
				);
			}
		}
	}

	void Noobs::onUpdate(const ml::UpdateEvent & ev)
	{
		// Update Surfaces
		if (noobs.freeAspect)
			noobs.resolution = ev.window.getFrameSize();
		noobs.surf_main->resize(noobs.resolution);
		noobs.surf_post->resize(noobs.resolution);
	}

	void Noobs::onDraw(const ml::DrawEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Bind Main Surface
		noobs.surf_main->bind();
		{
			// Clear Screen
			ev.window.clear(noobs.clearColor);

			// Draw Renderer
			ev.window.draw(noobs.entity->get<ml::Renderer>());
		}
		// Unbind Main Surface
		noobs.surf_main->unbind();

		/* * * * * * * * * * * * * * * * * * * * */

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

	void Noobs::onGui(const ml::GuiEvent & ev)
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
					static const auto & res_values = ml::VideoSettings::resolutions();
					static ml::List<ml::String> res_names;
					if (res_names.empty())
					{
						res_names.push_back("Auto");
						for (const auto & video : res_values)
						{
							ml::SStream ss;
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
						noobs.resolution = (ml::vec2i)res_values[index - 1].resolution;
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
						hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
						vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
					return (src * (((hs) < (vs)) ? (hs) : (vs)));
				};

				const ml::vec2 src = texture->size();
				const ml::vec2 dst = ML_EditorUtility.getWindowSize();
				const ml::vec2 scl = scaleToFit(src, dst);
				const ml::vec2 pos = ((dst - scl) * 0.5f);

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
						for (auto & file : noobs.file_list)
						{
							file->dirty = false;
						}

						// custom shader parser
						struct NoobParser
						{
							inline ml::String operator()(const NoobFile::List & file_list, const ml::String & src) const
							{
								ml::SStream out;
								ml::SStream ss(src);
								ml::String	line;
								while (std::getline(ss, line))
								{
									if (line.find("#include") != ml::String::npos)
									{
										bool found = false;
										ml::String name;
										if (ml::ShaderParser::parseWrapped(
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
											out << line << ml::endl;
										}
									}
									else
									{
										out << line << ml::endl;
									}
								}
								return (ml::String)out.str();
							}
						};

						const ml::String source = NoobParser()(
							noobs.file_list,
							noobs.file_list.front()->text.GetText()
						);
						ml::Shader * shader = std::remove_cv_t<ml::Shader *>(
							noobs.material->shader()
						);
						if (shader && shader->loadFromMemory(source))
						{
							ml::Debug::log("Compiled Shader");
						}
						else
						{
							ml::Debug::logError("Failed Compiling Shader");
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
						static char name[NoobFile::MaxName] = "New File";

						// reset popup
						auto resetPopup = [&]()
						{
							std::strcpy(name, "New File");
							ImGui::CloseCurrentPopup();
						};

						// add new file
						auto addNewFile = [&]()
						{
							if (!ml::String(name))
							{
								ml::Debug::logError("Name cannot be empty");
								return;
							}
							for (auto file : noobs.file_list)
							{
								if (file->name == name)
								{
									ml::Debug::logError("File with name \'{0}\' already exists", name);
									return;
								}
							}
							noobs.file_list.push_back(new NoobFile(name, ml::String()));
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

						ml::List<NoobFile::List::iterator> toRemove;

						for (auto it = noobs.file_list.begin(); it != noobs.file_list.end(); it++)
						{
							const size_t i = (it - noobs.file_list.begin());

							const ml::String label {
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
										"code here, or create multiple file_list and\n"
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
									char buf[NoobFile::MaxName];
									std::strcpy(buf, (*it)->name.c_str());
									if (ImGui::InputText(
										"Name",
										buf,
										NoobFile::MaxName,
										ImGuiInputTextFlags_EnterReturnsTrue
									))
									{
										(*it)->name = buf;
									}
								}

								(*it)->text.Render(
									ml::String("##File" + (*it)->name + "##Text").c_str()
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
							noobs.file_list.erase(it);
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
					ml::Uniform * u = nullptr;
					if (ml::UniformPropertyDrawer()("##NewUniform##Material##Noobs", u))
					{
						if (noobs.material->hasUniform(u->name) ||
							!noobs.material->uniforms().insert({ u->name, u }).first->second)
						{
							delete u;
							ml::Debug::logError("A uniform with that name already exists");
						}
					}

					// do nothing if empty
					if (!noobs.material->uniforms().empty())
						ImGui::Separator();

					// to remove
					ml::List<ml::Map<ml::String, ml::Uniform *>::iterator> toRemove;

					for (auto it = noobs.material->uniforms().rbegin();
						it != noobs.material->uniforms().rend();
						it++)
					{
						// label
						const ml::String label("##Uni##" + it->first + "##Material##Noobs");

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
								if (it->second->type == ml::uni_mat3::ID) { height = 3; }
								else if (it->second->type == ml::uni_mat4::ID) { height = 4; }

								ImGui::PushID(label.c_str());
								ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
								ImGui::BeginChild(
									("UniformChild##" + label).c_str(),
									{ -1, (32 * height) + (height == 1 ? 8 : -8) },
									true,
									ImGuiWindowFlags_NoScrollWithMouse
								);

								if (ml::UniformPropertyDrawer()(label, (ml::Uniform &)(*it->second)))
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
						ml::Uniform * u = it->second;
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

						int32_t i = ml::GL::indexOf(noobs.renderer->states().alpha.comp);
						if (ImGui::Combo(
							"Comparison##Alpha Testing##Renderer##Noobs",
							&i,
							ml::GL::Comp_names,
							IM_ARRAYSIZE(ml::GL::Comp_names)
						))
						{
							ml::GL::valueAt(i, noobs.renderer->states().alpha.comp);
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

						auto factor_combo = [](ml::C_String label, int32_t & i)
						{
							return ImGui::Combo(
								label,
								&i,
								ml::GL::Factor_names,
								IM_ARRAYSIZE(ml::GL::Factor_names)
							);
						};

						int32_t srcRGB = ml::GL::indexOf(noobs.renderer->states().blend.srcRGB);
						if (factor_combo("Src RGB##Blending##Renderer##Noobs", srcRGB))
						{
							ml::GL::valueAt(srcRGB, noobs.renderer->states().blend.srcRGB);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t srcAlpha = ml::GL::indexOf(noobs.renderer->states().blend.srcAlpha);
						if (factor_combo("Src Alpha##Blending##Renderer##Noobs", srcAlpha))
						{
							ml::GL::valueAt(srcAlpha, noobs.renderer->states().blend.srcAlpha);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t dstRGB = ml::GL::indexOf(noobs.renderer->states().blend.dstRGB);
						if (factor_combo("Dst RGB##Blending##Renderer##Noobs", dstRGB))
						{
							ml::GL::valueAt(dstRGB, noobs.renderer->states().blend.dstRGB);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						int32_t dstAlpha = ml::GL::indexOf(noobs.renderer->states().blend.dstAlpha);
						if (factor_combo("Dst Alpha##Blending##Renderer##Noobs", dstAlpha))
						{
							ml::GL::valueAt(dstAlpha, noobs.renderer->states().blend.dstAlpha);
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

						int32_t i = ml::GL::indexOf(noobs.renderer->states().culling.face);
						if (ImGui::Combo(
							"Face##Culling##Renderer##Noobs",
							&i,
							ml::GL::Face_names,
							IM_ARRAYSIZE(ml::GL::Face_names)
						))
						{
							ml::GL::valueAt(i, noobs.renderer->states().culling.face);
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

						int32_t i = ml::GL::indexOf(noobs.renderer->states().depth.comp);
						if (ImGui::Combo(
							"Comparison##Depth Testing##Renderer##Noobs",
							&i,
							ml::GL::Comp_names,
							IM_ARRAYSIZE(ml::GL::Comp_names)
						))
						{
							ml::GL::valueAt(i, noobs.renderer->states().depth.comp);
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

						int32_t i = ml::GL::indexOf(noobs.renderer->states().texture.target);
						if (ImGui::Combo(
							"Target##Texture##Renderer##Noobs",
							&i,
							"Texture 2D\0"
							"Texture 3D\0"
							"Texture Cube Map\0"))
						{
							ml::GL::valueAt(i, noobs.renderer->states().texture.target);
						}
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Misc"))
					{
						ImGui::Checkbox(
							"Multisample##Misc##Renderer##Noobs",
							&noobs.renderer->states().misc.multisample
						);
						ImGui::SameLine();
						ML_EditorUtility.HelpMarker("Some very helpful text.");

						ImGui::Checkbox(
							"Framebuffer SRGB##Misc##Renderer##Noobs",
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
					const ml::Shader * shader = noobs.material->shader();
					if (ml::ShaderPropertyDrawer()("Shader##Material##Noobs", shader))
					{
						noobs.material->shader() = shader;
					}
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("The shader to be used.");

					/* * * * * * * * * * * * * * * * * * * * */

					// Model
					const ml::Model * model = (const ml::Model *)noobs.renderer->drawable();
					if (ml::ModelPropertyDrawer()("Model##Renderer##Noobs", model))
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
	}

	void Noobs::onExit(const ml::ExitEvent & ev)
	{
		for (auto & e : noobs.file_list)
		{
			if (e) delete e;
		}
		noobs.file_list.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}