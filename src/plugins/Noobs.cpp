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
#include <ML/Graphics/RenderTexture.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/ScopedBinder.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ImGuiColorTextEdit/TextEditor.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API Noobs final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ShaderError final : public Trackable
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
				if (const size_t a{ str.find_first_of('(') }; a != String::npos)
				{
					if (const size_t b{ str.find_first_of(')', a + 1) }; b != String::npos)
					{
						if (const size_t c{ str.find_first_of(':', b) }; c != String::npos)
						{
							if (const size_t d{ str.find_first_of(':', c + 1) }; d != String::npos)
							{
								this->line = util::to_i32(str.substr(a + 1, b - a - 1));
								this->code = str.substr(c + 2, d - c - 2);
								this->desc = str.substr(d + 2);
							}
						}
					}
				}
			}

			ShaderError(String const & file, int32_t line, String const & code, String const & expr)
				: file{ file }, line{ line }, code{ code }, desc{ expr }
			{
			}

			ShaderError(const ShaderError & copy)
				: ShaderError{ copy.file, copy.line, copy.code, copy.desc }
			{
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using TextEditor = ImGui::TextEditor;

		struct ShaderFile final : public Trackable, public NonCopyable
		{
			static constexpr C_String Names[] = { "Fragment", "Vertex", "Geometry" };

			using Errors = typename std::vector<ShaderError>;

			size_t		type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;
			Errors		errs;

			ShaderFile(size_t type)
				: type{ type }, text{}, name{ Names[type] }, open{ false }, dirty{ false }, errs{}
			{
# if (!ML_DEBUG)
				this->text.SetPalette(TextEditor::GetLightPalette());
# endif
				this->text.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pipeline = typename std::vector<RenderTexture *>;

		enum FileType : size_t { Frag, Vert, Geom, MAX_DEMO_FILE };

		using FileArray = typename Array<ShaderFile, MAX_DEMO_FILE>;

		enum DisplayMode : int32_t { Automatic, Manual, Fixed };

		bool m_display_open	{ true };
		bool m_code_open	{ true };
		bool m_uniforms_open{ true };
		bool m_settings_open{ true };
		bool m_apply_camera	{ true };

		Pipeline	m_pipeline;
		String		m_target_name;
		FileArray	m_files;
		DisplayMode	m_displayMode;
		int32_t		m_displayIndex;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Noobs() : Plugin {}
			, m_pipeline{}
			, m_target_name{}
			, m_files{ ShaderFile{ Frag }, ShaderFile{ Vert }, ShaderFile{ Geom } }
			, m_displayMode{}
			, m_displayIndex{}
		{
			ML_EventSystem.addListener<LoadEvent>(this);
			ML_EventSystem.addListener<StartEvent>(this);
			ML_EventSystem.addListener<UpdateEvent>(this);
			ML_EventSystem.addListener<DrawEvent>(this);
			ML_EventSystem.addListener<GuiEvent>(this);
			ML_EventSystem.addListener<KeyEvent>(this);
			ML_EventSystem.addListener<DockspaceEvent>(this);
			ML_EventSystem.addListener<ShaderErrorEvent>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override
		{
			switch (*value)
			{
			case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ML_Editor.mainMenuBar()
					.addMenu("View", [&]()
					{
						ImGui::PushID(ML_ADDRESSOF(this));
						ImGui::Separator();
						ImGui::MenuItem("Code", "", &m_code_open);
						ImGui::MenuItem("Uniforms", "", &m_uniforms_open);
						ImGui::MenuItem("Settings", "", &m_settings_open);
						ImGui::MenuItem("Display", "", &m_display_open);
						ImGui::PopID();
					})
					.addMenu("Plugins", [&]()
					{
						ImGui::PushID(ML_ADDRESSOF(this));
						if (ImGui::BeginMenu("Noobs"))
						{
							ImGui::Checkbox("Show Display", &m_display_open);

							ImGui::Checkbox("Show Code", &m_code_open);

							ImGui::Checkbox("Show Uniforms", &m_uniforms_open);

							ImGui::Checkbox("Show Settings", &m_settings_open);

							ImGui::Checkbox("Update Camera", &m_apply_camera);
							ImGuiExt::Tooltip("If enabled, \'u_camera\' will be automatically updated");

							Entity const * e{ ML_Engine.content().get<Entity>(m_target_name) };
							if (PropertyDrawer<Entity>()("Target Entity", e) && e)
							{
								m_target_name = ML_Engine.content().get_name(e);
								generate_sources();
							}
							ImGuiExt::Tooltip("Select the target entity");
							ImGui::EndMenu();
						}
						ImGui::PopID();
					});

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case StartEvent::ID: if (auto ev{ value.as<StartEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				m_pipeline.push_back(ML_Engine.content().get<RenderTexture>("render/main"));

				m_pipeline.push_back(ML_Engine.content().get<RenderTexture>("render/post"));
					
				m_target_name = ML_Engine.prefs().get_string("Noobs", "target_entity", "");

				generate_sources();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (auto c{ Camera::mainCamera() }; c && c->enabled())
				{
					// Update Pipeline
					for (RenderTexture *& rt : m_pipeline)
					{
						rt->update((vec2)c->viewport().size());
					}

					// Update Camera Uniforms
					if (m_apply_camera)
					{
						for (auto & [ key, value ] : ML_Engine.content().data<Material>())
						{
							if (auto m{ (Material *)value })
							{
								// FIXME: slow
								m->set<uni_vec3>("u_camera.pos", c->position());
								m->set<uni_vec3>("u_camera.dir", c->direction());
								m->set<uni_float>("u_camera.fov", c->fieldOfView());
								m->set<uni_float>("u_camera.near", c->clipNear());
								m->set<uni_float>("u_camera.far", c->clipFar());
								m->set<uni_vec2>("u_camera.view", (vec2)c->viewport().size());
							}
						}
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case DrawEvent::ID: if (auto ev{ value.as<DrawEvent>()})
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Render Main Scene
				if (ML_BIND_SCOPE(RenderTexture, m_pipeline.at(0)))
				{
					// Apply Camera
					if (auto c{ Camera::mainCamera() }; c && c->enabled())
					{
						c->applyClear().applyViewport();
					}

					// Draw Renderers
					for (auto & [ key, value ] : ML_Engine.content().data<Entity>())
					{
						if (auto e{ (Entity *)value })
						{
							if (auto r{ e->get<Renderer>() }; r && r->enabled())
							{
								if (auto t{ e->get<Transform>() }; t && t->enabled())
								{
									if (auto m{ (Material *)r->material() })
									{
										// FIXME: slow
										m->set<uni_vec3>("u_position", t->position());
										m->set<uni_vec4>("u_rotation", t->rotation());
										m->set<uni_vec3>("u_scale", t->scale());
									}
								}
								ML_Engine.window().draw(r);
							}
						}
					}
				}
				
				// Render Post Processing
				if (ML_BIND_SCOPE(RenderTexture, m_pipeline.at(1)))
				{
					// Apply Camera
					if (auto c{ Camera::mainCamera() }; c && c->enabled())
					{
						c->applyClear().applyViewport();
					}

					// Apply States
					RenderStates{
						AlphaState	{ true, GL::Greater, 0.01f },
						BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
						CullState	{ false },
						DepthState	{ false },
					}();

					// Draw Previous
					ML_Engine.window().draw(m_pipeline.at(0));
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case GuiEvent::ID: if (auto ev{ value.as<GuiEvent>()})
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::PushID(ML_ADDRESSOF(this));
				
				// Draw Display
				if (m_display_open)
				{
					ImGui::PushID("Display##Noobs");
					if (ImGui::Begin("Display##Noobs", &m_display_open, ImGuiWindowFlags_NoScrollbar))
					{
						// Draw the last texture in the pipeline
						ML_Editor.previews().drawPreview<RenderTexture>(m_pipeline.back(), ImGuiExt::GetContentRegionAvail(), [&]
						{
							if (auto c{ Camera::mainCamera() }; c && (m_displayMode == Automatic))
							{
								c->setViewport((vec2i)ImGuiExt::GetContentRegionAvail());
							}
						});
					}
					ImGui::End();
					ImGui::PopID();
				}

				if (auto e{ ML_Engine.content().get<Entity>(m_target_name) })
				{
					// Draw Code
					if (m_code_open)
					{
						ImGui::PushID("Code##Noobs");
						if (ImGui::Begin("Code##Noobs", &m_code_open, 0)) { draw_code(e); }
						ImGui::End();
						ImGui::PopID();
					}

					// Draw Uniforms
					if (m_uniforms_open)
					{
						ImGui::PushID("Uniforms##Noobs");
						if (ImGui::Begin("Uniforms##Noobs", &m_uniforms_open, 0)) { draw_uniforms(e); }
						ImGui::End();
						ImGui::PopID();
					}

					// Draw Settings
					if (m_settings_open)
					{
						ImGui::PushID("Settings##Noobs");
						if (ImGui::Begin("Settings##Noobs", &m_settings_open, 0)) { draw_settings(e); }
						ImGui::End();
						ImGui::PopID();
					}
				}

				ImGui::PopID();

				// First Time Setup
				static bool once{ false };
				if (!once && (once = true))
				{
					ImGui::SetWindowFocus("Code##Noobs");
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case ShaderErrorEvent::ID: if (auto ev{ value.as<ShaderErrorEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (auto e{ ML_Engine.content().get<Entity>(m_target_name) })
				{
					if (auto r{ e ? e->get<Renderer>() : nullptr })
					{
						if (ev.shader && (ev.shader == r->shader()))
						{
							std::cout << ev.error;

							// Decode Errors
							SStream ss{ String{ ev.error } };
							String line;
							while (std::getline(ss, line, '\n'))
							{
								const ShaderError err{ ev.type, line };
								switch (ev.type)
								{
								case GL::VertexShader: m_files[Vert].errs.push_back(err); break;
								case GL::FragmentShader: m_files[Frag].errs.push_back(err); break;
								case GL::GeometryShader: m_files[Geom].errs.push_back(err); break;
								}
							}

							// Set Error Markers
							for (auto & file : m_files)
							{
								TextEditor::ErrorMarkers markers;
								for (auto & err : file.errs)
								{
									markers.insert({ err.line, err.code + ": " + err.desc });
								}
								file.text.SetErrorMarkers(markers);
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
				if (ev.getPress(KeyCode::F11))
				{
					ML_EventSystem.fireEvent<WindowFullscreenEvent>(-1);
				}

				// Refresh Sources
				if (ev.getPress(KeyCode::F5))
				{
					generate_sources();
				}

				// Compile Sources
				if (ev.isSave())
				{
					compile_sources();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (Editor_Dockspace & d{ ev.dockspace }; d.isOpen())
				{
					d.dockWindow("Display##Noobs", d.getNode(d.LeftUp));
					d.dockWindow("Code##Noobs", d.getNode(d.RightUp));
					d.dockWindow("Uniforms##Noobs", d.getNode(d.RightUp));
					d.dockWindow("Settings##Noobs", d.getNode(d.RightUp));
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw_code(Entity * e)
		{
			ImGui::PushID("##CodeTab");
			ImGui::BeginChild("##CodeTab##Content", { 0, 0 }, false, ImGuiWindowFlags_NoScrollbar);
		
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
						if (!file.open) continue;

						ImGui::PushID(ML_ADDRESSOF(&file));

						// Text Editor Tab
						const bool tab_open{ ImGui::BeginTabItem(
							file.name.c_str(), 
							nullptr, 
							file.dirty ? ImGuiTabItemFlags_UnsavedDocument : 0)
						};

						// Tab Context Item
						if (ImGui::BeginPopupContextItem())
						{
							// Close
							if (ImGui::MenuItem(("Close##" + file.name).c_str()))
							{
								file.open = false;
								ImGui::CloseCurrentPopup();
							}

							// Reload
							if (ImGui::MenuItem(("Reload##" + file.name).c_str(), "F5"))
							{
								const String * src { nullptr };
								switch (file.type)
								{
								case Vert: src = &r->shader()->sources().vs; break;
								case Frag: src = &r->shader()->sources().fs; break;
								case Geom: src = &r->shader()->sources().gs; break;
								}
								file.text.SetText((*src).trim());
								ImGui::CloseCurrentPopup();
							}

							// Copy
							if (ImGui::MenuItem(("Copy All##" + file.name).c_str()))
							{
								ML_Engine.window().setClipboardString(file.text.GetText());
								ImGui::CloseCurrentPopup();
							}

							// Show Whitespace
							bool show_whitespace { file.text.IsShowingWhitespaces() };
							if (ImGui::Checkbox("Show Whitespace", &show_whitespace))
							{
								file.text.SetShowWhitespaces(show_whitespace);
							}

							// Palette
							int32_t editor_style {
								((file.text.GetPalette() == TextEditor::GetDarkPalette())
									? 0
									: ((file.text.GetPalette() == TextEditor::GetLightPalette())
										? 1
										: (((file.text.GetPalette() == TextEditor::GetRetroBluePalette())
											? 2 : 2
											)))) };
							if (ImGui::Combo("##Palette", &editor_style, "Dark\0Light\0Retro"))
							{
								switch (editor_style)
								{
								case 0: file.text.SetPalette(TextEditor::GetDarkPalette()); break;
								case 1: file.text.SetPalette(TextEditor::GetLightPalette()); break;
								case 2: file.text.SetPalette(TextEditor::GetRetroBluePalette()); break;
								}
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}

						// Tab Contents
						if (tab_open)
						{
							// Text Editor 
							file.text.Render(
								("##ShaderFile##" + file.name + "##TextEditor").c_str(),
								{ 0, 0 },
								true
							);
							if (ImGui::BeginPopupContextItem("TextEditorContextMenu"))
							{
								if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr))
								{
									file.text.Cut();
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr))
								{
									file.text.Copy();
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr))
								{
									file.text.Paste();
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
							if (file.text.IsTextChanged())
							{
								file.dirty = true;
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
						if (i > 0) { ImGui::SameLine(); }
						ImGui::Checkbox(m_files[i].name.c_str(), &m_files[i].open);
					}
				}
				ImGui::EndChildFrame();
			}

			ImGui::EndChild();
			ImGui::PopID();
		}

		void draw_uniforms(Entity * e)
		{
			ImGui::PushID("##UniformsTab");
			ImGui::BeginChildFrame(ImGui::GetID("##UniformsTab##Content"), { 0, 0 }, 0);

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
					if (PropertyDrawer<Uniform>()("New Uniform##Noobs", (Uniform * &)to_add))
					{
						// Already Exists
						if (to_add && !m->insert(to_add))
						{
							Debug::logError("A uniform with the name \'{0}\' already exists",
								to_add->name()
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
								<< std::setw(15) << u->name() << " "
								<< "{ ";
							switch (u->get_root_id())
							{
							case typeof<bool>::hash		: if (auto * v{ uniform_cast<bool>(u) }) ss << (*v); break;
							case typeof<float_t>::hash	: if (auto * v{ uniform_cast<float_t>(u) }) ss << (*v); break;
							case typeof<int32_t>::hash	: if (auto * v{ uniform_cast<int32_t>(u) }) ss << (*v); break;
							case typeof<vec2>::hash		: if (auto * v{ uniform_cast<vec2>(u) }) ss << (*v); break;
							case typeof<vec3>::hash		: if (auto * v{ uniform_cast<vec3>(u) }) ss << (*v); break;
							case typeof<vec4>::hash		: if (auto * v{ uniform_cast<vec4>(u) }) ss << (*v); break;
							case typeof<Color>::hash	: if (auto * v{ uniform_cast<Color>(u) }) ss << (*v); break;
							case typeof<mat2>::hash		: if (auto * v{ uniform_cast<mat2>(u) }) ss << (*v); break;
							case typeof<mat3>::hash		: if (auto * v{ uniform_cast<mat3>(u) }) ss << (*v); break;
							case typeof<mat4>::hash		: if (auto * v{ uniform_cast<mat4>(u) }) ss << (*v); break;
							case typeof<Texture>::hash	: if (auto * v{ uniform_cast<Texture const *>(u) }) ss << ML_Engine.content().get_name(*v); break;
							}
							if (ss.str().back() != ' ') ss << ' ';
							ss << "}" << std::endl;
						}
						ML_Engine.window().setClipboardString(ss.str());
					}

					ImGui::Separator();

					/* * * * * * * * * * * * * * * * * * * * */

					// Uniform Column Headers
					static bool setup_uni_columns { false };
					ImGui::Columns(4, "##Uni##Columns");
					if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.1f); } // 10
					if (ImGui::Selectable("Type"))
					{
						static bool state{ 0 };
						if (state = !state)
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->category() < rhs->category(); }
							);
						}
						else
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->category() > rhs->category(); }
							);
						}
					}
					ImGui::NextColumn();
					if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.15f); } // 25
					if (ImGui::Selectable("Name"))
					{
						static bool state{ 0 };
						if (state = !state)
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->name() < rhs->name(); }
							);
						}
						else
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->name() > rhs->name(); }
							);
						}
					}
					ImGui::NextColumn();
					if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.65f); } // 90
					if (ImGui::Selectable("Value"))
					{
						static bool state { 0 };
						if (state = !state) 
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->is_modifiable() < rhs->is_modifiable(); }
							);
						}
						else
						{
							std::sort(m->begin(), m->end(),
								[](auto lhs, auto rhs) { return lhs->is_modifiable() > rhs->is_modifiable(); }
							);
						}
					}
					ImGui::NextColumn();
					if (!setup_uni_columns) { ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.1f); } // 100
					ImGuiExt::HelpMarker("Available Actions");
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
						ImGui::Columns(4, "##Uni##Columns");
						const String label { "##Uni##" + u->name() };
						const float_t color_ref{ (float_t)(u->category()) / (float_t)Uniform::Type::ID_MAX };
						const bool can_edit{ u->is_modifiable() };

						// Uniform Type
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color_ref, 0.9f, 0.9f));
						ImGui::ButtonEx(util::to_string(*u).c_str(), { ImGui::GetContentRegionAvailWidth(), 0 }, ImGuiButtonFlags_Disabled);
						ImGui::PopStyleColor();
						ImGui::NextColumn();

						// Uniform Name
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
						if (can_edit)
						{
							static char name[32] = "";
							std::strcpy(name, u->name().c_str());
							if (ImGui::InputText(
								(label + "##Name").c_str(),
								name,
								ML_ARRAYSIZE(name),
								ImGuiInputTextFlags_EnterReturnsTrue
							) && !m->rename(u->name(), name))
							{
								Debug::logError("A uniform with that name already exists");
							}
						}
						else
						{
							ImGui::Text(u->name().c_str());
						}
						ImGui::PopItemWidth();
						ImGui::NextColumn();

						// Uniform Value
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushID(label.c_str());
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
						PropertyDrawer<Uniform>()(label, (Uniform &)(*u));
						ImGui::PopItemWidth();
						ImGui::NextColumn();

						// Uniform Flags
						/* * * * * * * * * * * * * * * * * * * * */
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
						if (can_edit)
						{
							if (ImGuiExt::Confirm(
								ImGui::Button("Delete"),
								"Delete Uniform",
								nullptr,
								"Are you sure you want to delete uniform {0}?"_s.format(u->name()).c_str(),
								"Yes",
								"No",
								ImGuiWindowFlags_AlwaysAutoResize
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
						ImGui::NextColumn();

						ImGui::Columns(1);
						ImGui::PopID();

						ImGui::Separator();
					}
					if (to_remove) { m->erase(to_remove); }
				}
			}

			ImGui::EndChildFrame();
			ImGui::PopID();
		}

		void draw_settings(Entity * e)
		{
			ImGui::PushID("##SettingsTab");
			ImGui::BeginChildFrame(ImGui::GetID("##SettingsTab##Content"), { 0, 0 }, 0);

			if (auto r { e ? e->get<Renderer>() : nullptr })
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				// Select Shader
				Shader const * shd{ r ? r->shader() : nullptr };
				if (PropertyDrawer<Shader>()("Shader##Renderer##Noobs", shd))
				{
					r->setShader(shd);
					generate_sources();
				}
				ImGuiExt::Tooltip("Shaders provide the code for the programmable stages of the rendering pipeline.");
				ImGui::Separator();

				// Select Material
				Material const * mtl { r ? r->material() : nullptr };
				if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
				{
					r->setMaterial(mtl);
					generate_sources();
				}
				ImGuiExt::Tooltip("Materials contain the list uniforms applied to the shader.");
				ImGui::Separator();

				// Select Model
				Model const * mdl { r ? r->model() : nullptr };
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
					static std::vector<VideoMode> const & mode_values { Window::getFullscreenModes() };

					// Get Video Names
					static std::vector<String> const & mode_names = [&]
					{
						static std::vector<String> temp { "Automatic", "Manual" };
						for (const auto & elem : mode_values)
							temp.push_back(util::to_string(elem.size));
						return temp;
					}();

					// Viewport
					if (ImGuiExt::Combo("Resolution", &m_displayIndex, mode_names))
					{
						switch (m_displayIndex)
						{
						case 0: m_displayMode = Automatic; break;
						case 1: m_displayMode = Manual; break;
						default: m_displayMode = Fixed; break;
						}
					}
					if (m_displayMode == Manual)
					{
						ImGui::Indent();
						vec2 v { (vec2)c->viewport().size() };
						if (ImGui::DragFloat2("##Resolution", &v[0]))
						{
							if (v[0] <= 0.f) v[0] = limits<float_t>::min;
							if (v[1] <= 0.f) v[1] = limits<float_t>::min;
							c->setViewport((vec2i)v);
						}
						ImGui::Unindent();
					}
					else if (m_displayMode == Fixed)
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
				([](AlphaState * value) 
				{	if (!value) { return; }
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID(ML_ADDRESSOF(value));
					ImGui::PushID("##AlphaState");
					ImGui::NewLine();
					ImGui::PushItemWidth(200);

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::Checkbox(value->enabled
						? "glEnable" : "glDisable",
						&value->enabled
					);
					ImGuiExt::Tooltip("Enable and disable various capabilities.");
					ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
					ImGui::Text(GL::raw_name_of(GL::AlphaTest)); 
					ImGuiExt::Tooltip(String(
						"Param: \'cap\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(GL::AlphaTest),
						util::to_hex<uint32_t>(GL::AlphaTest),
						GL::desc_of(GL::AlphaTest)
					));
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

					int32_t func = GL::index_of(value->func);
					if (ImGuiExt::Combo("##Func", &func, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
					{
						value->func = GL::value_at<GL::Predicate>(func);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'func\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->func),
						util::to_hex<uint32_t>(value->func),
						GL::desc_of(value->func)
					));
					ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::DragFloat("##Ref", &value->coeff);
					ImGuiExt::Tooltip(String(
						"Param: \'ref\'\n\n"
						"Value: {0}\n\n"
						"Brief: {1}\n"
					).format(
						value->coeff,
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
				([](BlendState * value) 
				{	if (!value) { return; }
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID(ML_ADDRESSOF(value));
					ImGui::PushID("##BlendState");
					ImGui::NewLine();
					ImGui::PushItemWidth(288);

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::Checkbox(value->enabled
						? "glEnable" : "glDisable",
						&value->enabled
					);
					ImGuiExt::Tooltip("Enable and disable various capabilities.");
					ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
					ImGui::Text(GL::raw_name_of(GL::Blend));
					ImGuiExt::Tooltip(String(
						"Param: \'cap\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(GL::Blend),
						util::to_hex<uint32_t>(GL::Blend),
						GL::desc_of(GL::Blend)
					));
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

					int32_t sfactorRGB = GL::index_of(value->sfactorRGB);
					if (ImGuiExt::Combo("##sfactorRGB", &sfactorRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
					{
						value->sfactorRGB = GL::value_at<GL::Factor>(sfactorRGB);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'sfactorRGB\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->sfactorRGB),
						util::to_hex<uint32_t>(value->sfactorRGB),
						"Specifies how the red, green, and blue blending factors are computed"
					));
					ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

					/* * * * * * * * * * * * * * * * * * * * */

					int32_t dfactorRGB = GL::index_of(value->dfactorRGB);
					if (ImGuiExt::Combo("##dfactorRGB", &dfactorRGB, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
					{
						value->dfactorRGB = GL::value_at<GL::Factor>(dfactorRGB);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'dfactorRGB\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->dfactorRGB),
						util::to_hex<uint32_t>(value->dfactorRGB),
						"Specifies how the red, green, and blue destination blending factors are computed"
					));
					ImGui::SameLine(); ImGui::Text(","); //ImGui::SameLine();

					/* * * * * * * * * * * * * * * * * * * * */

					int32_t sfactorAlpha = GL::index_of(value->sfactorAlpha);
					if (ImGuiExt::Combo("##sfactorAlpha", &sfactorAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
					{
						value->sfactorAlpha = GL::value_at<GL::Factor>(sfactorAlpha);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'sfactorAlpha\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->sfactorAlpha),
						util::to_hex<uint32_t>(value->sfactorAlpha),
						"Specifies how the alpha source blending factor is computed"
					));
					ImGui::SameLine(); ImGui::Text(","); ImGui::SameLine();

					/* * * * * * * * * * * * * * * * * * * * */

					int32_t dfactorAlpha = GL::index_of(value->dfactorAlpha);
					if (ImGuiExt::Combo("##dfactorAlpha", &dfactorAlpha, GL::Factor_raw_names, ML_ARRAYSIZE(GL::Factor_raw_names)))
					{
						value->dfactorAlpha = GL::value_at<GL::Factor>(dfactorAlpha);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'dfactorAlpha\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->dfactorAlpha),
						util::to_hex<uint32_t>(value->dfactorAlpha),
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
				([](CullState * value)
				{	if (!value) { return; }
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID(ML_ADDRESSOF(value));
					ImGui::PushID("##CullState");
					ImGui::NewLine();
					ImGui::PushItemWidth(200);

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::Checkbox(value->enabled
						? "glEnable" : "glDisable",
						&value->enabled
					);
					ImGuiExt::Tooltip("Enable and disable various capabilities.");
					ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
					ImGui::Text(GL::raw_name_of(GL::CullFace));
					ImGuiExt::Tooltip(String(
						"Param: \'cap\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(GL::CullFace),
						util::to_hex<uint32_t>(GL::CullFace),
						GL::desc_of(GL::CullFace)
					));
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

					int32_t mode = GL::index_of(value->mode);
					if (ImGuiExt::Combo("##Mode", &mode, GL::Face_raw_names, ML_ARRAYSIZE(GL::Face_raw_names)))
					{
						value->mode = GL::value_at<GL::Face>(mode);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'mode\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->mode),
						util::to_hex<uint32_t>(value->mode),
						GL::desc_of(value->mode)
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
				([](DepthState * value)
				{	if (!value) { return; }
					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::PushID(ML_ADDRESSOF(value));
					ImGui::PushID("##DepthState");
					ImGui::NewLine();
					ImGui::PushItemWidth(200);

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::Checkbox(value->enabled
						? "glEnable" : "glDisable",
						&value->enabled
					);
					ImGuiExt::Tooltip("Enable and disable various capabilities.");
					ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
					ImGui::Text(GL::raw_name_of(GL::DepthTest));
					ImGuiExt::Tooltip(String(
						"Param: \'cap\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(GL::DepthTest),
						util::to_hex<uint32_t>(GL::DepthTest),
						GL::desc_of(GL::DepthTest)
					));
					ImGui::SameLine(); ImGui::Text(");");
					ImGui::NewLine();

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::Checkbox("glDepthMask", &value->mask); 
					ImGuiExt::Tooltip("Specifies whether the depth buffer is enabled for writing.");
					ImGui::SameLine(); ImGui::Text("("); ImGui::SameLine();
					ImGui::Text(value->mask ? "true" : "false");
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

					int32_t func = GL::index_of(value->func);
					if (ImGuiExt::Combo("##Func", &func, GL::Predicate_raw_names, ML_ARRAYSIZE(GL::Predicate_raw_names)))
					{
						value->func = GL::value_at<GL::Predicate>(func);
					}
					ImGuiExt::Tooltip(String(
						"Param: \'func\'\n\n"
						"Value: {0} (0x{1})\n\n"
						"Brief: {2}\n"
					).format(
						GL::raw_name_of(value->func),
						util::to_hex<uint32_t>(value->func),
						GL::desc_of(value->func)
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
			
			ImGui::EndChildFrame();
			ImGui::PopID(); 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Noobs & compile_sources()
		{
			if (auto e{ ML_Engine.content().get<Entity>(m_target_name) })
			{
				if (auto r{ e ? e->get<Renderer>() : nullptr })
				{
					if (auto s{ (Shader *)r->shader() })
					{
						for (auto & f : m_files)
						{
							f.dirty = false;
							f.errs.clear();
							f.text.SetErrorMarkers({});
						}

						if (!s->loadFromMemory(
							m_files[Vert].open ? m_files[Vert].text.GetText() : "",
							m_files[Geom].open ? m_files[Geom].text.GetText() : "",
							m_files[Frag].open ? m_files[Frag].text.GetText() : ""
						))
						{
							/* error */
						}
					}
				}
			}
			return (*this);
		}

		Noobs & generate_sources()
		{
			if (auto e{ ML_Engine.content().get<Entity>(m_target_name) })
			{
				if (auto r{ e ? e->get<Renderer>() : nullptr })
				{
					if (auto s{ (Shader *)r->shader() })
					{
						auto setup_file = ([&](size_t type, String const & src)
						{
							m_files[type].dirty = false;
							m_files[type].open = !src.empty();
							m_files[type].text.SetText(src.empty() ? String{} : src.trim());
							m_files[type].text.SetErrorMarkers({});
						});

						setup_file(Vert, s->sources().vs);
						setup_file(Frag, s->sources().fs);
						setup_file(Geom, s->sources().gs);
					}
				}
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::Noobs{};
}