/* * * * * * * * * * * * * * * * * * * * */

#include "Noobs.hpp"
#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Core/OS.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Window/WindowEvents.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::Noobs { eventSystem };
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(EventSystem & eventSystem)
		: Plugin		{ eventSystem }
		, m_pipeline	{}
		, m_editor		{}
		, m_scene		{}
		, m_skybox		{}
	{
		eventSystem.addListener(StartEvent::ID,			this);
		eventSystem.addListener(UpdateEvent::ID,		this);
		eventSystem.addListener(DrawEvent::ID,			this);
		eventSystem.addListener(GuiEvent::ID,			this);
		eventSystem.addListener(ExitEvent::ID,			this);
		eventSystem.addListener(KeyEvent::ID,			this);
		eventSystem.addListener(MainMenuBarEvent::ID,	this);
		eventSystem.addListener(DockspaceEvent::ID,		this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEvent(const Event & value)
	{
		switch (*value)
		{
		case StartEvent::ID	: if (auto ev = value.as<StartEvent>()) return onStart(*ev);
		case UpdateEvent::ID: if (auto ev = value.as<UpdateEvent>()) return onUpdate(*ev);
		case DrawEvent::ID	: if (auto ev = value.as<DrawEvent>()) return onDraw(*ev);
		case GuiEvent::ID	: if (auto ev = value.as<GuiEvent>()) return onGui(*ev);
		case ExitEvent::ID	: if (auto ev = value.as<ExitEvent>()) return onExit(*ev);

		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>())
			{
				// Toggle Fullscreen
				if (ev->getPress(KeyCode::F11))
				{
					eventSystem().fireEvent(WindowFullscreenEvent { -1 });
				}
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value.as<MainMenuBarEvent>())
			{
				switch (ev->submenu)
				{
				case MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Scene##Enable##Noobs##DemoScene", "", &m_scene.is_open());
					ImGui::MenuItem("Editor##Enable##Noobs##DemoEditor", "", &m_editor.is_open());
					break;
				}
			}
			break;

		case DockspaceEvent::ID:
			if (auto ev = value.as<DockspaceEvent>())
			{
				EditorDockspace & d = ev->dockspace;
				d.dockWindow("Scene##Noobs##DemoScene", d.getNode(d.LeftUp));
				d.dockWindow("Editor##Noobs##DemoEditor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onStart(const StartEvent & ev)
	{
		// CD
		ML_FS.setPath("../../../");

		// Modify Default Uniforms
		if (Material * m = m_editor.material())
		{
			// Viewport
			if (auto u = m->get<uni_vec2_ptr>("u_viewport"))
			{
				u->data = &m_scene.viewport();
			}
		}

		// Create Editor Entity
		if (Entity * e = m_editor.entity().create())
		{
			// Create Editor Renderer
			m_editor.renderer() = e->add<Renderer>(
				m_editor.model(),
				m_editor.material(),
				RenderStates::get_default()
			);
		}

		// Setup Source Editors
		m_editor.generate_sources();
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		// Update Viewports
		if (auto & surf { m_pipeline[Surf_Main] }) { surf->update(m_scene.viewport()); }
		if (auto & surf { m_pipeline[Surf_Post] }) { surf->update(m_scene.viewport()); }
	}

	void Noobs::onDraw(const DrawEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Render to Main Surface
		m_pipeline.render_to(Surf_Main, [&]()
		{
			// Set Viewport
			ev.window.setViewport({ 0, 0 }, m_scene.viewport());

			// Clear Sceen
			ev.window.clear(m_scene.clearColor());

			// Draw Skybox
			if (m_skybox.material && m_skybox.model)
			{
				ML_GL.depthMask(false);
				m_skybox.material->bind();
				ev.window.draw(m_skybox.model);
				m_skybox.material->unbind();
				ML_GL.depthMask(true);
			}

			ev.window.draw(m_editor.renderer()); // Draw Renderer
		});

		/* * * * * * * * * * * * * * * * * * * * */

		// Apply Default States
		static RenderStates states
		{ {
			new AlphaState	{ true, GL::Greater, 0.01f },
			new BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
			new CullState	{ false, GL::Back },
			new DepthState	{ false, GL::Less, true },
		} };
		states.apply();

		/* * * * * * * * * * * * * * * * * * * * */

		// Render to Post Surface
		m_pipeline.render_to(Surf_Post, [&]()
		{
			// Set Viewport
			ev.window.setViewport({ 0, 0 }, m_scene.viewport());

			// Apply Effects to Main
			if (Surface * surf { m_pipeline[Surf_Main] })
			{
				surf->setUniform(&m_scene.effectMode());
				surf->setUniform(&m_scene.kernel());
				ev.window.draw(surf);
			}
		});

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onGui(const GuiEvent & ev)
	{
		// Render Scene
		m_scene.render(ev, "Scene##Noobs##DemoScene", m_pipeline[Surf_Post]);
		
		// Render Editor
		m_editor.render(ev, "Editor##Noobs##DemoEditor");
	}

	void Noobs::onExit(const ExitEvent & ev)
	{
		Debug::log("Goodbye!");
	}


	// DEMO SCENE
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Noobs::DemoScene::render(const GuiEvent & ev, C_String title, const Surface * surf)
	{
		if (!m_open) return;
		ImGui::PushID("Noobs");
		ImGui::PushID("Demo Scene");
		if (ImGui::Begin(title, &m_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Settings"))
				{
					// Get Video Modes
					static const List<VideoMode> & mode_values
					{
						Window::getFullscreenModes()
					};

					// Get Video Names
					static const List<String> & mode_names = [&]
					{
						static List<String> temp
						{
							"Free" 
						};
						for (const VideoMode & elem : mode_values)
						{
							temp.push_back(String("{0}").format(elem.size));
						}
						return temp;
					}();
					ImGui::Separator();

					// Viewport
					static int32_t index = 0;
					if (ImGuiExt::Combo("Viewport", &index, mode_names))
					{
						m_freeAspect = (index == 0);
					}
					if (!m_freeAspect)
					{
						m_viewport = (vec2i)mode_values[index - 1].size;
					}
					ImGui::Separator();

					// Clear Color
					ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);
					ImGui::Separator();

					// Effect Mode
					ImGuiExt::Combo("Effect Mode", &m_effectMode.data,
						"Default\0"
						"Grayscale\0"
						"Blur\0"
						"Kernel\0"
						"Inverted\0"
					);
					ImGui::Separator();

					// Kernel
					PropertyDrawer<Uniform>()("##Kernel", (Uniform &)m_kernel);
					ImGui::SameLine(); ImGui::Text("Kernel");

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			if (surf && (*surf))
			{
				const vec2 dst = ImGuiExt::GetContentRegionAvail();
				const vec2 scl = alg::scale_to_fit((vec2)surf->size(), dst) * 0.975f;
				const vec2 pos = ((dst - scl) * 0.5f);

				if (m_freeAspect) { m_viewport = dst; }

				ImGui::BeginChild("NoobsSceneViewport", { 0, 0 }, true);
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(surf->get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}
		ImGui::End();
		ImGui::PopID();
		ImGui::PopID();
	}


	// DEMO FILE
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Noobs::DemoFile::render(const GuiEvent & ev)
	{
		ImGui::BeginChild("DemoFile##ContentArea", { 0, 0 }, true);
		
		this->text.Render(("##DemoFile##" + this->name + "##ImGuiColorTextEdit").c_str());

		if (this->text.IsTextChanged())
		{ 
			this->dirty = true; 
		}

		ImGui::EndChild();
	}

	
	// DEMO EDITOR
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Noobs::DemoEditor::render(const GuiEvent & ev, C_String title)
	{
		if (!m_open) return;
		ImGui::PushID(ML_ADDRESSOF(this));
		if (ImGui::Begin(title, &m_open, ImGuiWindowFlags_None))
		{
			if (ImGui::BeginTabBar("DemoEditor##TabBar##Main"))
			{
				// Sources Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Sources"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID("##Sources");
					ImGui::BeginChild("##Sources##Content", { 0, 0 }, true);

					/* * * * * * * * * * * * * * * * * * * * */

					// Compile
					if (ImGui::Button("Compile"))
					{
						this->compile_sources();
					}
					ImGuiExt::Tooltip("Build shader source code");
					ImGui::SameLine();
					
					// Rebuild
					if (ImGui::Button("Rebuild"))
					{
						this->reset_sources();
						this->generate_sources();
					}
					ImGuiExt::Tooltip("Update source code to that from the last successfull compilation");
					ImGui::Separator();

					// Toggle Demo Files (start at one to always show config)
					for (size_t i = 0; i < m_files.size(); i++)
					{
						if (!m_files[i]) { continue; }
						if (i > 0) { ImGui::SameLine(); }
						ImGui::Checkbox(m_files[i]->name.c_str(), &m_files[i]->open);
					}

					/* * * * * * * * * * * * * * * * * * * * */

					// Demo File Tab Bar
					if (ImGui::BeginTabBar("Demo File Tab Bar"))
					{
						for (size_t i = 0; i < m_files.size(); i++)
						{
							DemoFile *& file { m_files[i] };
							if (file && file->open)
							{
								// Demo File Tab
								if (ImGui::BeginTabItem(
									file->name.c_str(),
									nullptr,
									(file->dirty
										? ImGuiTabItemFlags_UnsavedDocument
										: ImGuiTabItemFlags_None)
								))
								{
									file->render(ev); // Render Demo File

									ImGui::EndTabItem();
								}
							}
						}
						ImGui::EndTabBar();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChild();
					ImGui::PopID();
					ImGui::EndTabItem();

					/* * * * * * * * * * * * * * * * * * * * */
				}

				// Uniforms Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Uniforms"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID("##Uniforms");
					ImGui::BeginChild("##Uniforms##Content", { 0, 0 }, true);

					/* * * * * * * * * * * * * * * * * * * * */

					// New Uniform Popup
					Uniform * to_add { nullptr };
					if (PropertyDrawer<Uniform>()("##New##Uni", (Uniform *&)to_add))
					{
						// Already Exists
						if (to_add && !m_material->add(to_add))
						{
							delete to_add;
							Debug::logError("A uniform with that name already exists");
						}
					}
					ImGui::Separator();

					/* * * * * * * * * * * * * * * * * * * * */
					
					// Header Columns
					ImGui::Columns(3, "##Uni##Columns");
					ImGui::Selectable("Name"); ImGui::NextColumn();
					ImGui::Selectable("Type"); ImGui::NextColumn();
					ImGui::Selectable("Value"); ImGui::NextColumn();
					ImGui::Separator();
					ImGui::Columns(1);

					/* * * * * * * * * * * * * * * * * * * * */
					
					// Uniform List
					Uniform * to_remove { nullptr };
					for (Uniform *& uni : (*m_material))
					{
						if (!uni) continue;
						ImGui::Columns(3, "##Uni##Columns");
						const String label { "##Uni##" + uni->name };

						// Uniform Name
						/* * * * * * * * * * * * * * * * * * * * */
						if (uni->isModifiable())
						{
							static char name[32] = "";
							std::strcpy(name, uni->name.c_str());
							if (ImGui::InputText(
								(label + "##Name").c_str(),
								name,
								ML_ARRAYSIZE(name),
								ImGuiInputTextFlags_EnterReturnsTrue
							))
							{
								if (!m_material->get(name))
								{
									uni->name = name;
								}
								else
								{
									Debug::logError("A uniform with that name already exists");
								}
							}
						}
						else
						{
							ImGui::Text(uni->name.c_str());
						}
						ImGui::NextColumn();
						
						// Uniform Type
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::Text("%s", detail::name_of((Uniform::Type)uni->id)); 
						ImGui::NextColumn();

						// Uniform Value
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushID(label.c_str());
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						if (PropertyDrawer<Uniform>()(label, (Uniform &)(*uni)))
						{
							// Remove Uniform
							ImGui::SameLine();
							if (ImGui::Button(("X##" + label).c_str())) 
							{
								to_remove = uni;
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
					if (to_remove) { m_material->erase(to_remove); }

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChild();
					ImGui::PopID();
					ImGui::EndTabItem();

					/* * * * * * * * * * * * * * * * * * * * */
				}

				// Settings Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Settings"))
				{
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID("##Settings");
					ImGui::BeginChild("##Settings##Content", { 0, 0 }, true);
					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					const Material * mtl = this->material();
					if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
					{
						if ((this->material().get() != mtl) &&
							this->material().update(
								ML_Content.get_name<Material>(mtl),
								std::remove_cv_t<Material *>(mtl)
							))
						{
							this->renderer()->setMaterial(mtl);
						}
					}

					ImGui::NewLine();

					const Shader * shd = this->shader();
					if (PropertyDrawer<Shader>()("Shader##Material##Noobs", shd))
					{
						this->shader() = shd;
						this->reset_sources();
						this->generate_sources();
					}
					ImGuiExt::Tooltip("Specifies the shader to be edited");

					ImGui::NewLine();

					const Model * mdl = (const Model *)m_renderer->drawable();
					if (PropertyDrawer<Model>()("Model##Renderer##Noobs", mdl))
					{
						this->renderer()->setDrawable(mdl);
					}
					ImGuiExt::Tooltip("Specifies model to be drawn");

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					enum Setting_
					{
						Setting_Disabled,
						Setting_Enum,
						Setting_Bool,
						Setting_Number,
					};

					static const ImVec4 Setting_Colors[] = {
						ImVec4 { 0.84f, 0.63f, 0.63f, 1.00f },	// Disabled
						ImVec4 { 0.72f, 0.84f, 0.63f, 1.00f },	// Enum
						ImVec4 { 0.20f, 0.33f, 1.00f, 1.00f },	// Bool
						ImVec4 { 0.20f, 1.00f, 0.33f, 1.00f },	// Number
					};

					AlphaState	* alpha_state	{ m_renderer->states().get<AlphaState>() };
					BlendState	* blend_state	{ m_renderer->states().get<BlendState>() };
					CullState	* cull_state	{ m_renderer->states().get<CullState>()	 };
					DepthState	* depth_state	{ m_renderer->states().get<DepthState>() };

					ImGui::NewLine(); ImGui::Separator();


					// Alpha State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					if (alpha_state) ([](AlphaState *& alpha)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID("##AlphaState");
						ImGui::NewLine();
						ImGui::PushItemWidth(150);

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox(alpha->enabled
							? "glEnable" : "glDisable",
							&alpha->enabled
						);
						ImGuiExt::Tooltip(GL::desc_of(GL::AlphaTest));
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, alpha->enabled
							? Setting_Colors[Setting_Enum]
							: Setting_Colors[Setting_Disabled]
						);
						ImGui::Text(GL::raw_name_of(GL::AlphaTest));
						ImGui::PopStyleColor();
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glAlphaFunc"))
						{
							OS::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml");
						}
						ImGuiExt::Tooltip("Specify the alpha test function");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t predicate = GL::index_of(alpha->predicate);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##Predicate", &predicate, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							alpha->predicate = GL::value_at<GL::Predicate>(predicate);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'predicate\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(alpha->predicate),
							alg::to_hex<uint32_t>(alpha->predicate),
							GL::desc_of(alpha->predicate)
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Number]);
						ImGui::DragFloat("##Coefficient", &alpha->coeff);
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'coefficient\'\n\n"
							"Value: {0}\n\n"
							"Brief: {1}\n"
						).format(
							alpha->coeff,
							"Specifies the reference value that incoming alpha values are compared to"
						));
						ImGui::SameLine(); ImGui::Text(")");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::NewLine(); ImGui::Separator();

						/* * * * * * * * * * * * * * * * * * * * */
					})(alpha_state);


					// Blend State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					if (blend_state) ([](BlendState * blend)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID("##BlendState");
						ImGui::NewLine();
						ImGui::PushItemWidth(150);

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox(blend->enabled
							? "glEnable" : "glDisable",
							&blend->enabled
						);
						ImGuiExt::Tooltip(GL::desc_of(GL::Blend));
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, blend->enabled
							? Setting_Colors[Setting_Enum]
							: Setting_Colors[Setting_Disabled]
						);
						ImGui::Text(GL::raw_name_of(GL::Blend));
						ImGui::PopStyleColor();
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glBlendFuncSeparate"))
						{
							OS::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glBlendFuncSeparate.xml");
						}
						ImGuiExt::Tooltip("Specify pixel arithmetic for RGB and alpha components separately");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t srcRGB = GL::index_of(blend->srcRGB);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##SrcRGB", &srcRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->srcRGB = GL::value_at<GL::Factor>(srcRGB);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'srcRGB\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->srcRGB),
							alg::to_hex<uint32_t>(blend->srcRGB),
							"Specifies how the red, green, and blue blending factors are computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t dstRGB = GL::index_of(blend->dstRGB);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##DstRGB", &dstRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dstRGB = GL::value_at<GL::Factor>(dstRGB);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'dstRGB\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->dstRGB),
							alg::to_hex<uint32_t>(blend->dstRGB),
							"Specifies how the red, green, and blue destination blending factors are computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t srcAlpha = GL::index_of(blend->srcAlpha);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##SrcAlpha", &srcAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->srcAlpha = GL::value_at<GL::Factor>(srcAlpha);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'srcAlpha\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->srcAlpha),
							alg::to_hex<uint32_t>(blend->srcAlpha),
							"Specifies how the alpha source blending factor is computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t dstAlpha = GL::index_of(blend->dstAlpha);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##DstAlpha", &dstAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dstAlpha = GL::value_at<GL::Factor>(dstAlpha);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'dstAlpha\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->dstAlpha),
							alg::to_hex<uint32_t>(blend->dstAlpha),
							"Specifies how the alpha destination blending factor is computed"
						));
						ImGui::SameLine(); ImGui::Text(")");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::NewLine(); ImGui::Separator();

						/* * * * * * * * * * * * * * * * * * * * */
					})(blend_state);


					// Cull State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					if (cull_state) ([](CullState * cull)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID("##CullState");
						ImGui::NewLine();
						ImGui::PushItemWidth(150);

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox(cull->enabled
							? "glEnable" : "glDisable",
							&cull->enabled
						);
						ImGuiExt::Tooltip(GL::desc_of(GL::CullFace));
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, cull->enabled
							? Setting_Colors[Setting_Enum]
							: Setting_Colors[Setting_Disabled]
						);
						ImGui::Text(GL::raw_name_of(GL::CullFace));
						ImGui::PopStyleColor();
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glCullFace"))
						{
							OS::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml");
						}
						ImGuiExt::Tooltip("Specify which faces can be culled");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t face = GL::index_of(cull->face);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##Face", &face, GL::Face_raw_names, ML_ARRAYSIZE(GL::Face_raw_names)))
						{
							cull->face = GL::value_at<GL::Face>(face);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'face\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(cull->face),
							alg::to_hex<uint32_t>(cull->face),
							GL::desc_of(cull->face)
						));
						ImGui::SameLine(); ImGui::Text(")");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::NewLine(); ImGui::Separator();

						/* * * * * * * * * * * * * * * * * * * * */
					})(cull_state);


					// Depth State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					if (depth_state) ([](DepthState * depth)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID("##DepthState");
						ImGui::NewLine();
						ImGui::PushItemWidth(150);

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox(depth->enabled
							? "glEnable" : "glDisable",
							&depth->enabled
						); 
						ImGuiExt::Tooltip("If enabled, do depth comparisons and update the depth buffer.");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, depth->enabled
							? Setting_Colors[Setting_Enum]
							: Setting_Colors[Setting_Disabled]
						);
						ImGui::Text(GL::raw_name_of(GL::DepthTest));
						ImGui::PopStyleColor();
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox("glDepthMask", &depth->mask); 
						ImGuiExt::Tooltip("Specifies whether the depth buffer is enabled for writing");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, depth->mask 
							? Setting_Colors[Setting_Bool]
							: Setting_Colors[Setting_Disabled]
						);
						ImGui::Text(depth->mask ? "true" : "false");
						ImGui::PopStyleColor();
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glDepthFunc"))
						{
							OS::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml");
						}
						ImGuiExt::Tooltip("Specify the value used for depth buffer comparisons");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t predicate = GL::index_of(depth->predicate);
						ImGui::PushStyleColor(ImGuiCol_Text, Setting_Colors[Setting_Enum]);
						if (ImGuiExt::Combo("##Predicate", &predicate, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							depth->predicate = GL::value_at<GL::Predicate>(predicate);
						}
						ImGui::PopStyleColor();
						ImGuiExt::Tooltip(String(
							"Param: \'predicate\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(depth->predicate),
							alg::to_hex<uint32_t>(depth->predicate),
							GL::desc_of(depth->predicate)
						));
						ImGui::SameLine(); ImGui::Text(")");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::NewLine(); ImGui::Separator();

						/* * * * * * * * * * * * * * * * * * * * */
					})(depth_state);

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChild();
					ImGui::PopID(); 
					ImGui::EndTabItem();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
		ImGui::PopID();
	}

	void Noobs::DemoEditor::generate_sources()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto setup_file = [&](DemoFile::Type type, const String & src)
		{
			if (this->material() && this->shader())
			{
				if (!m_files[type])
				{
					m_files[type] = new DemoFile(type, src);
				}
				else
				{
					m_files[type]->text.SetText(src);
				}

				return m_files[type];
			}
			return (DemoFile *)nullptr;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (!this->material()) return;

		if (auto vert = setup_file(DemoFile::Vert, this->shader()->sources().vs))
		{
			vert->open = this->shader()->sources().vs;
		}

		if (auto frag = setup_file(DemoFile::Frag, this->shader()->sources().fs))
		{
			frag->open = this->shader()->sources().fs;
		}

		if (auto geom = setup_file(DemoFile::Geom, this->shader()->sources().gs))
		{
			geom->open = this->shader()->sources().gs;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	bool Noobs::DemoEditor::compile_sources()
	{
		if (m_material && shader())
		{
			if (Shader * s = std::remove_cv_t<Shader *>(this->shader()))
			{
				for (DemoFile *& f : m_files)
				{
					if (f) { f->dirty = false; }
				}

				if (m_files[DemoFile::Geom]->open)
				{
					return s->loadFromMemory(
						m_files[DemoFile::Vert]->text.GetText(),
						m_files[DemoFile::Geom]->text.GetText(),
						m_files[DemoFile::Frag]->text.GetText()
					);
				}
				else
				{
					return s->loadFromMemory(
						m_files[DemoFile::Vert]->text.GetText(),
						m_files[DemoFile::Frag]->text.GetText()
					);
				}
			}
		}
		return false;
	}

	void Noobs::DemoEditor::reset_sources()
	{
		for (DemoFile *& f : m_files)
		{
			if (!f) continue;
			f->open = false;
			f->text.SetText(String());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}