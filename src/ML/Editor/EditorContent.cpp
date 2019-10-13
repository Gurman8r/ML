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
			EditorContent & self { ML_Editor.content() };

			// Data
			static auto & database { ML_Content.data<T>() };

			// Type Name
			static const String type_name { PropertyDrawer<T>::type_name().str() };

			// Plural Name
			static const String label { ([&]()
			{
				if (!type_name) { return String(); }
				switch (util::to_lower(type_name).back())
				{
				case 's': return String(type_name + "es");
				case 'y': return String(type_name.substr(0, type_name.size() - 1) + "ies");
				default	: return String(type_name + "s");
				}
			})() };

			// Tab Item
			const bool tab_visible { ImGui::BeginTabItem(label.c_str()) };
			
			// Tab Context Menu
			if (ImGui::BeginPopupContextItem(("##TabContextMenu##" + label).c_str()))
			{
				void * temp { nullptr };
				if (!std::is_same_v<T, Surface>)
				{
					if (PropertyDrawer<T>()(String("New {0}").format(type_name), (T *&)temp, 0b1))
					{
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			
			// Tab Content
			if (!tab_visible) { return; }

			// Draw Items
			ImGui::PushID(ML_ADDRESSOF(&database));
			if (database.empty()) { ImGui::Text("-"); }
			auto to_remove { database.end() }, to_select { database.end() };
			for (auto it = database.begin(); it != database.end(); it++)
			{
				if (!it->second || ImGuiExt::IsHidden(it->first)) { continue; }
				
				ImGui::PushID(it->first.c_str());

				// Item Selectable
				if (ImGui::Selectable(
					(it->first + "##" + label).c_str(), 
					(self.m_selected == it->second)
				))
				{
					to_select = it;
				}
				
				// Item Context Menu
				if (ImGui::BeginPopupContextItem(("##ItemContextMenu##" + label).c_str()))
				{
					bool no_delete { false };

					if (std::is_same_v<T, Uniform>)
					{
						if (auto u { static_cast<Uniform *>(it->second) })
						{
							if (!u->isModifiable())
							{
								no_delete = true;
							}
						}
					}

					// Delete
					if (!no_delete &&
						ImGuiExt::Confirm(
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
				ML_Content.destroy<T>(to_remove->first);
				self.m_typename = String();
				self.m_itemname = String();
				self.m_selected = nullptr;
			}
			if (to_select != database.end())
			{
				ML_Editor.inspector().Focus(true);
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

	EditorContent::EditorContent()
		: EditorWindow { "Content", "Ctrl+Alt+C", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorContent::update(const UpdateEvent & ev)
	{
	}

	bool EditorContent::draw(const GuiEvent & ev)
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
			//Layout::draw_list<Sprite>(ev);
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