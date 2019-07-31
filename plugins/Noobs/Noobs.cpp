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
	return new ml::Noobs { eventSystem };
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(EventSystem & eventSystem)
		: EditorPlugin	{ eventSystem }
		, m_pipeline	{}
		, m_editor		{}
		, m_scene		{}
		, m_skybox		{}
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
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value->as<MainMenuBarEvent>())
			{
				switch (ev->menu)
				{
				case MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Scene##Enable##Noobs##DemoScene", "", &m_scene.is_open());
					ImGui::MenuItem("Editor##Enable##Noobs##DemoEditor", "", &m_editor.is_open());
					break;
				}
			}
			break;

		case BuildDockspaceEvent::ID:
			if (auto ev = value->as<BuildDockspaceEvent>())
			{
				EditorDockspace & d = ev->dockspace;
				d.dockWindow("Scene##Noobs##DemoScene", d.getNode(d.LeftUp));
				d.dockWindow("Editor##Noobs##DemoEditor", d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onStart(const StartEvent & ev)
	{
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
			m_editor.renderer() = e->add<Renderer>(m_editor.model(), m_editor.material(),
				RenderStates { {
					new AlphaState	{ true, GL::Greater, 0.01f },
					new BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
					new CullState	{ true, GL::Back },
					new DepthState	{ true, GL::Less, true }
				} });
		}

		// Setup Editor
		m_editor.generate_sources();
	}

	void Noobs::onUpdate(const UpdateEvent & ev)
	{
		// Update Viewports
		if (m_scene.autoView()) { m_scene.viewport() = ev.window.getFrameSize(); }
		
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

			// Draw Renderer
			ev.window.draw(m_editor.renderer());
		});

		/* * * * * * * * * * * * * * * * * * * * */

		// Apply Default States
		static RenderStates states
		{ {
			new AlphaState	{ true, GL::Greater, 0.01f },
			new BlendState	{ true	},
			new CullState	{ false },
			new DepthState	{ false },
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
	}


	// DEMO SCENE
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
					// Get Video Settings
					static const List<VideoMode> & video_modes { VideoMode::get_modes() };
					static const List<String> & video_names = [&]
					{
						static List<String> temp { "Auto" };
						for (const VideoMode & video : video_modes)
						{
							temp.push_back(String("{0}").format(video.resolution));
						}
						return temp;
					}();

					// Viewport
					static int32_t index = 0;
					if (ImGui::Combo("Viewport",
						&index, 
						ML_EditorUtility.vector_getter,
						(void *)&video_names,
						(int32_t)video_names.size()
					))
					{
						m_autoView = (index == 0);
					}
					if (!m_autoView)
					{
						m_viewport = (vec2i)VideoMode::get_modes()[index - 1].resolution;
					}

					// Clear Color
					ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);

					// Effect Mode
					UniformPropertyDrawer()("Effect Mode", (Uni &)m_effectMode);
					ImGui::SameLine(); ImGui::Text("Effect Mode");

					// Kernel
					UniformPropertyDrawer()("Kernel", (Uni &)m_kernel);
					ImGui::SameLine(); ImGui::Text("Kernel");

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			if (surf && (*surf))
			{
				const vec2 src = surf->size();
				const vec2 dst = ML_EditorUtility.getWindowSize();
				const vec2 off = { 1.0f, 0.8f };
				const vec2 scl = alg::scale_to_fit(src, dst * off);
				const vec2 pos = ((dst - scl) * 0.5f) * off;

				ImGui::BeginChild(
					"Viewport", { 0, 0 }, true, ImGuiWindowFlags_NoScrollWithMouse
				);
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(
					surf->get_handle(), 
					{ scl[0], scl[1] }, 
					{ 0, 1 }, 
					{ 1, 0 }
				);
				ImGui::EndChild();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}
		ImGui::End();
		ImGui::PopID();
		ImGui::PopID();
	}


	// DEMO FILE
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Noobs::DemoFile::render(const GuiEvent & ev)
	{
		ImGui::BeginChild("DemoFile##ContentArea", { 0, 0 }, true);
		
		this->text.Render(("##DemoFile##" + this->name + "##TextEditor").c_str());

		if (this->text.IsTextChanged())
		{ 
			this->dirty = true; 
		}

		ImGui::EndChild();
	}


	// DEMO EDITOR
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Noobs::DemoEditor::render(const GuiEvent & ev, C_String title)
	{
		if (!m_open) return;
		ImGui::PushID("Noobs");
		ImGui::PushID("Demo Editor");
		if (ImGui::Begin(title, &m_open, ImGuiWindowFlags_None))
		{
			if (ImGui::BeginTabBar("DemoEditor##TabBar##Main"))
			{
				// Sources Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Sources"))
				{
					// Compile
					if (ImGui::Button("Compile"))
					{
						this->compile_sources();
					}
					ImGui::Separator();

					// Toggle Demo Files (start at one to always show config)
					for (size_t i = 1; i < m_files.size(); i++)
					{
						if (!m_files[i]) { continue; }
						if (i > 1) { ImGui::SameLine(); }
						ImGui::Checkbox(m_files[i]->name.c_str(), &m_files[i]->open);
					}

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
					ImGui::EndTabItem();
				}

				// Uniforms Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Uniforms"))
				{
					// New Uniform Popup
					Uni * to_add { nullptr };
					if (UniformPropertyDrawer()("##New##Uni", (Uni *&)to_add))
					{
						// Already Exists
						if (!m_material->add(to_add))
						{
							delete to_add;
							Debug::logError("A uniform with that name already exists");
						}
					}
					ImGui::Separator();

					// Header Columns
					ImGui::Columns(3, "##Uni##Columns");
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Value"); ImGui::NextColumn();
					ImGui::Separator();
					ImGui::Columns(1);

					// Uniform List
					/* * * * * * * * * * * * * * * * * * * * */
					Uni * to_remove { nullptr };
					
					for (Uni *& uni : (*m_material))
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
								ImGuiInputTextFlags_EnterReturnsTrue |
								(uni->isModifiable()
									? ImGuiInputTextFlags_None
									: ImGuiInputTextFlags_ReadOnly)
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
						ImGui::Text("%s", detail::nameOf((Uni::Type)uni->id)); 
						ImGui::NextColumn();

						// Uniform Value
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushID(label.c_str());
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						if (UniformPropertyDrawer()(label, (Uni &)(*uni)))
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

					ImGui::EndTabItem();
				}

				// Settings Tab
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Settings"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					const Shader * s = this->shader();
					if (ShaderPropertyDrawer()("Shader##Material##Noobs", s))
					{
						this->shader() = s;
						reset_sources();
						generate_sources();
					}
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("The shader targeted by this editor.");

					/* * * * * * * * * * * * * * * * * * * * */

					const Model * m = (const Model *)m_renderer->drawable();
					if (ModelPropertyDrawer()("Model##Renderer##Noobs", m))
					{
						m_renderer->drawable() = m;
					}
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("The model to be drawn.");

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					if (AlphaState * alpha = m_renderer->states().get<AlphaState>())
					{
						if (ImGui::CollapsingHeader("Alpha"))
						{
							bool & enabled = alpha->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Alpha Testing##Renderer##Noobs"
								: "Disabled##Alpha Testing##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(enabled
								? ("glEnable(GL_ALPHA_TEST)")
								: ("glDisable(GL_ALPHA_TEST)")
							);

							ML_EditorUtility.HelpMarker("glAlphaFunc");

							int32_t i = GL::indexOf(alpha->comp);
							if (ImGui::Combo(
								"Comparison##Alpha Testing##Renderer##Noobs",
								&i,
								GL::Comp_names,
								ML_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(i, alpha->comp);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies the alpha comparison function.\n"
							);

							ImGui::DragFloat("Coeff##Alpha Testing##Renderer##Noobs", &alpha->coeff);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies the reference value that incoming alpha values are compared to.\n"
								"This value is clamped to the range 0 1 , where 0 represents the lowest possible alpha value and 1 the highest possible value.\n"
							);
						}
					}
					
					/* * * * * * * * * * * * * * * * * * * * */

					if (BlendState * blend = m_renderer->states().get<BlendState>())
					{
						if (ImGui::CollapsingHeader("Blend"))
						{
							bool & enabled = blend->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Blending##Renderer##Noobs"
								: "Disabled##Blending##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(enabled
								? ("glEnable(GL_BLEND)")
								: ("glDisable(GL_BLEND)")
							);

							auto factor_combo = [](C_String label, int32_t & i)
							{
								return ImGui::Combo(
									label,
									&i,
									GL::Factor_names,
									ML_ARRAYSIZE(GL::Factor_names)
								);
							};

							ML_EditorUtility.HelpMarker("glBlendFuncSeparate");

							int32_t srcRGB = GL::indexOf(blend->srcRGB);
							if (factor_combo("Src RGB##Blending##Renderer##Noobs", srcRGB))
							{
								GL::valueAt(srcRGB, blend->srcRGB);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies how the red, green, and blue blending factors are computed."
							);

							int32_t srcAlpha = GL::indexOf(blend->srcAlpha);
							if (factor_combo("Src Alpha##Blending##Renderer##Noobs", srcAlpha))
							{
								GL::valueAt(srcAlpha, blend->srcAlpha);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specified how the alpha source blending factor is computed."
							);

							int32_t dstRGB = GL::indexOf(blend->dstRGB);
							if (factor_combo("Dst RGB##Blending##Renderer##Noobs", dstRGB))
							{
								GL::valueAt(dstRGB, blend->dstRGB);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies how the red, green, and blue destination blending factors are computed."
							);

							int32_t dstAlpha = GL::indexOf(blend->dstAlpha);
							if (factor_combo("Dst Alpha##Blending##Renderer##Noobs", dstAlpha))
							{
								GL::valueAt(dstAlpha, blend->dstAlpha);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specified how the alpha destination blending factor is computed."
							);
						}
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (CullState * cullFace = m_renderer->states().get<CullState>())
					{
						if (ImGui::CollapsingHeader("Cull Face"))
						{
							bool & enabled = cullFace->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Culling##Renderer##Noobs"
								: "Disabled##Culling##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(enabled
								? ("glEnable(GL_CULL_FACE)")
								: ("glDisable(GL_CULL_FACE)")
							);

							ML_EditorUtility.HelpMarker("glCullFace");

							int32_t i = GL::indexOf(cullFace->face);
							if (ImGui::Combo(
								"Face##Culling##Renderer##Noobs",
								&i,
								GL::Face_names,
								ML_ARRAYSIZE(GL::Face_names)
							))
							{
								GL::valueAt(i, cullFace->face);
							}
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(
								"Specifies whether front- or back-facing facets are candidates for culling."
							);
						}
					}

					/* * * * * * * * * * * * * * * * * * * * */

					if (DepthState * depth = m_renderer->states().get<DepthState>())
					{
						if (ImGui::CollapsingHeader("Depth"))
						{
							bool & enabled = depth->enabled;
							ImGui::Checkbox((enabled
								? "Enabled ##Depth Testing##Renderer##Noobs"
								: "Disabled##Depth Testing##Renderer##Noobs"
								),
								&enabled
							);
							ImGui::SameLine();
							ML_EditorUtility.HelpMarker(enabled
								? ("glEnable(GL_DEPTH_TEST)")
								: ("glDisable(GL_DEPTH_TEST)")
							);

							ML_EditorUtility.HelpMarker("glDepthFunc");

							int32_t i = GL::indexOf(depth->comp);
							if (ImGui::Combo(
								"Comparison##Depth Testing##Renderer##Noobs",
								&i,
								GL::Comp_names,
								ML_ARRAYSIZE(GL::Comp_names)
							))
							{
								GL::valueAt(i, depth->comp);
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
					}
					

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
		ImGui::PopID();
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
					m_files[type] = new DemoFile(type, DemoFile::Names[type], src);
				}
				else
				{
					m_files[type]->text.SetText(src);
				}

				if ((type != DemoFile::Conf) && src)
				{
					m_files[DemoFile::Conf]->text.SetText(([&]()
					{
						SStream ss; ss
							<< m_files[DemoFile::Conf]->text.GetText()
							<< DemoFile::Tags[type] << endl
							<< DemoFile::Incl[type] << endl;
						return ss.str();
					})());
				}

				return m_files[type];
			}
			return (DemoFile *)nullptr;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (auto conf = setup_file(DemoFile::Conf, String()))
		{
			conf->open = true;
		}

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

				return s->loadFromMemory(
					this->parse_sources(m_files[DemoFile::Conf]->text.GetText())
				);
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

	String Noobs::DemoEditor::parse_sources(const String & source) const
	{
		SStream out;
		SStream sstr { source };
		String	line;
		while (std::getline(sstr, line))
		{
			if (line.find("#include") != String::npos)
			{
				bool found { false };
				String name {};
				if (ShaderParser::parseWrapped(line, '\"', '\"', name))
				{
					// Search File List
					for (const DemoFile * elem : m_files)
					{
						if (elem && (elem->name == name) && (elem->open))
						{
							out << this->parse_sources(elem->text.GetText());
							found = true;
							break;
						}
					}

					// Search Content
					const Shader * shader { nullptr };
					if (!found && (found = (shader = ML_Content.get<Shader>(name))))
					{
						if (const auto & src = shader->sources().vs)
							out << DemoFile::Tags[DemoFile::Vert] << endl << src << endl;
						
						if (const auto & src = shader->sources().fs)
							out << DemoFile::Tags[DemoFile::Frag] << endl << src << endl;
						
						if (const auto & src = shader->sources().gs)
							out << DemoFile::Tags[DemoFile::Geom] << endl << src << endl;
					}
				}

				if (!found) { out << line << endl; }
			}
			else
			{
				out << line << endl;
			}
		}
		return (String)out.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}