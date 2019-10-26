/* * * * * * * * * * * * * * * * * * * * */

#include "Noobs.hpp"
#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
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
#include <ML/Editor/AssetPreview.hpp>

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
					cout << ev->error;

					// Decode Errors
					SStream ss { String{ ev->error } };
					String line;
					while (std::getline(ss, line, '\n'))
					{
						const ShaderError err { ev->type, line };
						switch (ev->type)
						{
						case GL::VertexShader	: m_files[ShaderFile::Vert]->errs.push_back(err); break;
						case GL::FragmentShader	: m_files[ShaderFile::Frag]->errs.push_back(err); break;
						case GL::GeometryShader	: m_files[ShaderFile::Geom]->errs.push_back(err); break;
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
					ImGui::Separator();
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
				d.dockWindow(display_name, d.getNode(d.LeftUp));
				d.dockWindow(editor_name, d.getNode(d.RightUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onStart(const StartEvent & ev)
	{
		// Setup Editor
		if (const String ent_name { ML_Engine.prefs().get_string("Noobs", "target_entity", "") })
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

		Camera * camera { Camera::mainCamera() };
		if (camera && (*camera))
		{
			// Update Camera
			camera->setViewport((vec2i)m_viewport);
			
			// Update Viewports
			for (auto & surf : m_pipeline)
			{
				surf->update((vec2)camera->viewport().size());
			}

			// Update Materials
			for (auto & [key, value] : ML_Content.data<Material>())
			{
				if (auto m { (Material *)value })
				{
					// Update Uniforms (slow)
					m->set<uni_vec3>("u_camera.pos", camera->position());
					m->set<uni_vec3>("u_camera.dir", camera->direction());
					m->set<uni_float>("u_camera.fov", camera->fieldOfView());
					m->set<uni_float>("u_camera.near", camera->clipNear());
					m->set<uni_float>("u_camera.far", camera->clipFar());
					m->set<uni_vec2>("u_camera.view", (vec2)camera->viewport().size());
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
			if (camera) camera->apply();

			// Draw Renderers
			for (auto & [key, value] : ML_Content.data<Entity>())
			{
				if (auto ent { (Entity *)value })
				{
					auto renderer { ent->get<Renderer>() };
					if (renderer && (*renderer))
					{
						auto transform { ent->get<Transform>() };
						if (transform && (*transform))
						{
							if (auto m { (Material *)renderer->material() })
							{
								// Update Uniforms (slow)
								m->set<uni_vec3>("u_position", transform->position());
								m->set<uni_vec4>("u_rotation", transform->rotation());
								m->set<uni_vec3>("u_scale", transform->scale());
							}
						}
					}
					ML_Engine.window().draw(renderer);
				}
			}

			// Unbind Surface
			m_pipeline[Surf_Main]->unbind();
		}

		// Render Post Processing
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_pipeline[Surf_Post])
		{
			// Bind Surface
			m_pipeline[Surf_Post]->bind();

			// Apply Camera
			if (camera) camera->apply();

			// Reset States
			RenderStates {
				AlphaState	{ true, GL::Greater, 0.01f },
				BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
				CullState	{ false },
				DepthState	{ false },
			}();

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
		draw_display(display_name, m_pipeline[Surf_Post]);

		// Render Editor
		draw_editor(editor_name);
	}

	void Noobs::onUnload(const UnloadEvent & ev)
	{
		dispose_files();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Noobs::draw_display(C_String title, const Surface * surf)
	{
		if (!m_display_open) return;
		ImGui::PushID(ML_ADDRESSOF(this));
		if (ImGui::Begin(title, &m_display_open,
			ImGuiWindowFlags_None
		))
		{
			ML_AssetPreview.drawPreview(surf, ImGuiExt::GetContentRegionAvail(), [&] 
			{
				if (m_freeAspect) { m_viewport = ImGuiExt::GetContentRegionAvail(); }
			});
		}
		ImGui::End();
		ImGui::PopID();
	}
	
	void Noobs::draw_editor(C_String title)
	{
		if (!m_editor_open) return;
		ImGui::PushID(ML_ADDRESSOF(this));
		if (ImGui::Begin(title, &m_editor_open, 
			ImGuiWindowFlags_None
		))
		{
			if (ImGui::BeginTabBar(
				"DemoEditor##TabBar##Main",
				ImGuiTabBarFlags_Reorderable
			))
			{
				if (ImGui::BeginTabItem("Code")) { draw_code_tab(); }
				if (ImGui::BeginTabItem("Uniforms")) { draw_uniforms_tab(); }
				if (ImGui::BeginTabItem("Settings")) { draw_settings_tab(); }

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::draw_code_tab()
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

				ImGuiExt::Tooltip((
					"" + file->name + " Shader Source"
				).c_str());
				
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
					if (ImGui::MenuItem(("Reload##" + file->name).c_str()))
					{
						const String * src { nullptr };
						switch (file->type)
						{
						case ShaderFile::Vert: src = &m_renderer->shader()->sources().vs; break;
						case ShaderFile::Frag: src = &m_renderer->shader()->sources().fs; break;
						case ShaderFile::Geom: src = &m_renderer->shader()->sources().gs; break;
						}
						if (src) { file->text.SetText(*src); }
						ImGui::CloseCurrentPopup();
					}
					// Copy
					if (ImGui::MenuItem(("Copy to Clipboard##" + file->name).c_str()))
					{
						ML_Engine.window().setClipboardString(file->text.GetText());
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

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::EndChild();
		ImGui::PopID();
		ImGui::EndTabItem();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Noobs::draw_uniforms_tab()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::PushID("##Uniforms");
		ImGui::BeginChildFrame(ImGui::GetID("##Uniforms##Content"), { 0, 0 }, 0);

		/* * * * * * * * * * * * * * * * * * * * */

		ImGuiExt::HelpMarker(
			"** Note:\n"
			"Some values may be overwritten internally,\n"
			"even if they aren't marked as read-only.\n"
		);
		ImGui::SameLine();

		// New Uniform Popup
		Uniform * to_add { nullptr };
		if (PropertyDrawer<Uniform>()("New Uniform##Noobs", (Uniform *&)to_add))
		{
			// Already Exists
			if (to_add && !m_renderer->material()->insert(to_add))
			{
				Debug::logError("A uniform with the name \'{0}\' already exists",
					to_add->name
				);
				delete to_add;
			}
		}

		ImGui::SameLine();

		// Copy to Clipboard
		if (ImGui::Button("Copy to Clipboard"))
		{
			SStream ss;
			for (const auto & u : (*m_renderer->material()))
			{
				if (!u) continue;
				ss	<< std::left
					<< "uniform " 
					<< std::setw(7) << util::to_string(*u) << " "
					<< std::setw(15) << u->name << " "
					<< "{ ";
				switch (u->getID())
				{
				case uni_bool	::ID: if (auto a { detail::as_bool(u) }) ss << (*a); break;
				case uni_float	::ID: if (auto a { detail::as_float(u) }) ss << (*a); break;
				case uni_int	::ID: if (auto a { detail::as_int(u) }) ss << (*a); break;
				case uni_vec2	::ID: if (auto a { detail::as_vec2(u) }) ss << (*a); break;
				case uni_vec3	::ID: if (auto a { detail::as_vec3(u) }) ss << (*a); break;
				case uni_vec4	::ID: if (auto a { detail::as_vec4(u) }) ss << (*a); break;
				case uni_color	::ID: if (auto a { detail::as_color(u) }) ss << (*a); break;
				case uni_mat2	::ID: if (auto a { detail::as_mat2(u) }) ss << (*a); break;
				case uni_mat3	::ID: if (auto a { detail::as_mat3(u) }) ss << (*a); break;
				case uni_mat4	::ID: if (auto a { detail::as_mat4(u) }) ss << (*a); break;
				case uni_sampler::ID: if (auto a { detail::as_sampler(u) }) ss << ML_Content.get_name(*a); break;
				}
				if (ss.str().back() != ' ') ss << ' ';
				ss << "}" << endl;
			}
			ML_Engine.window().setClipboardString(ss.str());
		}

		ImGui::Separator();

		/* * * * * * * * * * * * * * * * * * * * */

		static bool setup_uni_columns { false };
		// Uniform Column Headers
		ImGui::Columns(3, "##Uni##Columns");
		if (ImGui::Selectable("Name"))
		{
			auto sort_name_ascending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->name) < (rhs->name); }); });

			auto sort_name_descending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->name) > (rhs->name); }); });

			static bool state { 0 };
			if (state = !state) { sort_name_ascending(); }
			else { sort_name_descending(); }
		}
		ImGui::NextColumn();
		if (!setup_uni_columns)
		{
			ImGui::SetColumnWidth(-1, 
				ImGui::GetWindowContentRegionWidth() * 0.1f);
		}
		if (ImGui::Selectable("Type"))
		{
			auto sort_type_ascending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->getID()) < (rhs->getID()); }); });

			auto sort_type_descending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->getID()) > (rhs->getID()); }); });

			static bool state { 0 };
			if (state = !state) { sort_type_ascending(); }
			else { sort_type_descending(); }
		}
		ImGui::NextColumn();
		if (ImGui::Selectable("Value")) 
		{
			auto sort_value_ascending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->isModifiable()) < (rhs->isModifiable()); }); });

			auto sort_value_descending = ([&]() { std::sort(
				m_renderer->material()->begin(),
				m_renderer->material()->end(),
				[](auto lhs, auto rhs) { return (lhs->isModifiable()) > (rhs->isModifiable()); }); });

			static bool state { 0 };
			if (state = !state) { sort_value_ascending(); }
			else { sort_value_descending(); }
		}
		ImGui::NextColumn();
		ImGui::Separator(); ImGui::Columns(1);
		if (!setup_uni_columns) { setup_uni_columns = true; }

		/* * * * * * * * * * * * * * * * * * * * */
					
		// Uniform List
		Uniform * to_remove { nullptr };
		for (auto & u : (*m_renderer->material()))
		{
			if (!u) continue;
			ImGui::Columns(3, "##Uni##Columns");
			const String label { "##Uni##" + u->name };

			// Uniform Name
			/* * * * * * * * * * * * * * * * * * * * */

			if (u->isModifiable())
			{
				static char name[32] = "";
				std::strcpy(name, u->name.c_str());
				if (ImGui::InputText(
					(label + "##Name").c_str(),
					name,
					ML_ARRAYSIZE(name),
					ImGuiInputTextFlags_EnterReturnsTrue
				))
				{
					if (!m_renderer->material()->get(name))
					{
						u->name = name;
					}
					else
					{
						Debug::logError("A uniform with that name already exists");
					}
				}
			}
			else
			{
				ImGui::Text(u->name.c_str());
			}
			ImGui::NextColumn();
						
			// Uniform FileType
			/* * * * * * * * * * * * * * * * * * * * */
			ImGui::Text("%s", util::to_string(*u).c_str());
			ImGui::NextColumn();

			// Uniform Value
			/* * * * * * * * * * * * * * * * * * * * */
			ImGui::PushID(label.c_str());
			if (PropertyDrawer<Uniform>()(label, (Uniform &)(*u)))
			{
				// Remove Uniform
				ImGui::SameLine();
				if (ImGuiExt::Confirm(
					"Delete Uniform",
					ImGui::Button(("X##" + label).c_str()),
					"Are you sure you want to delete this Uniform?"
				) == 1)
				{
					to_remove = u;
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

	void Noobs::draw_settings_tab()
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImGui::PushID("##Settings");
		ImGui::BeginChildFrame(ImGui::GetID("##Settings##Content"), { 0, 0 }, 0);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const bool show_general_settings {
			ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)
		};
		if (show_general_settings)
		{
			// Select Material
			const Material * mtl { m_renderer ? m_renderer->material() : nullptr };
			if (PropertyDrawer<Material>()("Material##Renderer##Noobs", mtl))
			{
				m_renderer->setMaterial(mtl);
				reset_sources();
				generate_sources();
			}
			ImGuiExt::Tooltip("Select the uniform set to use.");
			ImGui::Separator();

			// Select Shader
			const Shader * shd { m_renderer ? m_renderer->shader() : nullptr };
			if (PropertyDrawer<Shader>()("Shader##Renderer##Noobs", shd))
			{
				m_renderer->setShader(shd);
				reset_sources();
				generate_sources();
			}
			ImGuiExt::Tooltip("Select the shader program to use.");
			ImGui::Separator();

			// Select Model
			const Model * mdl { m_renderer ? m_renderer->model() : nullptr };
			if (PropertyDrawer<Model>()("Model##Renderer##Noobs", mdl))
			{
				m_renderer->setModel(mdl);
			}
			ImGuiExt::Tooltip("Select the geometry to be drawn.");
			ImGui::Separator();

			if (auto c { Camera::mainCamera() })
			{
				// Get Video Modes
				static const List<VideoMode> & mode_values { Window::getFullscreenModes() };

				// Get Video Names
				static const List<String> & mode_names = [&]
				{
					static List<String> temp { "Free" };
					for (const auto & elem : mode_values)
						temp.push_back(util::to_string(elem.size));
					return temp;
				}();

				// Viewport
				static int32_t index { 0 };
				if (ImGuiExt::Combo("Viewport", &index, mode_names))
				{
					m_freeAspect = (index == 0);
				}
				if (!m_freeAspect)
				{
					m_viewport = (vec2i)mode_values[index - 1].size;
				}
				ImGuiExt::Tooltip("Specify the display resolution.");

				ImGui::Separator();

				vec4 background { c->background() };
				// Background Color
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
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const bool show_render_settings { 
			ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen)
		};
		if (show_render_settings)
		{
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
					Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml");
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
					Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glBlendFuncSeparate.xml");
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
					Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml");
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
					Debug::execute("open", "https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml");
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

				/* * * * * * * * * * * * * * * * * * * * */
			})(&m_renderer->states().depth());

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ImGui::EndChildFrame();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImGui::EndChildFrame();
		ImGui::PopID(); 
		ImGui::EndTabItem();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Noobs::compile_sources()
	{
		if (m_renderer && m_renderer->material())
		{
			if (auto s { m_renderer->shader() })
			{
				for (auto & f : m_files)
				{
					f->dirty = false;
					f->errs.clear();
					f->text.SetErrorMarkers({});
				}

				return s->loadFromMemory(
					m_files[ShaderFile::Vert]->open ? m_files[ShaderFile::Vert]->text.GetText() : "",
					m_files[ShaderFile::Geom]->open ? m_files[ShaderFile::Geom]->text.GetText() : "",
					m_files[ShaderFile::Frag]->open ? m_files[ShaderFile::Frag]->text.GetText() : ""
				);
			}
		}
		return false;
	}

	bool Noobs::dispose_files()
	{
		for (auto & file : m_files)
		{
			if (file) delete file;
		}
		return true;
	}

	void Noobs::generate_sources()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto setup_file = [&](ShaderFile::FileType type, const String & src)
		{
			if (m_renderer->material() && m_renderer->shader())
			{
				if (!m_files[type])
				{
					m_files[type] = new ShaderFile(type, src);
				}
				else
				{
					m_files[type]->text.SetText(src);
				}
				return m_files[type];
			}
			return (ShaderFile *)nullptr;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (!m_renderer->material()) return;

		if (auto vert = setup_file(ShaderFile::Vert, m_renderer->shader()->sources().vs))
		{
			vert->open = m_renderer->shader()->sources().vs;
		}

		if (auto frag = setup_file(ShaderFile::Frag, m_renderer->shader()->sources().fs))
		{
			frag->open = m_renderer->shader()->sources().fs;
		}

		if (auto geom = setup_file(ShaderFile::Geom, m_renderer->shader()->sources().gs))
		{
			geom->open = m_renderer->shader()->sources().gs;
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