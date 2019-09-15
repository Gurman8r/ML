#include <ML/Editor/EditorContent.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/TypeTag.hpp>
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

			// Tab Item
			const bool visible { ImGui::BeginTabItem(label.c_str()) };
			
			// Context Menu
			if (ImGui::BeginPopupContextItem(("##MainContextMenu##" + label).c_str()))
			{
				void * temp { nullptr };
				if (PropertyDrawer<T>()(String("New {0}").format(type_name), (T *&)temp, 1))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
			if (visible)
			{
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

				ImGui::EndTabItem();
			}
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