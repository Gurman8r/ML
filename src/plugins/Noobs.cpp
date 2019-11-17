/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Core/CoreEvents.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Entity.hpp>
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
#include <ML/Editor/AssetPreview.hpp>
#include <ImGuiColorTextEdit/TextEditor.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API Noobs final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ShaderError final : public Newable
		{
			String	file;
			int32_t line;
			String	code;
			String	desc;

			explicit ShaderError(uint32_t type, String str)
				: ShaderError { "", 0, "", "" }
			{
				if (!str || str.front() != '0') { return; }
				str.erase(str.begin());
				switch (type)
				{
				case GL::FragmentShader: this->file = "Fragment"; break;
				case GL::GeometryShader: this->file = "Geometry"; break;
				case GL::VertexShader: this->file = "Vertex"; break;
				default: this->file = "Unknown"; break;
				}
				size_t a, b;
				if ((a = str.find_first_of('(')) != String::npos)
				{
					if ((b = str.find_first_of(')', a + 1)) != String::npos)
					{
						this->line = util::to_i32(str.substr(a + 1, b - a - 1));

						if ((a = str.find_first_of(':', b)) != String::npos)
						{
							if ((b = str.find_first_of(':', a + 1)) != String::npos)
							{
								this->code = str.substr(a + 2, b - a - 2);
								this->desc = str.substr(b + 2);
							}
						}
					}
				}
			}

			ShaderError(const String & file, int32_t line, const String & code, const String & expr)
				: file{ file }
				, line{ line }
				, code{ code }
				, desc{ expr }
			{
			}

			ShaderError(const ShaderError & copy)
				: ShaderError{ copy.file, copy.line, copy.code, copy.desc }
			{
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using TextEditor = ImGui::TextEditor;

		struct ShaderFile final : public Newable, public NonCopyable
		{
			enum FileType : size_t { Frag, Vert, Geom, MAX_DEMO_FILE };

			static constexpr C_String Names[] = { "Fragment", "Vertex", "Geometry" };

			using Errors = typename List<ShaderError>;

			FileType	type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;
			Errors		errs;

			ShaderFile(FileType type)
				: type	{ type }
				, text	{}
				, name	{ Names[type] }
				, open	{ false }
				, dirty { false }
				, errs	{}
			{
# if (!ML_DEBUG)
				this->text.SetPalette(TextEditor::GetLightPalette());
# endif
				this->text.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Surf_Main, Surf_Post, MAX_DEMO_SURFACE };

		using DemoPipeline = typename Array<ptr_t<Surface>, MAX_DEMO_SURFACE>;

		enum class DisplayMode : int32_t { Automatic, Manual, Fixed };

		using FileArray = typename Array<ptr_t<ShaderFile>, ShaderFile::MAX_DEMO_FILE>;

		bool m_editor_open		{ true };
		bool m_display_open		{ true };
		bool m_use_main_camera	{ true };

		DemoPipeline	m_pipeline		{ 0 };
		ptr_t<Entity>	m_entity		{ 0 };
		String			m_ent_name		{ "" };
		FileArray		m_files			{ 0 };
		DisplayMode		m_displayMode	{ 0 };
		int32_t			m_displayIndex	{ 0 };

		static constexpr auto display_name { "Display##Noobs##DemoView" };
		static constexpr auto editor_name { "Editor##Noobs##DemoEditor" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Noobs() : Plugin {}
		{
			ML_EventSystem.addListener<StartEvent>(this);
			ML_EventSystem.addListener<UpdateEvent>(this);
			ML_EventSystem.addListener<DrawEvent>(this);
			ML_EventSystem.addListener<GuiEvent>(this);
			ML_EventSystem.addListener<UnloadEvent>(this);
			ML_EventSystem.addListener<KeyEvent>(this);
			ML_EventSystem.addListener<DockspaceEvent>(this);
			ML_EventSystem.addListener<ShaderErrorEvent>(this);
			ML_EventSystem.addListener<SecretEvent>(this);
		}

		~Noobs() {}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(const Event & value) override
		{
			switch (*value)
			{
			case StartEvent::ID: if (auto ev{ value.as<StartEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ML_Editor.mainMenuBar().addMenu("Window", [&]()
				{
					ImGui::Separator();
					ImGui::PushID(ML_ADDRESSOF(this));
					ImGui::MenuItem("Editor##Noobs", "", &m_editor_open);
					ImGui::MenuItem("Display##Noobs", "", &m_display_open);
					ImGui::PopID();
				});

				ML_Editor.mainMenuBar().addMenu("Plugins", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu("Noobs"))
					{
						draw_plugin_menu();
						ImGui::EndMenu();
					}
					ImGui::PopID();
				});

				m_pipeline[0] = ML_Engine.content().get<Surface>("surf/main");
					
				m_pipeline[1] = ML_Engine.content().get<Surface>("surf/post");
					
				if (m_ent_name = ML_Engine.prefs().get_string("Noobs", "target_entity", ""))
				{
					m_entity = ML_Engine.content().get<Entity>(m_ent_name);
				}

				generate_sources();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (auto camera{ Camera::mainCamera() }; camera && camera->enabled())
				{
					// Update Pipeline
					for (auto & surf : m_pipeline)
					{
						surf->update((vec2)camera->viewport().size());
					}

					// Update Camera Uniforms
					if (m_use_main_camera)
					{
						for (auto & [key, value] : ML_Engine.content().data<Material>())
						{
							if (auto m{ (Material *)value })
							{
								m->set<uni_vec3>("u_camera.pos", camera->position());
								m->set<uni_vec3>("u_camera.dir", camera->direction());
								m->set<uni_float>("u_camera.fov", camera->fieldOfView());
								m->set<uni_float>("u_camera.near", camera->clipNear());
								m->set<uni_float>("u_camera.far", camera->clipFar());
								m->set<uni_vec2>("u_camera.view", (vec2)camera->viewport().size());
							}
						}
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case DrawEvent::ID: if (auto ev{ value.as<DrawEvent>()})
			{
				/* * * * * * * * * * * * * * * * * * * * */

				auto camera{ Camera::mainCamera() };
					
				// Render Main Scene
				if (m_pipeline[Surf_Main] && m_pipeline[Surf_Main]->bind())
				{
					// Apply Camera
					if (camera) { camera->apply(); }
						
					// Draw Renderers
					for (auto & [key, value] : ML_Engine.content().data<Entity>())
					{
						if (auto e{ (Entity *)value })
						{
							if (auto r{ e->get<Renderer>() }; r && r->enabled())
							{
								if (auto t{ e->get<Transform>() }; t && t->enabled())
								{
									if (auto m{ (Material *)r->material() })
									{
										m->set<uni_vec3>("u_position", t->position());
										m->set<uni_vec4>("u_rotation", t->rotation());
										m->set<uni_vec3>("u_scale", t->scale());
									}
								}
								ML_Engine.window().draw(r);
							}
						}
					}

					// Unbind
					m_pipeline[Surf_Main]->unbind();
				}

				// Render Post Processing
				if (m_pipeline[Surf_Post] && m_pipeline[Surf_Post]->bind())
				{
					// Apply Camera
					if (camera) { camera->apply(); }
						
					// Reset States
					RenderStates{
						AlphaState	{ true, GL::Greater, 0.01f },
						BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
						CullState	{ false },
						DepthState	{ false },
					}();

					// Draw Scene Output
					ML_Engine.window().draw(m_pipeline[Surf_Main]);

					// Unbind
					m_pipeline[Surf_Post]->unbind();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case GuiEvent::ID: if (auto ev{ value.as<GuiEvent>()})
			{
				/* * * * * * * * * * * * * * * * * * * * */

				draw_display(display_name, m_pipeline[Surf_Post]);
				draw_editor(editor_name);

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>()})
			{
				/* * * * * * * * * * * * * * * * * * * * */

				dispose_files();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case ShaderErrorEvent::ID: if (auto ev{ value.as<ShaderErrorEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
				{
					if (auto r{ e ? e->get<Renderer>() : nullptr })
					{
						if (ev->obj && (ev->obj == r->shader()))
						{
							std::cout << ev->error;

							// Decode Errors
							SStream ss{ String{ ev->error } };
							String line;
							while (std::getline(ss, line, '\n'))
							{
								const ShaderError err{ ev->type, line };
								switch (ev->type)
								{
								case GL::VertexShader: m_files[ShaderFile::Vert]->errs.push_back(err); break;
								case GL::FragmentShader: m_files[ShaderFile::Frag]->errs.push_back(err); break;
								case GL::GeometryShader: m_files[ShaderFile::Geom]->errs.push_back(err); break;
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
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case KeyEvent::ID: if (auto ev{ value.as<KeyEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Toggle Fullscreen
				if (ev->getPress(KeyCode::F11))
				{
					ML_EventSystem.fireEvent<WindowFullscreenEvent>(-1);
				}

				// Refresh Sources
				if (ev->getPress(KeyCode::F5))
				{
					reset_sources().generate_sources();
				}

				// Compile Sources
				if (ev->isSave())
				{
					compile_sources();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (Editor_Dockspace & d{ ev->dockspace }; d.isOpen())
				{
					d.dockWindow(display_name, d.getNode(d.LeftUp));
					d.dockWindow(editor_name, d.getNode(d.RightUp));
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case SecretEvent::ID: if (auto ev{ value.as<SecretEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				Debug::execute("open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ");

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
		}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void Noobs::draw_display(C_String title, const Surface * surf)
		{
			if (!m_display_open) { return; }
			ImGui::PushID(ML_ADDRESSOF(this));
			if (ImGui::Begin(title, &m_display_open, 0))
			{
				ML_AssetPreview.drawPreview(surf, ImGuiExt::GetContentRegionAvail(), [&] 
				{
					if (m_displayMode == DisplayMode::Automatic)
					{ 
						if (auto c { Camera::mainCamera() })
						{
							c->setViewport((vec2i)ImGuiExt::GetContentRegionAvail());
						}
					}
				});
			}
			ImGui::End();
			ImGui::PopID();
		}
	
		void Noobs::draw_editor(C_String title)
		{
			if (!m_editor_open) { return; }
			ImGui::PushID(ML_ADDRESSOF(this));
			if (ImGui::Begin(title, &m_editor_open, 0))
			{
				if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
				{
					if (auto r{ e ? e->get<Renderer>() : nullptr })
					{
						if (ImGui::BeginTabBar("DemoEditor##TabBar##Main", ImGuiTabBarFlags_Reorderable))
						{
							if (ImGui::BeginTabItem("Code")) { draw_code_tab(); ImGui::EndTabItem(); }
							if (ImGui::BeginTabItem("Uniforms")) { draw_uniforms_tab(); ImGui::EndTabItem(); }
							if (ImGui::BeginTabItem("Settings")) { draw_settings_tab(); ImGui::EndTabItem(); }
							ImGui::EndTabBar();
						}
					}
					else
					{
						ImGui::Text("Nothing Selected");
					}
				}
			}
			ImGui::End();
			ImGui::PopID();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void Noobs::draw_code_tab()
		{
			ImGui::PushID("##Code");
			ImGui::BeginChild("##Code##Content", { 0, 0 }, false, ImGuiWindowFlags_NoScrollbar);
		
			if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
			{
				if (auto r { e ? e->get<Renderer>() : nullptr })
				{
					const vec2 max_size { ImGuiExt::GetContentRegionAvail() };
					const float_t tools_height { ImGui::GetTextLineHeightWithSpacing() * 1.5f };

					// Text Editors
					/* * * * * * * * * * * * * * * * * * * * */
					if (ImGui::BeginChildFrame(
						ImGui::GetID("ShaderFile##TextEditors"),
						{ max_size[0], max_size[1] - tools_height },
						ImGuiWindowFlags_NoScrollbar
					) && ImGui::BeginTabBar(
						"Demo File Tab Bar",
						ImGuiTabBarFlags_Reorderable
					))
					{
						// Demo File Tab Bar
						for (auto & file : m_files)
						{
							if (!file || !file->open) continue;

							ImGui::PushID(ML_ADDRESSOF(file));

							// Text Editor Tab
							const bool tab_open { ImGui::BeginTabItem(
								file->name.c_str(),
								nullptr,
								(file->dirty
									? ImGuiTabItemFlags_UnsavedDocument
									: ImGuiTabItemFlags_None)
							) };

							// Tab Context Item
							if (ImGui::BeginPopupContextItem())
							{
								// Close
								if (ImGui::MenuItem(("Close##" + file->name).c_str()))
								{
									file->open = false;
									ImGui::CloseCurrentPopup();
								}

								// Reload
								if (ImGui::MenuItem(("Reload##" + file->name).c_str(), "F5"))
								{
									const String * src { nullptr };
									switch (file->type)
									{
									case ShaderFile::Vert: src = &r->shader()->sources().vs; break;
									case ShaderFile::Frag: src = &r->shader()->sources().fs; break;
									case ShaderFile::Geom: src = &r->shader()->sources().gs; break;
									}
									file->text.SetText((*src).trim());
									ImGui::CloseCurrentPopup();
								}

								// Copy
								if (ImGui::MenuItem(("Copy to Clipboard##" + file->name).c_str()))
								{
									ML_Engine.window().setClipboardString(file->text.GetText());
									ImGui::CloseCurrentPopup();
								}

								// Show Whitespace
								bool show_whitespace { file->text.IsShowingWhitespaces() };
								if (ImGui::Checkbox("Show Whitespace", &show_whitespace))
								{
									file->text.SetShowWhitespaces(show_whitespace);
								}

								// Palette
								int32_t editor_style {
									((file->text.GetPalette() == TextEditor::GetDarkPalette())
										? 0
										: ((file->text.GetPalette() == TextEditor::GetLightPalette())
											? 1
											: (((file->text.GetPalette() == TextEditor::GetRetroBluePalette())
												? 2 : 2
												)))) };
								if (ImGui::Combo("##Palette", &editor_style, "Dark\0Light\0Retro"))
								{
									switch (editor_style)
									{
									case 0: file->text.SetPalette(TextEditor::GetDarkPalette()); break;
									case 1: file->text.SetPalette(TextEditor::GetLightPalette()); break;
									case 2: file->text.SetPalette(TextEditor::GetRetroBluePalette()); break;
									}
									ImGui::CloseCurrentPopup();
								}

								ImGui::EndPopup();
							}

							// Tab Contents
							if (tab_open)
							{
								// Text Editor 
								file->text.Render(
									("##ShaderFile##" + file->name + "##TextEditor").c_str(),
									{ 0, 0 },
									true
								);

								if (file->text.IsTextChanged())
								{
									file->dirty = true;
								}

								ImGui::EndTabItem();
							}

							ImGui::PopID();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndChildFrame();

					// Toolbar
					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::BeginChildFrame(
						ImGui::GetID("ShaderFile##Toolbar"),
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
				}
			}

			ImGui::EndChild();
			ImGui::PopID();
		}

		void Noobs::draw_uniforms_tab()
		{
			ImGui::PushID("##Uniforms");
			ImGui::BeginChildFrame(ImGui::GetID("##Uniforms##Content"), { 0, 0 }, 0);

			if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
			{
				if (auto r { e ? e->get<Renderer>() : nullptr })
				{
					if (auto m { r->material() })
					{
						ImGuiExt::HelpMarker(
							"** Note:\n"
							"Some values may be overwritten internally,\n"
							"even if they aren't marked as read-only.\n"
						);
						ImGui::SameLine();

						// New Uniform Popup
						Uniform * to_add { nullptr };
						if (PropertyDrawer<Uniform>()("New Uniform##Noobs", (ptr_t<Uniform> &)to_add))
						{
							// Already Exists
							if (to_add && !m->insert(to_add))
							{
								Debug::logError("A uniform with the name \'{0}\' already exists",
									to_add->getName()
								);
								delete to_add;
							}
						}

						ImGui::SameLine();

						// Copy to Clipboard
						if (ImGui::Button("Copy to Clipboard"))
						{
							SStream ss;
							for (const auto & u : (*m))
							{
								if (!u) continue;
								ss << std::left
									<< "uniform "
									<< std::setw(7) << util::to_string(*u) << " "
									<< std::setw(15) << u->getName() << " "
									<< "{ ";
								switch (u->getID())
								{
								case uni_bool::ID	: if (auto a { detail::as_bool(u) }) ss << (*a); break;
								case uni_float::ID	: if (auto a { detail::as_float(u) }) ss << (*a); break;
								case uni_int::ID	: if (auto a { detail::as_int(u) }) ss << (*a); break;
								case uni_vec2::ID	: if (auto a { detail::as_vec2(u) }) ss << (*a); break;
								case uni_vec3::ID	: if (auto a { detail::as_vec3(u) }) ss << (*a); break;
								case uni_vec4::ID	: if (auto a { detail::as_vec4(u) }) ss << (*a); break;
								case uni_color::ID	: if (auto a { detail::as_color(u) }) ss << (*a); break;
								case uni_mat2::ID	: if (auto a { detail::as_mat2(u) }) ss << (*a); break;
								case uni_mat3::ID	: if (auto a { detail::as_mat3(u) }) ss << (*a); break;
								case uni_mat4::ID	: if (auto a { detail::as_mat4(u) }) ss << (*a); break;
								case uni_sampler::ID: if (auto a { detail::as_sampler(u) }) ss << ML_Engine.content().get_name(*a); break;
								}
								if (ss.str().back() != ' ') ss << ' ';
								ss << "}" << std::endl;
							}
							ML_Engine.window().setClipboardString(ss.str());
						}

						ImGui::Separator();

						/* * * * * * * * * * * * * * * * * * * * */

						static bool setup_uni_columns { false };

						// Uniform Column Headers
						ImGui::Columns(3, "##Uni##Columns");
						if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.1f); }
						if (ImGui::Selectable("Type"))
						{
							auto sort_type_ascending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->getID()) < (rhs->getID()); }); });

							auto sort_type_descending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->getID()) > (rhs->getID()); }); });

							static bool state { 0 };
							if (state = !state) { sort_type_ascending(); }
							else { sort_type_descending(); }
						}
						ImGui::NextColumn();
						if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.15f); }
						if (ImGui::Selectable("Name"))
						{
							auto sort_name_ascending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->getName()) < (rhs->getName()); }); });

							auto sort_name_descending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->getName()) > (rhs->getName()); }); });

							static bool state { 0 };
							if (state = !state) { sort_name_ascending(); }
							else { sort_name_descending(); }
						}
						ImGui::NextColumn();
						if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.75f); }
						if (ImGui::Selectable("Value"))
						{
							auto sort_value_ascending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->isModifiable()) < (rhs->isModifiable()); }); });

							auto sort_value_descending = ([&]() { std::sort(
								m->begin(),
								m->end(),
								[](auto lhs, auto rhs) { return (lhs->isModifiable()) > (rhs->isModifiable()); }); });

							static bool state { 0 };
							if (state = !state) { sort_value_ascending(); }
							else { sort_value_descending(); }
						}
						ImGui::NextColumn();
						ImGui::Separator();
						ImGui::Columns(1);
						if (!setup_uni_columns) { setup_uni_columns = true; }

						/* * * * * * * * * * * * * * * * * * * * */

						// Uniform List
						Uniform * to_remove { nullptr };
						for (auto & u : (*m))
						{
							if (!u) continue;
							ImGui::Columns(3, "##Uni##Columns");
							const String label { "##Uni##" + u->getName() };
							const float_t color_ref{ (float_t)u->getID() / (float_t)Uniform::MAX_UNIFORM_TYPE };

							// Uniform Type
							/* * * * * * * * * * * * * * * * * * * * */
							ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color_ref, 0.9f, 0.9f));
							ImGui::ButtonEx(util::to_string(*u).c_str(), { ImGui::GetContentRegionAvailWidth(), 0 }, ImGuiButtonFlags_Disabled);
							ImGui::PopStyleColor();
							ImGui::NextColumn();

							// Uniform Name
							/* * * * * * * * * * * * * * * * * * * * */
							ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
							if (u->isModifiable())
							{
								static char name[32] = "";
								std::strcpy(name, u->getName().c_str());
								const bool input_submit{ ImGui::InputText(
									(label + "##Name").c_str(),
									name,
									ML_ARRAYSIZE(name),
									ImGuiInputTextFlags_EnterReturnsTrue
								) };
								if (input_submit)
								{
									if (!m->rename(u->getName(), name))
									{
										Debug::logError("A uniform with that name already exists"); 
									}
								}
							}
							else
							{
								ImGui::Text(u->getName().c_str());
							}
							ImGui::PopItemWidth();
							ImGui::NextColumn();

							// Uniform Value
							/* * * * * * * * * * * * * * * * * * * * */
							ImGui::PushID(label.c_str());
							ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.9f);
							const bool canEdit{ PropertyDrawer<Uniform>()(label, (Uniform &)(*u)) };
							ImGui::PopItemWidth();
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.1f);
							if (canEdit)
							{
								if (ImGuiExt::Confirm(
									"Delete Uniform",
									ImGui::Button("Delete"),
									"Are you sure you want to delete uniform {0}?"_s.format(u->getName())
								) == ImGuiExt::Submitted)
								{
									to_remove = u;
								}
								ImGuiExt::Tooltip("Delete this uniform");
							}
							else
							{
								ImGuiExt::HelpMarker("This uniform cannot be modified.");
							}
							ImGui::PopItemWidth();
							ImGui::PopID();
							ImGui::NextColumn();
							ImGui::Columns(1);
							ImGui::Separator();
						}
						if (to_remove) { m->erase(to_remove); }
					}
				}
			}

			ImGui::EndChildFrame();
			ImGui::PopID();
		}

		void Noobs::draw_settings_tab()
		{
			ImGui::PushID("##Settings");
			ImGui::BeginChildFrame(ImGui::GetID("##Settings##Content"), { 0, 0 }, 0);

			if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
			{
				if (auto r { e ? e->get<Renderer>() : nullptr })
				{
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					// Select Material
					const Material * mtl { r ? r->material() : nullptr };
					if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
					{
						r->setMaterial(mtl);
						reset_sources().generate_sources();
					}
					ImGuiExt::Tooltip("Materials contain the list uniforms applied to the shader.");
					ImGui::Separator();

					// Select Shader
					const Shader * shd { r ? r->shader() : nullptr };
					if (PropertyDrawer<Shader>()("Shader##Renderer##Noobs", shd))
					{
						r->setShader(shd);
						reset_sources().generate_sources();
					}
					ImGuiExt::Tooltip("Shaders provide the code for the programmable stages of the rendering pipeline.");
					ImGui::Separator();

					// Select Model
					const Model * mdl { r ? r->model() : nullptr };
					if (PropertyDrawer<Model>()("Model##Renderer##Noobs", mdl))
					{
						r->setModel(mdl);
					}
					ImGuiExt::Tooltip("Models specify the geometry to be rendered.");
					ImGui::Separator();

					// Camera Settings
					if (auto c { Camera::mainCamera() })
					{
						// Get Video Modes
						static const List<VideoMode> & mode_values { Window::getFullscreenModes() };

						// Get Video Names
						static const List<String> & mode_names = [&]
						{
							static List<String> temp { "Automatic", "Manual" };
							for (const auto & elem : mode_values)
								temp.push_back(util::to_string(elem.size));
							return temp;
						}();

						// Viewport
						if (ImGuiExt::Combo("Resolution", &m_displayIndex, mode_names))
						{
							if (m_displayIndex == 0)
							{
								m_displayMode = DisplayMode::Automatic;
							}
							else if (m_displayIndex == 1)
							{
								m_displayMode = DisplayMode::Manual;
							}
							else
							{
								m_displayMode = DisplayMode::Fixed;
							}
						}
						if (m_displayMode == DisplayMode::Manual)
						{
							ImGui::Indent();
							vec2 v { (vec2)c->viewport().size() };
							if (ImGui::DragFloat2("##Resolution", &v[0]))
							{
								if (v[0] <= 0.f) v[0] = FLT_MIN;
								if (v[1] <= 0.f) v[1] = FLT_MIN;
								c->setViewport((vec2i)v);
							}
							ImGui::Unindent();
						}
						if (m_displayMode == DisplayMode::Fixed)
						{
							c->setViewport((vec2i)mode_values[m_displayIndex - 2].size);
						}
						ImGuiExt::Tooltip("Specify the display resolution.");

						ImGui::Separator();

						// Background Color
						vec4 background { c->background() };
						if (ImGui::ColorEdit4("Background", &background[0]))
						{
							c->setBackground(background);
						}
						ImGuiExt::Tooltip("Specify the display background color.");

						ImGui::Separator();
					}
					else
					{
						ImGui::Text("No Camera Found");
					}

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::BeginChildFrame(ImGui::GetID("NoobsRenderStates"), { 0, 0 }, 0);

					// Alpha State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](AlphaState * alpha) 
					{	if (!alpha) { return; }
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID(ML_ADDRESSOF(alpha));
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
						ImGui::SameLine(); ImGui::Text(");");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glAlphaFunc"))
						{
							Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml");
						}
						ImGuiExt::Tooltip("Specify the alpha test function");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t func = GL::index_of(alpha->func);
						if (ImGuiExt::Combo("##Func", &func, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							alpha->func = GL::value_at<GL::Predicate>(func);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'func\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(alpha->func),
							util::to_hex<uint32_t>(alpha->func),
							GL::desc_of(alpha->func)
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::DragFloat("##Ref", &alpha->coeff);
						ImGuiExt::Tooltip(String(
							"Param: \'ref\'\n\n"
							"Value: {0}\n\n"
							"Brief: {1}\n"
						).format(
							alpha->coeff,
							"Specifies the reference value that incoming alpha values are compared to"
						));
						ImGui::SameLine(); ImGui::Text(");");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::PopID();

						/* * * * * * * * * * * * * * * * * * * * */
					})(&r->states().alpha());

					// Blend State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](BlendState * blend) 
					{	if (!blend) { return; }
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID(ML_ADDRESSOF(blend));
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
						ImGui::SameLine(); ImGui::Text(");");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glBlendFuncSeparate"))
						{
							Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glBlendFuncSeparate.xml");
						}
						ImGuiExt::Tooltip(
							"Specify pixel arithmetic for RGB and alpha components separately."
						);
						ImGui::SameLine(); ImGui::Text("(");
						ImGui::Indent();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t sfactorRGB = GL::index_of(blend->sfactorRGB);
						if (ImGuiExt::Combo("##sfactorRGB", &sfactorRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->sfactorRGB = GL::value_at<GL::Factor>(sfactorRGB);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'sfactorRGB\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->sfactorRGB),
							util::to_hex<uint32_t>(blend->sfactorRGB),
							"Specifies how the red, green, and blue blending factors are computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t dfactorRGB = GL::index_of(blend->dfactorRGB);
						if (ImGuiExt::Combo("##dfactorRGB", &dfactorRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dfactorRGB = GL::value_at<GL::Factor>(dfactorRGB);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'dfactorRGB\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->dfactorRGB),
							util::to_hex<uint32_t>(blend->dfactorRGB),
							"Specifies how the red, green, and blue destination blending factors are computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t sfactorAlpha = GL::index_of(blend->sfactorAlpha);
						if (ImGuiExt::Combo("##sfactorAlpha", &sfactorAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->sfactorAlpha = GL::value_at<GL::Factor>(sfactorAlpha);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'sfactorAlpha\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->sfactorAlpha),
							util::to_hex<uint32_t>(blend->sfactorAlpha),
							"Specifies how the alpha source blending factor is computed"
						));
						ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t dfactorAlpha = GL::index_of(blend->dfactorAlpha);
						if (ImGuiExt::Combo("##dfactorAlpha", &dfactorAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
						{
							blend->dfactorAlpha = GL::value_at<GL::Factor>(dfactorAlpha);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'dfactorAlpha\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(blend->dfactorAlpha),
							util::to_hex<uint32_t>(blend->dfactorAlpha),
							"Specifies how the alpha destination blending factor is computed"
						));
						ImGui::Unindent();
						ImGui::Text(");");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::PopID();

						/* * * * * * * * * * * * * * * * * * * * */
					})(&r->states().blend());

					// Cull State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](CullState * cull)
					{	if (!cull) { return; }
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID(ML_ADDRESSOF(cull));
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
						ImGui::SameLine(); ImGui::Text(");");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glCullFace"))
						{
							Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml");
						}
						ImGuiExt::Tooltip("Specify which facets are culled (as specified by mode) when facet culling is enabled.");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t mode = GL::index_of(cull->mode);
						if (ImGuiExt::Combo("##Mode", &mode, GL::Face_raw_names, ML_ARRAYSIZE(GL::Face_raw_names)))
						{
							cull->mode = GL::value_at<GL::Face>(mode);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'mode\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(cull->mode),
							util::to_hex<uint32_t>(cull->mode),
							GL::desc_of(cull->mode)
						));
						ImGui::SameLine(); ImGui::Text(");");

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::PopID();

						/* * * * * * * * * * * * * * * * * * * * */
					})(&r->states().cull());

					// Depth State
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					([](DepthState * depth)
					{	if (!depth) { return; }
						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::PushID(ML_ADDRESSOF(depth));
						ImGui::PushID("##DepthState");
						ImGui::NewLine();
						ImGui::PushItemWidth(150);

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox(depth->enabled
							? "glEnable" : "glDisable",
							&depth->enabled
						);
						ImGuiExt::Tooltip(GL::desc_of(GL::DepthTest));
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::Text(GL::raw_name_of(GL::DepthTest));
						ImGui::SameLine(); ImGui::Text(");");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::Checkbox("glDepthMask", &depth->mask); 
						ImGuiExt::Tooltip("Specifies whether the depth buffer is enabled for writing.");
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
						ImGui::Text(depth->mask ? "true" : "false");
						ImGui::SameLine(); ImGui::Text(");");
						ImGui::NewLine();

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::Button("glDepthFunc"))
						{
							Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml");
						}
						ImGuiExt::Tooltip(
							"Specify the function used to compare each incoming pixel depth value with the depth value present in the depth buffer."
						);
						ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();

						/* * * * * * * * * * * * * * * * * * * * */

						int32_t func = GL::index_of(depth->func);
						if (ImGuiExt::Combo("##Func", &func, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
						{
							depth->func = GL::value_at<GL::Predicate>(func);
						}
						ImGuiExt::Tooltip(String(
							"Param: \'func\'\n\n"
							"Value: {0} (0x{1})\n\n"
							"Brief: {2}\n"
						).format(
							GL::raw_name_of(depth->func),
							util::to_hex<uint32_t>(depth->func),
							GL::desc_of(depth->func)
						));
						ImGui::SameLine(); ImGui::Text(");");

						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::PopID();

						/* * * * * * * * * * * * * * * * * * * * */
					})(&r->states().depth());

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

					ImGui::EndChildFrame();

					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				}
			}
			
			ImGui::EndChildFrame();
			ImGui::PopID(); 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void Noobs::draw_plugin_menu()
		{
			// Show Editor
			ImGui::Checkbox("Show Editor", &m_editor_open);
			ImGuiExt::Tooltip("Toggle editor visibility");

			// Show Display
			ImGui::Checkbox("Show Display", &m_display_open);
			ImGuiExt::Tooltip("Toggle display visibility");

			// Use Main Camera
			ImGui::Checkbox("Apply Camera Uniforms", &m_use_main_camera);
			ImGuiExt::Tooltip("If enabled, \'u_camera\' will be automatically updated");

			ImGui::Separator();

			// Camera
			if (ImGui::BeginMenu("Camera"))
			{
				if (auto c { Camera::mainCamera() })
				{
					constexpr float_t speed { 0.005f };

					bool	enabled = c->enabled();
					int32_t clearFlags = c->clearFlags();
					int32_t projection = c->projection();
					vec3	position = c->position();
					vec3	forward = c->direction();
					vec4	background = c->background();
					float_t fieldOfView = c->fieldOfView();
					float_t clipNear = c->clipNear();
					float_t clipFar = c->clipFar();
					vec2	viewport = (vec2)c->viewport().size();

					/* * * * * * * * * * * * * * * * * * * * */

					// Enabled
					if (ImGui::Checkbox(("Enabled##Camera##Noobs"), &enabled))
					{
						c->setEnabled(enabled);
					}
					ImGuiExt::Tooltip("If enabled, the camera be applied.");

					// Clear Flags
					if (ImGuiExt::Combo(
						("Clear Flags##Camera##Noobs"),
						&clearFlags,
						"Solid Color\0Depth Only\0Don't Clear"
					))
					{
						c->setClearFlags((Camera::ClearFlags)clearFlags);
					}
					ImGuiExt::Tooltip("Specify how the screen should be cleared.");

					// Projection
					if (ImGuiExt::Combo(
						("Projection##Camera##Noobs"),
						&projection,
						"Perspective\0"
					))
					{
						c->setProjection((Camera::Projection)projection);
					}
					ImGuiExt::Tooltip("Specify which projection to use.");

					// Background
					if (ImGui::ColorEdit4(("Background##Camera##Noobs"), &background[0]))
					{
						c->setBackground(background);
					}
					ImGuiExt::Tooltip("Specify the color to apply when using \'Solid Color\'.");

					ImGui::Separator();

					// Position
					if (ImGui::DragFloat3(("Position##Camera##Noobs"), &position[0], speed))
					{
						c->setPosition(position);
					}
					ImGuiExt::Tooltip("Set the position of the camera.");

					// Direction
					if (ImGui::DragFloat3(("Direction##Camera##Noobs"), &forward[0], speed))
					{
						c->setDirection(forward);
					}
					ImGuiExt::Tooltip("Set the direction the camera is facing.");

					ImGui::Separator();

					// Field of View
					if (ImGui::DragFloat(("Field of View##Camera##Noobs"), &fieldOfView, speed))
					{
						c->setFieldOfView(fieldOfView);
					}
					ImGuiExt::Tooltip("Specify the field of view.");

					// Clip Near
					if (ImGui::DragFloat(("Clip Near##Camera##Noobs"), &clipNear, speed))
					{
						c->setClipNear(clipNear);
					}
					ImGuiExt::Tooltip("Specify the near clipping plane.");

					// Clip Far
					if (ImGui::DragFloat(("Clip Far##Camera##Noobs"), &clipFar, speed))
					{
						c->setClipFar(clipFar);
					}
					ImGuiExt::Tooltip("Specify the far clipping plane.");

					// Viewport
					if (ImGui::DragFloat2(("Viewport##Camera##Noobs"), &viewport[0], speed))
					{
						if (viewport[0] <= 0.f) viewport[0] = FLT_MIN;
						if (viewport[1] <= 0.f) viewport[1] = FLT_MIN;
						c->setViewport((vec2i)viewport);
					}
					ImGuiExt::Tooltip("Specify the viewport size.");
				}
				else
				{
					ImGui::Text("Main Camera Not Found");
				}
				ImGui::EndMenu();
			}
		
			ImGui::Separator();

			// Entity
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Entity"))
			{
				ImGui::PushID(ML_ADDRESSOF(m_entity));

				// Entity
				const_ptr_t<Entity> e { m_entity };
				if (PropertyDrawer<Entity>()("##TargetEntity", e) && e)
				{
					m_entity = std::remove_cv_t<ptr_t<Entity>>(e);
					reset_sources().generate_sources();
				}
				ImGuiExt::Tooltip("Select the target entity");

				// Renderer
				if (auto r { m_entity ? m_entity->get<Renderer>() : nullptr })
				{
					if (ImGui::CollapsingHeader("Renderer"))
					{
						PropertyDrawer<Renderer>()("Renderer", *r);
					}
				}
				else if (m_entity)
				{
					if (ImGui::Button("Add Renderer"))
					{
						m_entity->add<Renderer>();
						reset_sources().generate_sources();
					}
					ImGuiExt::Tooltip("Attach a Renderer to the target Entity");
				}

				// Transform
				if (auto t { m_entity ? m_entity->get<Transform>() : nullptr })
				{
					if (ImGui::CollapsingHeader("Transform"))
					{
						PropertyDrawer<Transform>()("Transform", *t);
					}
				}
				else if (m_entity)
				{
					if (ImGui::Button("Add Transform")) { m_entity->add<Transform>(); }
					ImGuiExt::Tooltip("Attach a Transform to the target Entity");
				}
				ImGui::PopID();
				ImGui::EndMenu();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Noobs & Noobs::compile_sources()
		{
			if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
			{
				if (auto r{ e ? e->get<Renderer>() : nullptr })
				{
					if (auto s{ r->shader() })
					{
						for (auto & f : m_files)
						{
							f->dirty = false;
							f->errs.clear();
							f->text.SetErrorMarkers({});
						}

						if (!s->loadFromMemory(
							m_files[ShaderFile::Vert]->open ? m_files[ShaderFile::Vert]->text.GetText() : "",
							m_files[ShaderFile::Geom]->open ? m_files[ShaderFile::Geom]->text.GetText() : "",
							m_files[ShaderFile::Frag]->open ? m_files[ShaderFile::Frag]->text.GetText() : ""
						))
						{
							/* error */
						}
					}
				}
			}
			return (*this);
		}

		Noobs & Noobs::dispose_files()
		{
			for (auto & file : m_files)
			{
				if (file) { delete file; }
			}
			return (*this);
		}

		Noobs & Noobs::generate_sources()
		{
			if (auto e{ m_entity ? m_entity : (m_entity = ML_Engine.content().get<Entity>(m_ent_name)) })
			{
				if (auto r{ e ? e->get<Renderer>() : nullptr })
				{
					if (auto s{ r->shader() })
					{
						auto setup_file = ([&](ShaderFile::FileType type, const String & src) {
							if (!m_files[type]) { (m_files[type] = new ShaderFile{ type }); }
							m_files[type]->text.SetText(src.trim());
							m_files[type]->open = !src.empty();
							return m_files[type];
						});

						auto vert{ setup_file(ShaderFile::Vert, s->sources().vs) };
						auto frag{ setup_file(ShaderFile::Frag, s->sources().fs) };
						auto geom{ setup_file(ShaderFile::Geom, s->sources().gs) };
					}
				}
			}
			return (*this);
		}

		Noobs & Noobs::reset_sources()
		{
			for (auto & f : m_files)
			{
				if (!f) continue;
				f->open = false;
				f->text.SetText({});
				f->text.SetErrorMarkers({});
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

extern "C"
{
	ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main()
	{
		return new ml::Noobs{};
	}
}