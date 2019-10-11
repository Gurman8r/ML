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

	struct EditorInspector::Layout
	{
		template <class T>
		inline static void draw_item(void * ptr, const String & name)
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorInspector::EditorInspector()
		: EditorWindow { "Inspector", "Ctrl+Alt+I", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorInspector::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorInspector::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			EditorContent & con { ML_Editor.content() };

			/* * * * * * * * * * * * * * * * * * * * */

			ImGuiStyle & style { ImGui::GetStyle() };

			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(
				("##Inspector##" + String(getTitle())).c_str(),
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_MenuBar
			);
			ImGui::PopStyleVar();
			if (ImGui::BeginMenuBar())
			{
				if (ML_Editor.content().m_selected)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.1f, 1.0f, 1.0f });
					ImGui::Text("[%s]", con.m_typename.c_str());
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::Text("\'%s\'", con.m_itemname.c_str());
				}
				else
				{
					ImGui::Text("Nothing selected");
				}
				ImGui::EndMenuBar();
			}
			switch (Hash { con.m_typename.data(), con.m_typename.size() })
			{
			case PropertyDrawer<Entity>::hash_code():	Layout::draw_item<Entity>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Font>::hash_code():		Layout::draw_item<Font>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Image>::hash_code():	Layout::draw_item<Image>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Material>::hash_code():	Layout::draw_item<Material>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Model>::hash_code():	Layout::draw_item<Model>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Script>::hash_code():	Layout::draw_item<Script>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Shader>::hash_code():	Layout::draw_item<Shader>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Sound>::hash_code():	Layout::draw_item<Sound>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Sprite>::hash_code():	Layout::draw_item<Sprite>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Surface>::hash_code():	Layout::draw_item<Surface>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Texture>::hash_code():	Layout::draw_item<Texture>(con.m_selected, con.m_itemname); break;
			case PropertyDrawer<Uniform>::hash_code():	Layout::draw_item<Uniform>(con.m_selected, con.m_itemname); break;
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}