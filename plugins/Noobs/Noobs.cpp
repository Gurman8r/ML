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

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::Noobs { eventSystem };
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <
		class U, class T
	> static inline auto redirect_uni_ptr(const String & name, const T * value)
	{
		if (!name) return;
		if (U * u = (U *)ML_Content.get<Uniform>(name))
		{
			u->data = value;
		}
		for (auto & pair : ML_Content.data<Material>())
		{
			if (Material * m { static_cast<Material *>(pair.second) })
			{
				if (U * u = m->get<U>(name))
				{
					u->data = value;
				}
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(EventSystem & eventSystem)
		: Plugin		{ eventSystem }
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

				// Refresh Sources
				if (ev->getPress(KeyCode::F5))
				{
					m_editor.reset_sources();
					m_editor.generate_sources();
				}

				// Compile Sources
				if (ev->isSave())
				{
					m_editor.compile_sources();
				}
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value.as<MainMenuBarEvent>())
			{
				switch (ev->submenu)
				{
				case MainMenuBarEvent::Window:
					ImGui::MenuItem("Display##Enable##Noobs##DemoView", "", &m_editor.m_scene.m_open);
					ImGui::MenuItem("Editor##Enable##Noobs##DemoEditor", "", &m_editor.m_open);
					break;
				}
			}
			break;

		case DockspaceEvent::ID:
			if (auto ev = value.as<DockspaceEvent>())
			{
				EditorDockspace & d = ev->dockspace;
				d.dockWindow("Display##Noobs##DemoView", d.getNode(d.LeftUp));
				d.dockWindow("Editor##Noobs##DemoEditor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onStart(const StartEvent & ev)
	{
		// Set Path
		ML_FS.setPath("../../../");

		// Setup Uniforms
		redirect_uni_ptr<uni_vec2_ptr>("u_viewport", &m_editor.m_scene.m_viewport);

		// Create Editor Entity
		if (Entity * e = m_editor.m_entity.create())
		{
			// Create Editor Renderer
			m_editor.m_renderer = e->add<Renderer>(
				m_editor.m_model,
				m_editor.m_material,
				RenderStates {}
			);
			m_editor.m_renderer->states().cull().enabled = false;
		}

		// Create Skybox Entity
		if (Entity * e = m_editor.m_skybox.entity.create())
		{
			// Create Skybox Renderer
			m_editor.m_skybox.renderer = e->add<Renderer>(
				m_editor.m_skybox.model, 
				m_editor.m_skybox.material, 
				RenderStates { {}, {}, {}, DepthState { true, false } }
			);
			m_editor.m_skybox.renderer->setEnabled(false);
		}

		// Setup Source Editors
		m_editor.generate_sources();
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		// Update Surfaces Viewports
		for (auto & surf : m_pipeline) { surf->update(m_editor.m_scene.m_viewport); }
	}

	void Noobs::onDraw(const DrawEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Update Camera
		m_editor.m_camera.setClearFlags(Camera::SolidColor);
		m_editor.m_camera.setProjection(Camera::Perspective);
		m_editor.m_camera.setFieldOfView(45.0f);
		m_editor.m_camera.setClipNear(0.001f);
		m_editor.m_camera.setClipFar(1000.0f);
		m_editor.m_camera.setViewport({ 0, 0 }, (vec2i)m_editor.m_scene.m_viewport);
		m_editor.m_camera.setBackground(m_editor.m_scene.m_clearColor);

		// Render Scene
		m_pipeline[Surf_Main]->render_to([&]()
		{
			// Apply Camera
			m_editor.m_camera.apply();

			// Draw Skybox
			ev.window.draw(m_editor.m_skybox.renderer);

			// Draw Renderer
			ev.window.draw(m_editor.m_renderer);

			// Draw Geometry
			static Ref<Material> geo { "geometry" };
			if (geo)
			{
				geo->bind();
				ML_GL.drawArrays(m_editor.m_geoMode, 0, m_editor.m_geoCount);
				geo->unbind();
			}
		});

		/* * * * * * * * * * * * * * * * * * * * */

		// Reset States
		RenderStates {
			AlphaState	{ true, GL::Greater, 0.01f },
			BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
			CullState	{ false },
			DepthState	{ false },
		}();

		/* * * * * * * * * * * * * * * * * * * * */

		// Render Post Processing
		m_pipeline[Surf_Post]->render_to([&]()
		{
			// Apply Effects to Main
			if (Surface * surf { m_pipeline[Surf_Main] })
			{
				ev.window.draw(surf);
			}
		});

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onGui(const GuiEvent & ev)
	{
		// Render Scene
		m_editor.m_scene.render(ev, "Display##Noobs##DemoView", m_pipeline[Surf_Post]);
		
		// Render Editor
		m_editor.render(ev, "Editor##Noobs##DemoEditor");
	}

	void Noobs::onExit(const ExitEvent & ev)
	{
		Debug::log("Goodbye!");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Noobs::DemoScene::render(const GuiEvent & ev, C_String title, const Surface * surf)
	{
		if (!m_open) return;
		ImGui::PushID("Noobs");
		ImGui::PushID("Demo Scene");
		if (ImGui::Begin(title, &m_open))
		{
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
	
	void Noobs::DemoEditor::render(const GuiEvent & ev, C_String title)
	{
		if (!m_open) return;
		ImGui::PushID(ML_ADDRESSOF(this));
		if (ImGui::Begin(title, &m_open, ImGuiWindowFlags_None))
		{
			if (ImGui::BeginTabBar("DemoEditor##TabBar##Main"))
			{
				// Code Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Code"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID("##Code");
					ImGui::BeginChild("##Code##Content", { 0, 0, }, true,
						ImGuiWindowFlags_NoScrollbar
					);

					/* * * * * * * * * * * * * * * * * * * * */

					const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

					ImGui::BeginChild(
						"DemoFile##ContentArea", 
						{ max_size[0], max_size[1] * 0.95f }, 
						true
					);
					
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
									file->text.Render(
										("##DemoFile##" + file->name + "##ImGuiColorTextEdit").c_str(),
										{ 0, 0 }, 
										true
									);

									if (file->text.IsTextChanged())
									{
										file->dirty = true;
									}

									ImGui::EndTabItem();
								}
							}
						}
						ImGui::EndTabBar();
					}

					ImGui::EndChild();

					/* * * * * * * * * * * * * * * * * * * * */

					// Compile
					if (ImGui::BeginChild(
						"DemoFile##Tools",
						ImGui::GetContentRegionAvail(),
						true,
						ImGuiWindowFlags_NoScrollbar
					))
					{
						if (ImGui::Button("Compile"))
						{
							this->compile_sources();
						}
						ImGuiExt::Tooltip("Build shader source code (Ctrl+S)");

						ImGui::SameLine();

						// Toggle Files
						for (size_t i = 0; i < m_files.size() - 1; i++)
						{
							if (!m_files[i]) { continue; }
							if (i > 0) { ImGui::SameLine(); }
							ImGui::Checkbox(m_files[i]->name.c_str(), &m_files[i]->open);
						}
					}
					ImGui::EndChild();

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
					if (PropertyDrawer<Uniform>()("New Uniform##Noobs", (Uniform *&)to_add))
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
						ImGui::Text(detail::name_of((Uniform::Type)uni->id)); 
						ImGui::NextColumn();

						// Uniform Value
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushID(label.c_str());
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

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					// Select Material
					const Material * mtl = m_material;
					if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
					{
						if ((m_material.get() != mtl) &&
							m_material.update(
								ML_Content.get_name<Material>(mtl),
								std::remove_cv_t<Material *>(mtl)
							))
						{
							m_renderer->setMaterial(mtl);
							this->reset_sources();
							this->generate_sources();
						}
					}
					ImGuiExt::Tooltip("Specifies the targeted material");

					// Select Shader
					const Shader * shd = m_material->shader();
					if (PropertyDrawer<Shader>()("Shader##Material##Noobs", shd))
					{
						m_material->shader() = shd;
						this->reset_sources();
						this->generate_sources();
					}
					ImGuiExt::Tooltip("Specifies the targeted shader");

					// Select Model
					const Model * mdl = (const Model *)m_renderer->drawable();
					if (PropertyDrawer<Model>()("Model##Renderer##Noobs", mdl))
					{
						m_renderer->setDrawable(mdl);
					}
					ImGuiExt::Tooltip("Specifies model to be drawn");

					// Get Video Modes
					static const List<VideoMode> & mode_values {
						Window::getFullscreenModes()
					};

					// Get Video Names
					static const List<String> & mode_names = [&]
					{
						static List<String> temp {
							"Free"
						};
						for (const VideoMode & elem : mode_values)
						{
							temp.push_back(String("{0}").format(elem.size));
						}
						return temp;
					}();

					// Set Viewport
					static int32_t index = 0;
					if (ImGuiExt::Combo("Viewport", &index, mode_names))
					{
						m_scene.m_freeAspect = (index == 0);
					}
					if (!m_scene.m_freeAspect)
					{
						m_scene.m_viewport = (vec2i)mode_values[index - 1].size;
					}

					// Set Clear Color
					ImGui::ColorEdit4("Clear Color", &m_scene.m_clearColor[0]);

					// Geometry Settings
					if (m_files[DemoFile::Geom]->open)
					{
						ImGui::Separator();

						int32_t count { m_geoCount };
						if (ImGui::InputInt("Geometry Count", &count, 1, 100))
						{
							if (count >= 0) { m_geoCount = count; }
						}

						int32_t mode { GL::index_of(m_geoMode) };
						if (ImGuiExt::Combo(
							"Geometry Mode", &mode, GL::Mode_names, ML_ARRAYSIZE(GL::Mode_names)))
						{
							m_geoMode = GL::value_at<GL::Mode>(mode);
						}
					}

					ImGui::Separator();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					// Alpha State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](AlphaState * alpha)
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
						ImGui::Text(GL::raw_name_of(GL::AlphaTest));
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
						if (ImGuiExt::Combo("##Predicate", &predicate, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							alpha->predicate = GL::value_at<GL::Predicate>(predicate);
						}
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

						ImGui::DragFloat("##Coefficient", &alpha->coeff);
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
					})(&m_renderer->states().alpha());


					// Blend State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](BlendState * blend)
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
						ImGui::Text(GL::raw_name_of(GL::Blend));
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
						if (ImGuiExt::Combo("##SrcRGB", &srcRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->srcRGB = GL::value_at<GL::Factor>(srcRGB);
						}
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
						if (ImGuiExt::Combo("##DstRGB", &dstRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dstRGB = GL::value_at<GL::Factor>(dstRGB);
						}
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
						if (ImGuiExt::Combo("##SrcAlpha", &srcAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->srcAlpha = GL::value_at<GL::Factor>(srcAlpha);
						}
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
						if (ImGuiExt::Combo("##DstAlpha", &dstAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dstAlpha = GL::value_at<GL::Factor>(dstAlpha);
						}
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
					})(&m_renderer->states().blend());


					// Cull State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](CullState * cull)
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
						ImGui::Text(GL::raw_name_of(GL::CullFace));
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
						if (ImGuiExt::Combo("##Face", &face, GL::Face_raw_names, ML_ARRAYSIZE(GL::Face_raw_names)))
						{
							cull->face = GL::value_at<GL::Face>(face);
						}
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
					})(&m_renderer->states().cull());


					// Depth State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](DepthState * depth)
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
						ImGui::Text(GL::raw_name_of(GL::DepthTest));
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox("glDepthMask", &depth->mask); 
						ImGuiExt::Tooltip("Specifies whether the depth buffer is enabled for writing");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::Text(depth->mask ? "true" : "false");
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
						if (ImGuiExt::Combo("##Predicate", &predicate, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							depth->predicate = GL::value_at<GL::Predicate>(predicate);
						}
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
					})(&m_renderer->states().depth());

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
	
	bool Noobs::DemoEditor::dispose()
	{
		for (size_t i = 0; i < m_files.size(); i++)
		{
			if (m_files[i])
			{
				delete m_files[i];
			}
		}
		return true;
	}

	void Noobs::DemoEditor::generate_sources()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto setup_file = [&](DemoFile::Type type, const String & src)
		{
			if (m_material && m_material->shader())
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

		if (!m_material) return;

		if (auto vert = setup_file(DemoFile::Vert, m_material->shader()->sources().vs))
		{
			vert->open = m_material->shader()->sources().vs;
		}

		if (auto frag = setup_file(DemoFile::Frag, m_material->shader()->sources().fs))
		{
			frag->open = m_material->shader()->sources().fs;
		}

		if (auto geom = setup_file(DemoFile::Geom, m_material->shader()->sources().gs))
		{
			geom->open = m_material->shader()->sources().gs;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	bool Noobs::DemoEditor::compile_sources()
	{
		if (m_material && m_material->shader())
		{
			if (Shader * s = std::remove_cv_t<Shader *>(m_material->shader()))
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