#include <ML/Editor/EditorContent.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Editor/PropertyDrawer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EditorContent::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		static inline void draw_list(const GuiEvent & ev)
		{
			// Data
			static ContentManager::ObjectDatabase & database { ML_Content.data<T>() };

			// Type Name
			static const String type_name { PropertyDrawer<T>::type_name().str() };

			// Plural Name
			static const String label { ([&]()
			{
				if (!type_name) { return String(); }
				switch (alg::to_lower(type_name).back())
				{
				case 's': return String(type_name + "es");
				case 'y': return String(type_name.substr(0, type_name.size() - 1) + "ies");
				default	: return String(type_name + "s");
				}
			})() };

			const bool has_selected_type { type_name == ev.editor.content().m_typename };
			if (has_selected_type)
			{
				ImGui::PushStyleColor(ImGuiCol_Header, { 1.0f, 1.0f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.0f, 1.0f });
			}

			// Header
			const bool header { ImGui::CollapsingHeader(label.c_str()) };
			if (has_selected_type) ImGui::PopStyleColor(2);
			if (!header) { return; }

			// Empty
			if (database.empty()) { ImGui::Text("-"); return; }

			ImGui::PushID(ML_ADDRESSOF(&database));
			ImGui::BeginGroup();
			// Create Item
			{
				T * temp { nullptr };
				if (PropertyDrawer<T>()(label, (T *&)temp, 0b1)) {}
			}
			// Draw Items
			for (auto & pair : database)
			{
				if (!pair.second || ImGuiExt::IsHidden(pair.first)) { continue; }

				const bool is_selected { ev.editor.content().m_selected == pair.second };
				ImGui::PushID(pair.first.c_str());
				ImGui::PushStyleColor(ImGuiCol_Header, { 1.0f, 1.0f, 0.8f, 1.0f });
				if (is_selected)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.0f, 1.0f });
				}
				if (ImGui::Selectable((pair.first + "##" + label).c_str(), is_selected))
				{
					ev.editor.content().m_typename = type_name;
					ev.editor.content().m_selected = pair.second;
				}
				ImGui::PopStyleColor(1 + is_selected);
				ImGui::PopID();
				ImGui::Separator();
			}
			ImGui::EndGroup();
			ImGui::PopID();
		}
		
		/* * * * * * * * * * * * * * * * * * * * */
		template <class T>
		inline static void draw_inspector(void * ptr)
		{
			static const String type_name { PropertyDrawer<T>::type_name().str() };
			if (!ptr) return;
			ImGui::PushID(type_name.c_str());
			ImGui::PushID(ML_ADDRESSOF(&ML_Content.data<T>()));
			ImGui::PushID(ptr);
			PropertyDrawer<T>()((type_name + " ##Inspector"), ((T &)*static_cast<T *>(ptr)));
			ImGui::PopID();
			ImGui::PopID();
			ImGui::PopID();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorContent::EditorContent(Editor & editor)
		: EditorForm { editor, "Content Manager", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorContent::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorContent::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			if (ImGui::BeginChild(
				("##Content##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] * 0.5f },
				true,
				ImGuiWindowFlags_MenuBar
			))
			{
				ImGui::PopStyleVar();
				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("Database");
					ImGui::EndMenuBar();
				}
				Layout::draw_list<Entity>(ev);
				Layout::draw_list<Font>(ev);
				Layout::draw_list<Image>(ev);
				Layout::draw_list<Material>(ev);
				Layout::draw_list<Model>(ev);
				Layout::draw_list<Shader>(ev);
				Layout::draw_list<Script>(ev);
				Layout::draw_list<Sprite>(ev);
				Layout::draw_list<Surface>(ev);
				Layout::draw_list<Texture>(ev);
				Layout::draw_list<Uniform>(ev);
			}
			else
			{
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			if (ImGui::BeginChild(
				("##Inspector##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] * 0.5f },
				true,
				ImGuiWindowFlags_MenuBar
			))
			{
				ImGui::PopStyleVar();
				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("Inspector");
					ImGui::EndMenuBar();
				}
				switch (Hash { m_typename.data(), m_typename.size() })
				{
				case PropertyDrawer<Entity>::hash_code():	Layout::draw_inspector<Entity>(m_selected); break;
				case PropertyDrawer<Font>::hash_code():		Layout::draw_inspector<Font>(m_selected); break;
				case PropertyDrawer<Image>::hash_code():	Layout::draw_inspector<Image>(m_selected); break;
				case PropertyDrawer<Material>::hash_code():	Layout::draw_inspector<Material>(m_selected); break;
				case PropertyDrawer<Model>::hash_code():	Layout::draw_inspector<Model>(m_selected); break;
				case PropertyDrawer<Script>::hash_code():	Layout::draw_inspector<Script>(m_selected); break;
				case PropertyDrawer<Shader>::hash_code():	Layout::draw_inspector<Shader>(m_selected); break;
				case PropertyDrawer<Sound>::hash_code():	Layout::draw_inspector<Sound>(m_selected); break;
				case PropertyDrawer<Sprite>::hash_code():	Layout::draw_inspector<Sprite>(m_selected); break;
				case PropertyDrawer<Surface>::hash_code():	Layout::draw_inspector<Surface>(m_selected); break;
				case PropertyDrawer<Texture>::hash_code():	Layout::draw_inspector<Texture>(m_selected); break;
				case PropertyDrawer<Uniform>::hash_code():	Layout::draw_inspector<Uniform>(m_selected); break;
				}
			}
			else
			{
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}