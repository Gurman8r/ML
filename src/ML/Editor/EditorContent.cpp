#include <ML/Editor/EditorContent.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
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
#include <ML/Engine/Script.hpp>
#include <ML/Editor/PropertyDrawer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> static inline void draw_content(const GuiEvent & ev, const String & label)
	{
		// Type Name
		static String ty_name;
		if (!ty_name)
		{
			ty_name = typeid(T).name();
			ty_name = alg::to_lower(ty_name.substr(ty_name.find_last_of(":") + 1));
		}

		// Empty
		if ((ML_Content.data<T>().empty())) return;
		
		// Can Create
		bool creatable { 
			std::is_same_v<T, Material> ||
			std::is_same_v<T, Script> ||
			std::is_same_v<T, Shader>
		};

		// Header
		const bool collapsing_header { ImGui::CollapsingHeader(label.c_str()) };

		// Popup
		static bool popup_open { false };
		static char name[32] = "";
		if (creatable && ImGui::BeginPopupContextItem())
		{
			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + ty_name).c_str());
			}

			ImGui::Text("Create %s:", ty_name.c_str());
			ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
			const bool submit { ImGui::Button("Submit") };
			if (submit) 
			{ 
				if (!ML_Content.get<T>(name))
				{
					if (ML_Content.create<T>(name))
					{
						Debug::log("Created {0}", name);
					}
				}
			}
			ImGui::SameLine();

			// Popup Closed
			if (submit || ImGui::Button("Cancel")) 
			{ 
				popup_open = false;
				std::strcpy(name, "new_asset");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (!collapsing_header) { return; }
		
		ImGui::PushID(ML_ADDRESSOF(&ML_Content.data<T>()));
		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<T>())
		{
			if (pair.first.size() >= 2 && pair.first.substr(0, 2) == "##")
			{
				continue; // hidden items
			}

			ImGui::PushID(pair.first.c_str());
			if (ImGui::TreeNode((pair.first + "##" + PropertyDrawer<T>::tag).c_str()))
			{
				if (pair.second)
				{
					ImGui::PushID(ML_ADDRESSOF(pair.second));
					PropertyDrawer<T>()(pair.first, (T &)*pair.second);
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::PopID();
	}

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
		if (beginDraw(ImGuiWindowFlags_None))
		{
			draw_content<Entity		>(ev, "Entities");
			draw_content<Font		>(ev, "Fonts");
			draw_content<Image		>(ev, "Images");
			draw_content<Material	>(ev, "Materials");
			draw_content<Model		>(ev, "Models");
			draw_content<Shader		>(ev, "Shaders");
			draw_content<Script		>(ev, "Scripts");
			draw_content<Sprite		>(ev, "Sprites");
			draw_content<Surface	>(ev, "Surfaces");
			draw_content<Texture	>(ev, "Textures");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}