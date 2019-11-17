#include <ML/Editor/Editor_Content.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Engine.hpp>
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

	struct Editor_Content::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		static inline void draw_list()
		{
			// Database
			static auto & db { ML_Engine.content().data<T>() };

			// Plural Name
			static const String plural { ([&](String name)
			{
				if (!name) { return String{}; }
				name = name.substr(name.find_first_of(':') + 2);
				switch (util::to_lower(name).back())
				{
				case 's': return String(name + "es");
				case 'y': return String(name.substr(0, name.size() - 1) + "ies");
				default	: return String(name + "s");
				}
			})(typeof<T>::name) };

			// Tab Item
			const bool tab_visible { ImGui::BeginTabItem(plural.c_str()) };
			
			// Tab Context Menu
			if (ImGui::BeginPopupContextItem(("##TabContextMenu##" + plural).c_str()))
			{
				voidptr_t temp { nullptr };
				if (!std::is_same_v<T, Surface> && !std::is_same_v<T, Uniform>)
				{
					if (PropertyDrawer<T>()(("New {0}"_s).format(
						nameof<>::filter_namespace(typeof<T>::name)
					), (ptr_t<T> &)temp))
					{
						ImGui::CloseCurrentPopup();
					}
				}
				else
				{
					ImGui::Text("No actions available");
				}
				ImGui::EndPopup();
			}
			
			// Tab Content
			if (!tab_visible) { return; }

			// Draw Items
			ImGui::PushID(ML_ADDRESSOF(&db));
			if (db.empty()) { ImGui::Text("-"); }
			auto to_remove { db.end() }, to_select { db.end() };
			for (auto it = db.begin(); it != db.end(); it++)
			{
				if (!it->second) { continue; }
				
				ImGui::PushID(it->first.c_str());

				// Item Selectable
				if (ImGui::Selectable(
					(it->first + "##" + plural).c_str(), 
					(ML_Editor.content().m_selected == it->second)
				))
				{
					to_select = it;
				}
				
				// Item Context Menu
				if (ImGui::BeginPopupContextItem(("##ItemContextMenu##" + plural).c_str()))
				{
					bool no_delete { false };

					if (std::is_same_v<T, Uniform>)
					{
						if (auto u { (ptr_t<Uniform>)it->second })
						{
							no_delete = !u->isModifiable();
						}
					}
					else if (std::is_same_v<T, Surface>) no_delete = true;

					// Delete
					if (!no_delete &&
						ImGuiExt::Confirm(
						String("Delete {0}?").format(typeof<T>::name),
						ImGui::Button("Delete"),
						String("Are you sure you want to delete {0}: \'{1}\'?").format(typeof<T>::name, it->first)
					) == 1)
					{
						to_remove = it;
					}

					ImGui::EndPopup();
				}
				ImGui::PopID();
				ImGui::Separator();
			}
			if (to_remove != db.end())
			{
				ML_Engine.content().destroy<T>(to_remove->first);
				ML_Editor.content().select_none();
			}
			if (to_select != db.end())
			{
				ML_Editor.inspector().Focus(true);
				ML_Editor.content().select_item(typeof<T>::name, to_select->first, to_select->second);
			}
			ImGui::PopID();
			ImGui::EndTabItem();
		}
		
		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Content::Editor_Content()
		: EditorComponent { "Content", "Ctrl+Alt+C", ML_Engine.prefs().get_bool("Editor", "show_content", false) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Content::update()
	{
	}

	bool Editor_Content::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

		if (beginDraw(ImGuiWindowFlags_None))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ImGuiStyle & style { ImGui::GetStyle() };
			
			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			ImGui::BeginChildFrame(
				ImGui::GetID(("##Content##"_s + getTitle()).c_str()),
				{ max_size[0], max_size[1] },
				ImGuiWindowFlags_NoScrollbar
			);
			ImGui::BeginTabBar(
				"Content Type Tabs", 
				ImGuiTabBarFlags_TabListPopupButton |
				ImGuiTabBarFlags_FittingPolicyScroll
			);
			Layout::draw_list<Entity>();
			Layout::draw_list<Font>();
			Layout::draw_list<Image>();
			Layout::draw_list<Material>();
			Layout::draw_list<Model>();
			Layout::draw_list<Shader>();
			Layout::draw_list<Script>();
			//Layout::draw_list<Sprite>();
			Layout::draw_list<Surface>();
			Layout::draw_list<Texture>();
			Layout::draw_list<Uniform>();
			ImGui::EndTabBar();
			ImGui::EndChildFrame();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}