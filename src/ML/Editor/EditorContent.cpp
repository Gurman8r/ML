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

			// Highlight Group
			const bool has_selected_type { type_name == ev.editor.content().m_typename };
			if (has_selected_type)
			{
				ImGui::PushStyleColor(ImGuiCol_Header, { 0.7f, 0.7f, 0.5f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.0f, 1.0f });
			}

			// Header
			const bool header { ImGui::CollapsingHeader(label.c_str()) };
			if (has_selected_type) ImGui::PopStyleColor(2);
			
			// Main Context Menu
			if (ImGui::BeginPopupContextItem(("##MainContextMenu##" + label).c_str()))
			{
				T * temp { nullptr };
				if (PropertyDrawer<T>()(label, (T *&)temp, 0b1))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			// Hidden
			if (!header) { return; }

			// Draw Items
			ImGui::PushID(ML_ADDRESSOF(&database));
			ImGui::BeginGroup();
			if (database.empty()) { ImGui::Text("-"); }
			for (auto & pair : database)
			{
				if (!pair.second || ImGuiExt::IsHidden(pair.first)) { continue; }

				ImGui::PushID(pair.first.c_str());

				const bool is_selected { ev.editor.content().m_selected == pair.second };
				if (is_selected)
				{
					ImGui::PushStyleColor(ImGuiCol_Header, { 1.0f, 1.0f, 0.8f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.0f, 1.0f });
				}
				// Selectable
				if (ImGui::Selectable((pair.first + "##" + label).c_str(), is_selected))
				{

					ev.editor.content().m_typename = type_name;
					ev.editor.content().m_selected = pair.second;
					ev.editor.content().m_itemname = pair.first;
				}
				ImGui::PopStyleColor((int32_t)is_selected * 2);
				ImGui::PopID();
				ImGui::Separator();
			}
			ImGui::EndGroup();
			ImGui::PopID();
		}
		
		/* * * * * * * * * * * * * * * * * * * * */
		template <class T>
		inline static void draw_inspector(void * ptr, const String & name)
		{
			static const String type_name { PropertyDrawer<T>::type_name().str() };
			if (!ptr) return;
			ImGui::PushID(type_name.c_str());
			ImGui::PushID(ML_ADDRESSOF(&ML_Content.data<T>()));
			ImGui::PushID(ptr);
			PropertyDrawer<T>()(
				(type_name + "##" + name + " ##Inspector"),
				((T &)*static_cast<T *>(ptr))
			);
			ImGui::PopID();
			ImGui::PopID();
			ImGui::PopID();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorContent::EditorContent(Editor & editor)
		: EditorForm { editor, "Content", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorContent::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorContent::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_NoScrollbar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ImGuiStyle & style { ImGui::GetStyle() };
			
			const vec2 max_size { 
				ImGuiExt::GetContentRegionAvail() * vec2 { 1.0f, 0.5f } 
			};

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(
				("##Content##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_None
			);
			ImGui::PopStyleVar();
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { style.ItemSpacing.x, 5 });
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { style.FramePadding.x, 8 });
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
				ImGui::PopStyleVar(2);
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(
				("##Inspector##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_MenuBar
			);
			ImGui::PopStyleVar();
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.8f, 1.0f, 1.0f });
					ImGui::Text("[%s]", m_typename.c_str());
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 1.0f, 0.8f, 1.0f });
					ImGui::Text("\'%s\'", m_itemname.c_str());
					ImGui::PopStyleColor();
					ImGui::EndMenuBar();
				}
				switch (Hash { m_typename.data(), m_typename.size() })
				{
				case PropertyDrawer<Entity>::hash_code():	Layout::draw_inspector<Entity>(m_selected, m_itemname); break;
				case PropertyDrawer<Font>::hash_code():		Layout::draw_inspector<Font>(m_selected, m_itemname); break;
				case PropertyDrawer<Image>::hash_code():	Layout::draw_inspector<Image>(m_selected, m_itemname); break;
				case PropertyDrawer<Material>::hash_code():	Layout::draw_inspector<Material>(m_selected, m_itemname); break;
				case PropertyDrawer<Model>::hash_code():	Layout::draw_inspector<Model>(m_selected, m_itemname); break;
				case PropertyDrawer<Script>::hash_code():	Layout::draw_inspector<Script>(m_selected, m_itemname); break;
				case PropertyDrawer<Shader>::hash_code():	Layout::draw_inspector<Shader>(m_selected, m_itemname); break;
				case PropertyDrawer<Sound>::hash_code():	Layout::draw_inspector<Sound>(m_selected, m_itemname); break;
				case PropertyDrawer<Sprite>::hash_code():	Layout::draw_inspector<Sprite>(m_selected, m_itemname); break;
				case PropertyDrawer<Surface>::hash_code():	Layout::draw_inspector<Surface>(m_selected, m_itemname); break;
				case PropertyDrawer<Texture>::hash_code():	Layout::draw_inspector<Texture>(m_selected, m_itemname); break;
				case PropertyDrawer<Uniform>::hash_code():	Layout::draw_inspector<Uniform>(m_selected, m_itemname); break;
				}
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}