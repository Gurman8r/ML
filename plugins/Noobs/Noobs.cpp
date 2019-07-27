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
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				// Load Test
				if (ev->getPress(KeyCode::L, { 1, 1, 1, 0 }))
				{
					noobs.trigger.arm();
				}
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value->as<MainMenuBarEvent>())
			{
				switch (ev->menu)
				{
				case MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Noobs Scene", "", &noobs.showScene);
					ImGui::MenuItem("Noobs Editor", "", &noobs.showBuilder);
					break;
				}
			}
			break;

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

	void Noobs::onStart(const StartEvent & ev)
	{
		// Modify Default Uniforms
		if (noobs.material)
		{
			if (auto u = noobs.material->get<uni_vec2_ptr>("u_viewport"))
			{
				u->data = &noobs.viewport;
			}
		}

		// Create Entity
		if (Entity * ent = noobs.entity.create())
		{
			// Attach Renderer
			noobs.renderer = ent->add<Renderer>(noobs.model, noobs.material,
				RenderStates { {
					new AlphaTest	{ true, GL::Greater, 0.01f },
					new BlendFunc	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
					new CullFace	{ true, GL::Back },
					new DepthTest	{ true, GL::Less }
				} });
		}

		// Setup Editor
		generateFiles();
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		// Update Resolutions
		if (noobs.freeAspect)	{ noobs.viewport = ev.window.getFrameSize(); }
		if (noobs.surf_main)	{ noobs.surf_main->update(noobs.viewport); }
		if (noobs.surf_post)	{ noobs.surf_post->update(noobs.viewport); }
	}

	void Noobs::onDraw(const DrawEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Main Scene
		if (noobs.surf_main)
		{
			// Bind Main Surface
			noobs.surf_main->bind();

			// Viewport
			ev.window.setViewport({ 0, 0 }, noobs.viewport);
			
			// Clear Screen
			ev.window.clear(noobs.clearColor);

			if (skybox.material)
			{
				DepthMask { false }();
				skybox.material->bind();
				ev.window.draw(skybox.model);
				skybox.material->unbind();
				DepthMask { true }();
			}

			// Draw Renderer
			ev.window.draw(noobs.renderer);
			
			// Unbind Main Surface
			noobs.surf_main->unbind();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		// Reset States
		AlphaTest	{ true, GL::Greater, 0.01f }();
		BlendFunc	{ true	}();
		CullFace	{ false }();
		DepthTest	{ false }();

		/* * * * * * * * * * * * * * * * * * * * */

		// Post Processing
		if (noobs.surf_post)
		{
			// Bind Post Surface
			noobs.surf_post->bind();

			// Viewport
			ev.window.setViewport({ 0, 0 }, noobs.viewport);

			// Render Scene to Texture
			if (noobs.surf_main && noobs.surf_main->shader())
			{
				noobs.surf_main->shader()->setUniform("u_effectMode", noobs.effectMode);
				
				ev.window.draw(noobs.surf_main);
			}
			
			// Unbind Post Surface
			noobs.surf_post->unbind();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onGui(const GuiEvent & ev)
	{
		//static bool once = true;
		//if (once && !(once = false))
		//	ev.editor.terminal().setOpen(true);
		
		this->draw_scene_gui(ev);

		this->draw_editor_gui(ev);

		// Test Worker Popup
		if (true)
		{
			// Trigger Worker
			if (noobs.trigger.consume())
			{
				// Not already running
				if (noobs.worker.available())
				{
					// Open Popup
					ImGui::OpenPopup("Worker##Popup##Noobs");

					// Launch Thread
					noobs.worker.launch(100, [&]()
					{
						/* * * * * * * * * * * * * * * * * * * * */

						Debug::log("Loading...");

						for (size_t i = 0; noobs.worker.working(); i++)
						{
							// Do the thing, record the result, increment the counter.
							noobs.worker.process([&]()
							{
								// Dummy Load
								noobs.worker.sleep(50_ms);
								return true;
							});
						}

						Debug::log("Done loading.");

						/* * * * * * * * * * * * * * * * * * * * */
					});
				}
				else
				{
					Debug::logError("Loading already in progress.");
				}
			}

			// Draw Worker
			if (ImGui::BeginPopupModal(
				"Worker##Popup##Noobs", 
				nullptr, 
				ImGuiWindowFlags_AlwaysAutoResize
			))
			{
				if (noobs.worker.working())
				{
					auto str = String("Loading {0}/{1}").format(
						noobs.worker.current(), 
						noobs.worker.maximum()
					);
					ImGui::Text("Test Parallel Worker");
					ImGui::ProgressBar(noobs.worker.progress(), { 0.0f, 0.0f }, str.c_str());
				}
				else if (noobs.worker.done())
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Dispose Worker
			if (noobs.worker.done())
			{
				if (noobs.worker.dispose())
				{
					Debug::log("Worker disposed.");
				}
				else
				{ 
					Debug::logError("Failed disposing worker?");
				}
			}
		};
	}

	void Noobs::onExit(const ExitEvent & ev)
	{
		disposeFiles();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::draw_scene_gui(const GuiEvent & ev)
	{
		// Noobs Scene
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow("Noobs Scene", noobs.showScene, ImGuiWindowFlags_MenuBar, ([&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Settings"))
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
						"Viewport##Noobs",
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
						noobs.viewport = (vec2i)res_values[index - 1].resolution;
					}

					// Clear Color
					ImGui::ColorEdit4("Clear Color##Noobs", &noobs.clearColor[0]);

					// Effect Mode
					ImGui::InputInt("Effect Mode##Noobs", &noobs.effectMode);

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Texture * texture { nullptr };
			if ((noobs.surf_post) &&
				(texture = &noobs.surf_post->texture()) &&
				(*texture))
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
				const vec2 scl = scaleToFit(src, dst * vec2 { 1.0f, 0.8f });
				const vec2 pos = ((dst - scl) * 0.5f) * vec2 { 1.0f, 0.75f };

				ImGui::BeginChild("Viewport", { -1, -1 });
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(texture->get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}));
	}

	void Noobs::draw_editor_gui(const GuiEvent & ev)
	{
		// Noobs Editor
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow("Noobs Editor", noobs.showBuilder, ImGuiWindowFlags_None, ([&]()
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
						const String source = this->parseFiles(
							noobs.files,
							noobs.files.front()->text.GetText()
						);
						Shader * shader = std::remove_cv_t<Shader *>(
							noobs.material->shader()
						);
						if (!shader || !shader->loadFromMemory(source))
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

						FileList::iterator toRemove = noobs.files.end();

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
								toRemove = it;
							}
						}

						if (toRemove != noobs.files.cend())
						{
							delete (*toRemove);
							noobs.files.erase(toRemove);
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
					Uni * u = nullptr;
					if (UniformPropertyDrawer()("##NewUniform##Material##Noobs", u))
					{
						if (!noobs.material->add(u))
						{
							delete u;
							Debug::logError("A uniform with that name already exists");
						}
					}

					// do nothing if empty
					if (!noobs.material->uniforms().empty())
						ImGui::Separator();

					ImGui::Columns(3, "uniform columns");
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Value"); ImGui::NextColumn();
					ImGui::Separator();
					ImGui::Columns(1);

					auto toRemove = noobs.material->cend();
					for (auto it = noobs.material->cbegin(); it != noobs.material->cend(); it++)
					{
						Uni * u { *it };
						if (!*it) continue;
						bool modifiable = detail::is_modifiable(u);
						const String label("##Uni##" + u->name + "##Material##Noobs");
						ImGui::Columns(3, "uniform columns");

						// Name
						if (modifiable)
						{
							static char name[32] = "";
							std::strcpy(name, u->name.c_str());
							if (ImGui::InputText(
								("##Uniform##Name##Editor##Noobs##" + u->name).c_str(),
								name, IM_ARRAYSIZE(name),
								ImGuiInputTextFlags_EnterReturnsTrue
							))
							{
								if (!noobs.material->get(name))
								{
									u->name = name;
								}
							}
						}
						else
						{
							ImGui::Text("%s", u->name.c_str());
						}
						ImGui::NextColumn();
						
						// Type
						ImGui::Text("%s", detail::nameOf((Uni::Type)u->id)); 
						ImGui::NextColumn();

						// Value
						ImGui::PushID((u->name + label).c_str());
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						if (UniformPropertyDrawer()(label, (Uni &)(*u)))
						{
							ImGui::SameLine();
							if (ImGui::Button(("X##" + label).c_str()))
							{
								toRemove = it;
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted("Delete this uniform");
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
						}
						ImGui::PopStyleVar();
						ImGui::PopID();
						ImGui::Columns(1);
						ImGui::Separator();
					}
					if (toRemove != noobs.material->cend())
					{
						noobs.material->uniforms().erase(toRemove);
					}

					ImGui::EndTabItem();
				}

				// Settings
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Settings##Noobs"))
				{
					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Alpha Test"))
					{
						if (AlphaTest * alphaTest = noobs.renderer->states().get<AlphaTest>())
						{
							bool & enabled = alphaTest->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Alpha Testing##Renderer##Noobs"
								: "Disabled##Alpha Testing##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specify the alpha test function"
							);

							int32_t i = GL::indexOf(alphaTest->comp);
							if (ImGui::Combo(
								"Comparison##Alpha Testing##Renderer##Noobs",
								&i,
								GL::Comp_names,
								IM_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(i, alphaTest->comp);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies the alpha comparison function.\n"
							);

							ImGui::DragFloat("Coeff##Alpha Testing##Renderer##Noobs", &alphaTest->coeff);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies the reference value that incoming alpha values are compared to.\n"
								"This value is clamped to the range 0 1 , where 0 represents the lowest possible alpha value and 1 the highest possible value.\n"
								"The initial reference value is 0."
							);
						}

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Blend Func"))
					{
						if (BlendFunc * blendFunc = noobs.renderer->states().get<BlendFunc>())
						{
							bool & enabled = blendFunc->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Blending##Renderer##Noobs"
								: "Disabled##Blending##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specify pixel arithmetic for RGB and alpha components separately"
							);

							auto factor_combo = [](C_String label, int32_t & i)
							{
								return ImGui::Combo(
									label,
									&i,
									GL::Factor_names,
									IM_ARRAYSIZE(GL::Factor_names)
								);
							};

							int32_t srcRGB = GL::indexOf(blendFunc->srcRGB);
							if (factor_combo("Src RGB##Blending##Renderer##Noobs", srcRGB))
							{
								GL::valueAt(srcRGB, blendFunc->srcRGB);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies how the red, green, and blue blending factors are computed."
							);

							int32_t srcAlpha = GL::indexOf(blendFunc->srcAlpha);
							if (factor_combo("Src Alpha##Blending##Renderer##Noobs", srcAlpha))
							{
								GL::valueAt(srcAlpha, blendFunc->srcAlpha);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specified how the alpha source blending factor is computed."
							);

							int32_t dstRGB = GL::indexOf(blendFunc->dstRGB);
							if (factor_combo("Dst RGB##Blending##Renderer##Noobs", dstRGB))
							{
								GL::valueAt(dstRGB, blendFunc->dstRGB);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies how the red, green, and blue destination blending factors are computed."
							);

							int32_t dstAlpha = GL::indexOf(blendFunc->dstAlpha);
							if (factor_combo("Dst Alpha##Blending##Renderer##Noobs", dstAlpha))
							{
								GL::valueAt(dstAlpha, blendFunc->dstAlpha);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specified how the alpha destination blending factor is computed."
							);
						}

						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Cull Face"))
					{
						if (CullFace * cullFace = noobs.renderer->states().get<CullFace>())
						{
							bool & enabled = cullFace->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Culling##Renderer##Noobs"
								: "Disabled##Culling##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specify which face facets can be culled"
							);

							int32_t i = GL::indexOf(cullFace->face);
							if (ImGui::Combo(
								"Face##Culling##Renderer##Noobs",
								&i,
								GL::Face_names,
								IM_ARRAYSIZE(GL::Face_names)
							))
							{
								GL::valueAt(i, cullFace->face);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies whether front- or back-facing facets are candidates for culling."
							);
						}
						ImGui::TreePop();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::TreeNode("Depth Test"))
					{
						if (DepthTest * depthTest = noobs.renderer->states().get<DepthTest>())
						{
							bool & enabled = depthTest->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Depth Testing##Renderer##Noobs"
								: "Disabled##Depth Testing##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specify the value used for depth buffer comparisons"
							);

							int32_t i = GL::indexOf(depthTest->comp);
							if (ImGui::Combo(
								"Comparison##Depth Testing##Renderer##Noobs",
								&i,
								GL::Comp_names,
								IM_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(i, depthTest->comp);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"GL_NEVER\n"
								"Never passes.\n"
								"\n"
								"GL_LESS\n"
								"Passes if the incoming depth value is less than the stored depth value.\n"
								"\n"
								"GL_EQUAL\n"
								"Passes if the incoming depth value is equal to the stored depth value.\n"
								"\n"
								"GL_LEQUAL\n"
								"Passes if the incoming depth value is less than or equal to the stored depth value.\n"
								"\n"
								"GL_GREATER\n"
								"Passes if the incoming depth value is greater than the stored depth value.\n"
								"\n"
								"GL_NOTEQUAL\n"
								"Passes if the incoming depth value is not equal to the stored depth value.\n"
								"\n"
								"GL_GEQUAL\n"
								"Passes if the incoming depth value is greater than or equal to the stored depth value.\n"
								"\n"
								"GL_ALWAYS\n"
								"Always passes.\n"
							);
						}
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
						disposeFiles();
						generateFiles();
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Noobs::parseFiles(const FileList & file_list, const String & src) const
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
							out << this->parseFiles(file_list, e->text.GetText());
							found = true;
							break;
						}
					}
					if (!found)
					{
						if (auto shader = ML_Content.get<Shader>(name))
						{
							if (shader->vertSrc()) out << "#shader vertex" << endl << shader->vertSrc() << endl;
							if (shader->fragSrc()) out << "#shader fragment" << endl << shader->fragSrc() << endl;
							if (shader->geomSrc()) out << "#shader geometry" << endl << shader->geomSrc() << endl;
							found = true;
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

	void Noobs::generateFiles()
	{
		// Generate Source Editors
		if (noobs.material && noobs.material->shader())
		{
			// Create Main File
			noobs.files.push_back(new NoobsFile("Main", String()));

			// Create Vertex File
			if (noobs.material->shader()->vertSrc())
			{
				noobs.files.push_back(new NoobsFile(
					"Vertex",
					noobs.material->shader()->vertSrc()
				));
				noobs.files.front()->text.SetText(
					noobs.files.front()->text.GetText() +
					//"// Vertex Shader\n"
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
					//"// Fragment Shader\n"
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
					//"// Geometry Shader\n"
					"#shader geometry\n"
					"#include \"Geometry\"\n"
				);
			}
		}
	}

	void Noobs::disposeFiles()
	{
		// Dispose Files
		for (auto & e : noobs.files)
			if (e) delete e;
		noobs.files.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}