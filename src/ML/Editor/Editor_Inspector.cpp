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

	struct Editor_Inspector::Layout
	{
		template <class T>
		inline static void draw_item(voidptr_t ptr, const String & name)
		{
			if (!ptr) return;
			ImGui::PushID(typeof<T>::name.data());
			ImGui::PushID(ML_ADDRESSOF(&ML_Engine.content().data<T>()));
			ImGui::PushID(ptr);
			PropertyDrawer<T>()(
				(typeof<T>::name.str() + "##" + name + " ##Inspector"),
				(T &)(*((ptr_t<T>)ptr))
			);
			ImGui::PopID();
			ImGui::PopID();
			ImGui::PopID();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Inspector::Editor_Inspector()
		: EditorComponent { "Inspector", "Ctrl+Alt+I", ML_Engine.prefs().get_bool("Editor", "show_inspector", false) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Inspector::update()
	{
	}

	bool Editor_Inspector::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

		if (beginDraw(ImGuiWindowFlags_None))
		{
			Editor_Content & c { ML_Editor.content() };

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
			switch (Hash{}(c.type_name().data(), c.type_name().size()))
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