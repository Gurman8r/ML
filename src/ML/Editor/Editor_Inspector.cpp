#include <ML/Editor/Editor_Content.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/RenderTexture.hpp>
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
		inline static void draw_item(void * ptr, String const & name)
		{
			if (!ptr) return;
			ImGui::PushID(typeof<T>::name().data());
			ImGui::PushID(ML_ADDRESSOF(&ML_Engine.content().data<T>()));
			ImGui::PushID(ptr);
			PropertyDrawer<T>()(
				(typeof<T>::name().str() + "##" + name + " ##Inspector"),
				(T &)(*((T *)ptr))
			);
			ImGui::PopID();
			ImGui::PopID();
			ImGui::PopID();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Inspector::Editor_Inspector()
		: Editor_Widget { "Inspector", "Ctrl+Alt+I", ML_Engine.prefs().get_bool("Editor", "show_inspector", false) }
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Inspector::onEvent(Event const & value)
	{
		Editor_Widget::onEvent(value);

		switch (*value)
		{
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Editor.mainMenuBar().addMenu("View", [&]()
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::MenuItem(getTitle(), getHotkey(), openPtr());
				ImGui::PopID();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (Editor_Dockspace & d{ ev.dockspace }; d.isOpen())
			{
				d.dockWindow(getTitle(), d.getNode(d.RightUp));
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ev.getPress(KeyCode::I, { { 0, 1, 1, 0 } })) { toggleOpen(); }

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
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
			switch (Hash(c.type_name().data(), c.type_name().size()))
			{
			case typeof<Entity>::hash():	Layout::draw_item<Entity>(c.selected(), c.item_name()); break;
			case typeof<Font>::hash():	Layout::draw_item<Font>(c.selected(), c.item_name()); break;
			case typeof<Image>::hash():	Layout::draw_item<Image>(c.selected(), c.item_name()); break;
			case typeof<Material>::hash():Layout::draw_item<Material>(c.selected(), c.item_name()); break;
			case typeof<Model>::hash():	Layout::draw_item<Model>(c.selected(), c.item_name()); break;
			case typeof<Script>::hash():	Layout::draw_item<Script>(c.selected(), c.item_name()); break;
			case typeof<Shader>::hash():	Layout::draw_item<Shader>(c.selected(), c.item_name()); break;
			case typeof<Sound>::hash():	Layout::draw_item<Sound>(c.selected(), c.item_name()); break;
			case typeof<Sprite>::hash():	Layout::draw_item<Sprite>(c.selected(), c.item_name()); break;
			case typeof<RenderTexture>::hash():	Layout::draw_item<RenderTexture>(c.selected(), c.item_name()); break;
			case typeof<Texture>::hash():	Layout::draw_item<Texture>(c.selected(), c.item_name()); break;
			case typeof<Uniform>::hash():	Layout::draw_item<Uniform>(c.selected(), c.item_name()); break;
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}