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
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/GraphicsEvents.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ML/Window/WindowEvents.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main() { return new ml::Noobs {}; }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs() : Plugin {}
	{
		ML_EventSystem.addListener<StartEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<DrawEvent>(this);
		ML_EventSystem.addListener<GuiEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
		ML_EventSystem.addListener<MainMenuBarEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<ShaderErrorEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEvent(const Event & value)
	{
		switch (*value)
		{
		case StartEvent::ID	: return onStart(*value.as<StartEvent>());
		case UpdateEvent::ID: return onUpdate(*value.as<UpdateEvent>());
		case DrawEvent::ID	: return onDraw(*value.as<DrawEvent>());
		case GuiEvent::ID	: return onGui(*value.as<GuiEvent>());
		case UnloadEvent::ID: return onUnload(*value.as<UnloadEvent>());

		case ShaderErrorEvent::ID:
			if (auto ev = value.as<ShaderErrorEvent>())
			{
				if (ev->obj && (ev->obj == m_renderer->shader()))
				{
					// Decode Errors
					SStream ss { String{ ev->error } };
					String line;
					while (std::getline(ss, line, '\n'))
					{
						auto err { decode_error(ev->type, line) };
						switch (ev->type)
						{
						case GL::VertexShader	: m_files[DemoFile::Vert]->errs.push_back(err); break;
						case GL::FragmentShader	: m_files[DemoFile::Frag]->errs.push_back(err); break;
						case GL::GeometryShader	: m_files[DemoFile::Geom]->errs.push_back(err); break;
						}
					}

					// Set Markers
					for (auto & file : m_files)
					{
						TextEditor::ErrorMarkers markers;
						for (auto & err : file->errs)
						{
							markers.insert({ err.line, err.code + ": " + err.desc });
						}
						file->text.SetErrorMarkers(markers);
					}
				}
			}
			break;

		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>())
			{
				// Toggle Fullscreen
				if (ev->getPress(KeyCode::F11))
				{
					ML_EventSystem.fireEvent<WindowFullscreenEvent>(-1);
				}

				// Refresh Sources
				if (ev->getPress(KeyCode::F5))
				{
					reset_sources();
					generate_sources();
				}

				// Compile Sources
				if (ev->isSave())
				{
					compile_sources();
				}
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value.as<MainMenuBarEvent>())
			{
				switch (ev->submenu)
				{
				case MainMenuBarEvent::Window:
					ImGui::MenuItem("Display##Enable##Noobs##DemoView", "", &m_display_open);
					ImGui::MenuItem("Editor##Enable##Noobs##DemoEditor", "", &m_editor_open);
					break;
				}
			}
			break;

		case DockspaceEvent::ID:
			if (auto ev = value.as<DockspaceEvent>())
			{
				EditorDockspace & d { ev->dockspace };
				d.dockWindow("Display##Noobs##DemoView", d.getNode(d.LeftUp));
				d.dockWindow("Editor##Noobs##DemoEditor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onStart(const StartEvent & ev)
	{
		// Setup Editor
		if (const String ent_name { ML_Engine.prefs().get_string("Noobs", "demo_entity", "") })
		{
			if (Entity * ent { m_entity.update(ML_Content.get<Entity>(ent_name)) })
			{
				m_renderer = ent->get<Renderer>();
			}
		}
		generate_sources();
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Capture Camera
		Camera * camera { Camera::mainCamera() };

		/* * * * * * * * * * * * * * * * * * * * */

		// Update Viewports
		if (camera && (*camera))
		{
			camera->setViewport((vec2i)m_viewport);

			for (auto & surf : m_pipeline)
			{
				surf->update((vec2)camera->viewport().size());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		// Update Materials
		for (auto & pair : ML_Content.data<Material>())
		{
			if (auto m { (Material *)pair.second })
			{
				m->set<uni_vec2>("u_cursor", ML_Engine.window().getCursorPos());
				m->set<uni_float>("u_delta", ML_Engine.time().deltaTime());
				m->set<uni_int>("u_frame", ML_Engine.time().frameCount());
				m->set<uni_float>("u_fps", ML_Engine.time().frameRate());
				m->set<uni_float>("u_time", ML_Engine.time().deltaTime());

				if (camera && (*camera))
				{
					m->set<uni_vec3>("u_camera.pos", camera->position());
					m->set<uni_vec3>("u_camera.dir", camera->direction());
					m->set<uni_float>("u_camera.fov", camera->fieldOfView());
					m->set<uni_float>("u_camera.near", camera->clipNear());
					m->set<uni_float>("u_camera.far", camera->clipFar());
					m->set<uni_vec2>("u_camera.view", (vec2)camera->viewport().size());
					m->set<uni_vec2>("u_viewport", (vec2)camera->viewport().size());
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onDraw(const DrawEvent & ev)
	{
		// Get Camera
		const Camera * camera { Camera::mainCamera() };

		// Render Main Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_pipeline[Surf_Main])
		{
			// Bind Surface
			m_pipeline[Surf_Main]->bind();

			// Apply Camera
			if (camera) (*camera)
				.applyClear()
				.applyViewport()
				;

			// Draw Renderers
			for (auto & pair : ML_Content.data<Entity>())
			{
				if (auto ent { (Entity *)pair.second })
				{
					auto renderer { ent->get<Renderer>() };
					auto transform { ent->get<Transform>() };
					if (renderer && (*renderer) && transform && (*transform))
					{
						if (auto m { (Material *)renderer->material() })
						{
							m->set<uni_vec3>("u_position", transform->position());
							m->set<uni_vec3>("u_rotation", transform->rotation());
							m->set<uni_vec3>("u_scale", transform->scale());
						}
					}
					ML_Engine.window().draw(renderer);
				}
			}

			// Unbind Surface
			m_pipeline[Surf_Main]->unbind();
		}

		// Reset States
		/* * * * * * * * * * * * * * * * * * * * */
		RenderStates {
			AlphaState	{ true, GL::Greater, 0.01f },
			BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
			CullState	{ false },
			DepthState	{ false },
		}();

		// Render Post Processing
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_pipeline[Surf_Post])
		{
			// Bind Surface
			m_pipeline[Surf_Post]->bind();

			// Apply Camera
			if (camera) (*camera)
				.applyClear()
				.applyViewport()
				;

			// Draw Scene Output
			ML_Engine.window().draw(m_pipeline[Surf_Main]);

			// Unbind Surface
			m_pipeline[Surf_Post]->unbind();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::onGui(const GuiEvent & ev)
	{
		// Render Scene
		draw_display("Display##Noobs##DemoView", m_pipeline[Surf_Post]);

		// Render Editor
		draw_editor("Editor##Noobs##DemoEditor");
	}

	void Noobs::onUnload(const UnloadEvent & ev)
	{
		dispose_files();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Noobs::compile_sources()
	{
		if (m_renderer->material() && m_renderer->material()->shader())
		{
			if (Shader * s { m_renderer->shader() })
			{
				for (auto & f : m_files)
				{
					f->dirty = false;
					f->errs.clear();
					f->text.SetErrorMarkers({});
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

	void Noobs::draw_display(C_String title, const Surface * surf)
	{
		if (!m_display_open) return;
		ImGui::PushID("Noobs");
		ImGui::PushID("Demo Scene");
		if (ImGui::Begin(title, &m_display_open))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (surf && (*surf))
			{
				const vec2 dst { ImGuiExt::GetContentRegionAvail() };
				const vec2 scl { alg::scale_to_fit((vec2)surf->size(), dst) * 0.975f };
				const vec2 pos { ((dst - scl) * 0.5f) };
				if (m_freeAspect) { m_viewport = dst; }
				ImGui::BeginChild("NoobsSceneViewport", { 0, 0 }, true);
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(surf->texture().get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}
		ImGui::End();
		ImGui::PopID();
		ImGui::PopID();
	}
	
	void Noobs::draw_editor(C_String title)
	{
		if (!m_editor_open) return;
		ImGui::PushID(ML_ADDRESSOF(this));
		if (ImGui::Begin(title, &m_editor_open, ImGuiWindowFlags_None))
		{
			if (ImGui::BeginTabBar("DemoEditor##TabBar##Main"))
			{
				// Code Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Code"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID("##Code");
					ImGui::BeginChild(
						"##Code##Content", 
						{ 0, 0, }, 
						false,
						ImGuiWindowFlags_NoScrollbar
					);

					const vec2 max_size { ImGuiExt::GetContentRegionAvail() };
					const float_t tools_height { ImGuiExt::GetTextLineHeightWithSpacing() * 1.5f };
					
					// Text Editors
					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::BeginChildFrame(
						ImGui::GetID("DemoFile##TextEditors"),
						{ max_size[0], max_size[1] - tools_height },
						ImGuiWindowFlags_NoScrollbar
					) && ImGui::BeginTabBar("Demo File Tab Bar"))
					{
						// Demo File Tab Bar
						for (size_t i = 0; i < m_files.size(); i++)
						{
							auto & file { m_files[i] };
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
										("##DemoFile##" + file->name + "##TextEditor").c_str(),
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
					ImGui::EndChildFrame();

					// Toolbar
					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::BeginChildFrame(
						ImGui::GetID("DemoFile##Toolbar"),
						{ max_size[0], 0 },
						ImGuiWindowFlags_NoScrollbar
					))
					{
						if (ImGui::Button("Compile"))
						{
							compile_sources();
						}
						ImGuiExt::Tooltip("Build shader source code (Ctrl+S)");

						ImGui::SameLine();

						// Toggle Files
						for (size_t i = 0; i < m_files.size(); i++)
						{
							if (!m_files[i]) { continue; }
							if (i > 0) { ImGui::SameLine(); }
							ImGui::Checkbox(m_files[i]->name.c_str(), &m_files[i]->open);
						}
					}
					ImGui::EndChildFrame();

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
					ImGui::BeginChildFrame(ImGui::GetID("##Uniforms##Content"), { 0, 0 }, 0);

					/* * * * * * * * * * * * * * * * * * * * */

					// New Uniform Popup
					Uniform * to_add { nullptr };
					if (PropertyDrawer<Uniform>()("New Uniform##Noobs", (Uniform *&)to_add))
					{
						// Already Exists
						if (to_add && !m_renderer->material()->insert(to_add))
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
					for (auto & uni : (*m_renderer->material()))
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
								if (!m_renderer->material()->get(name))
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
						
						// Uniform FileType
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
							if (ImGuiExt::Confirm(
								"Delete Uniform",
								ImGui::Button(("X##" + label).c_str()),
								"Are you sure you want to delete this Uniform?"
							) == 1)
							{
								to_remove = uni;
							}
							ImGuiExt::Tooltip("Delete this uniform");
						}
						ImGui::PopID();
						ImGui::Columns(1);
						ImGui::Separator();
					}
					if (to_remove) { m_renderer->material()->erase(to_remove); }

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChildFrame();
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
					ImGui::BeginChildFrame(ImGui::GetID("##Settings##Content"), { 0, 0 }, 0);

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					// Select Material
					const Material * mtl { m_renderer ? m_renderer->material() : nullptr };
					if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
					{
						m_renderer->setMaterial(mtl);
						reset_sources();
						generate_sources();
					}
					ImGuiExt::Tooltip("Materials specify the active shader and its corresponding uniforms");

					// Select Shader
					const Shader * shd { m_renderer ? m_renderer->shader() : nullptr };
					if (PropertyDrawer<Shader>()("Shader##Renderer##Noobs", shd))
					{
						m_renderer->setShader(shd);
						reset_sources();
						generate_sources();
					}
					ImGuiExt::Tooltip("Specify the target material's shader");

					// Select Model
					const Model * mdl { m_renderer ? m_renderer->model() : nullptr };
					if (PropertyDrawer<Model>()("Model##Renderer##Noobs", mdl))
					{
						m_renderer->setModel(mdl);
					}
					ImGuiExt::Tooltip("Specify the geometry to be drawn");

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
							temp.push_back(util::to_string(elem.size));
						}
						return temp;
					}();

					// Camera Settings
					if (auto camera { Camera::mainCamera() })
					{
						// Set Viewport
						static int32_t index = 0;
						if (ImGuiExt::Combo("Viewport", &index, mode_names))
						{
							m_freeAspect = (index == 0);
						}
						if (!m_freeAspect)
						{
							m_viewport = (vec2i)mode_values[index - 1].size;
						}

						// Set Background
						vec4 background { camera->background() };
						if (ImGui::ColorEdit4("Background", &background[0]))
						{
							camera->setBackground(background);
						}
					}

					ImGui::Separator();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::BeginChildFrame(
						ImGui::GetID("NoobsRenderStates"),
						{ 0, 0 },
						ImGuiWindowFlags_None
					);

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
							util::to_hex<uint32_t>(alpha->predicate),
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
							util::to_hex<uint32_t>(blend->srcRGB),
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
							util::to_hex<uint32_t>(blend->dstRGB),
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
							util::to_hex<uint32_t>(blend->srcAlpha),
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
							util::to_hex<uint32_t>(blend->dstAlpha),
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
							util::to_hex<uint32_t>(cull->face),
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
							util::to_hex<uint32_t>(depth->predicate),
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

					ImGui::EndChildFrame();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChildFrame();
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

	bool Noobs::dispose_files()
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

	void Noobs::generate_sources()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto setup_file = [&](DemoFile::FileType type, const String & src)
		{
			if (m_renderer->material() && m_renderer->material()->shader())
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

		if (!m_renderer->material()) return;

		if (auto vert = setup_file(DemoFile::Vert, m_renderer->material()->shader()->sources().vs))
		{
			vert->open = m_renderer->material()->shader()->sources().vs;
		}

		if (auto frag = setup_file(DemoFile::Frag, m_renderer->material()->shader()->sources().fs))
		{
			frag->open = m_renderer->material()->shader()->sources().fs;
		}

		if (auto geom = setup_file(DemoFile::Geom, m_renderer->material()->shader()->sources().gs))
		{
			geom->open = m_renderer->material()->shader()->sources().gs;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::reset_sources()
	{
		for (auto & f : m_files)
		{
			if (!f) continue;
			f->open = false;
			f->text.SetText(String());
			f->text.SetErrorMarkers({});
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}