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
			// Self
			EditorContent & self { ev.editor.content() };

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

			// Tab Item
			const bool has_selected { self.m_typename == type_name };
			if (has_selected)
			{
				ImGui::PushStyleColor(ImGuiCol_Tab, { 1.0f, 1.0f, 0.8f, 1.0f });
			}
			const bool tab_visible { ImGui::BeginTabItem(label.c_str()) };
			if (has_selected) 
			{ 
				ImGui::PopStyleColor(1); 
			}
			
			// Tab Context Menu
			if (ImGui::BeginPopupContextItem(("##TabContextMenu##" + label).c_str()))
			{
				void * temp { nullptr };
				if (PropertyDrawer<T>()(String("New {0}").format(type_name), (T *&)temp, 1))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
			// Tab Content
			if (!tab_visible) { return; }

			// Draw Items
			ImGui::PushID(ML_ADDRESSOF(&database));
			if (database.empty()) { ImGui::Text("-"); }
			ContentManager::ObjectDatabase::iterator to_remove { database.end() };
			ContentManager::ObjectDatabase::iterator to_select { database.end() };
			for (auto it = database.begin(); it != database.end(); it++)
			{
				if (!it->second || ImGuiExt::IsHidden(it->first)) { continue; }
				
				ImGui::PushID(it->first.c_str());

				// Item Selectable
				const bool is_selected { self.m_selected == it->second };
				if (is_selected)
				{
					ImGui::PushStyleColor(ImGuiCol_Header, { 1.0f, 1.0f, 0.8f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.0f, 1.0f });
				}
				if (ImGui::Selectable((it->first + "##" + label).c_str(), is_selected))
				{
					to_select = it;
				}
				ImGui::PopStyleColor((int32_t)is_selected * 2);
				
				// Item Context Menu
				if (ImGui::BeginPopupContextItem(("##ItemContextMenu##" + label).c_str()))
				{
					// Inspect Button
					if (ImGui::Button("Inspect"))
					{
						ev.editor.inspector().setOpen(true);
						ImGui::SetWindowFocus(ev.editor.inspector().getTitle());
						to_select = it;
					}
					// Delete Button
					if (ImGuiExt::Confirm(
						String("Delete {0}?").format(type_name),
						ImGui::Button("Delete"),
						String("Are you sure you want to delete {0}: \'{1}\'?").format(type_name, it->first)
					) == 1)
					{
						to_remove = it;
					}

					ImGui::EndPopup();
				}
				ImGui::PopID();
				ImGui::Separator();
			}
			if (to_remove != database.end())
			{
				ML_Content.erase<T>(to_remove->first);
				self.m_typename = String();
				self.m_itemname = String();
				self.m_selected = nullptr;
			}
			if (to_select != database.end())
			{
				self.m_typename = type_name;
				self.m_itemname = to_select->first;
				self.m_selected = to_select->second;
			}
			ImGui::PopID();
			ImGui::EndTabItem();
		}
		
		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorContent::EditorContent(Editor & editor)
		: EditorWindow { editor, "Content", false }
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

			ImGuiStyle & style { ImGui::GetStyle() };
			
			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(
				("##Content##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_None
			);
			ImGui::PopStyleVar();
			ImGui::BeginTabBar("Content Type Tabs", ImGuiTabBarFlags_FittingPolicyScroll);
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
			ImGui::EndTabBar();
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}