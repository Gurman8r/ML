#include <ML/Editor/EditorContent.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Ref.hpp>
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
			if (!ptr) return;
			ImGui::PushID(typeof<T>::name.c_str());
			ImGui::PushID(ML_ADDRESSOF(&ML_Engine.content().data<T>()));
			ImGui::PushID(ptr);
			PropertyDrawer<T>()(
				(typeof<T>::name.str() + "##" + name + " ##Inspector"),
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

	void EditorInspector::update()
	{
	}

	bool EditorInspector::draw()
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			EditorContent & c { ML_Editor.content() };

			/* * * * * * * * * * * * * * * * * * * * */

			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(
				("##Inspector##"_s + getTitle()).c_str(),
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_MenuBar
			);
			ImGui::PopStyleVar();
			if (ImGui::BeginMenuBar())
			{
				if (c.selected())
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.1f, 1.0f, 1.0f });
					ImGui::Text("[%s]", c.type_name().c_str());
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::Text("\'%s\'", c.item_name().c_str());
				}
				else
				{
					ImGui::Text("Nothing selected");
				}
				ImGui::EndMenuBar();
			}
			switch (Hash { c.type_name().data(), c.type_name().size() })
			{
			case typeof<Entity>::hash:	Layout::draw_item<Entity>(c.selected(), c.item_name()); break;
			case typeof<Font>::hash:	Layout::draw_item<Font>(c.selected(), c.item_name()); break;
			case typeof<Image>::hash:	Layout::draw_item<Image>(c.selected(), c.item_name()); break;
			case typeof<Material>::hash:Layout::draw_item<Material>(c.selected(), c.item_name()); break;
			case typeof<Model>::hash:	Layout::draw_item<Model>(c.selected(), c.item_name()); break;
			case typeof<Script>::hash:	Layout::draw_item<Script>(c.selected(), c.item_name()); break;
			case typeof<Shader>::hash:	Layout::draw_item<Shader>(c.selected(), c.item_name()); break;
			case typeof<Sound>::hash:	Layout::draw_item<Sound>(c.selected(), c.item_name()); break;
			case typeof<Sprite>::hash:	Layout::draw_item<Sprite>(c.selected(), c.item_name()); break;
			case typeof<Surface>::hash:	Layout::draw_item<Surface>(c.selected(), c.item_name()); break;
			case typeof<Texture>::hash:	Layout::draw_item<Texture>(c.selected(), c.item_name()); break;
			case typeof<Uniform>::hash:	Layout::draw_item<Uniform>(c.selected(), c.item_name()); break;
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}