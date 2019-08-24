#include <ML/Editor/EditorContent.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/ContentDatabase.hpp>
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

	struct EditorContent::Layout
	{
		template <
			class T
		> static inline void draw_content(const GuiEvent & ev, const String & label)
		{
			// Type Name
			static String type_name;
			if (!type_name && (type_name = typeof<T>().name()))
			{
				type_name = type_name.substr(type_name.find_last_of(":") + 1);
			}

			// Header
			if (!ImGui::CollapsingHeader(label.c_str())) { return; }

			// Empty
			if (ML_Content.data<T>().empty()) { return ImGui::Text("-"); }

			// Draw Elements
			ImGui::PushID(ML_ADDRESSOF(&ML_Content.data<T>()));
			ImGui::BeginGroup();
			{
				if (std::is_same_v<T, Material>)
				{
					Material * temp { nullptr };
					if (!PropertyDrawer<Material>()(label, (Material *&)temp)) { /* error */ }
				}
				if (std::is_same_v<T, Shader>)
				{
					Shader * temp { nullptr };
					if (!PropertyDrawer<Shader>()(label, (Shader *&)temp)) { /* error */ }
				}
				if (std::is_same_v<T, Uniform>)
				{
					Uniform * temp { nullptr };
					if (!PropertyDrawer<Uniform>()(label, (Uniform *&)temp, 1)) { /* error */ }
				}
			}
			for (auto & pair : ML_Content.data<T>())
			{
				if ((!pair.second) ||
					(pair.first.size() >= 2 && pair.first.substr(0, 2) == "##"))
				{
					continue; // hidden items
				}

				ImGui::PushID(pair.first.c_str());
				if (ImGui::TreeNode((pair.first + "##" + PropertyDrawer<T>::name().str()).c_str()))
				{
					ImGui::PushID(ML_ADDRESSOF(pair.second));
					PropertyDrawer<T>()(pair.first, (T &)*pair.second);
					ImGui::PopID();
					ImGui::TreePop();
				}
				ImGui::Separator();
				ImGui::PopID();
			}
			ImGui::EndGroup();
			ImGui::PopID();
		}
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
		if (beginDraw(ImGuiWindowFlags_None))
		{
			Layout::draw_content<Font		>(ev, "Fonts");
			Layout::draw_content<Image		>(ev, "Images");
			Layout::draw_content<Material	>(ev, "Materials");
			Layout::draw_content<Model		>(ev, "Models");
			Layout::draw_content<Shader		>(ev, "Shaders");
			Layout::draw_content<Script		>(ev, "Scripts");
			Layout::draw_content<Sprite		>(ev, "Sprites");
			Layout::draw_content<Surface	>(ev, "Surfaces");
			Layout::draw_content<Texture	>(ev, "Textures");
			Layout::draw_content<Uniform	>(ev, "Uniforms");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}