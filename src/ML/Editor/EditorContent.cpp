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

	template <
		class T, class TT = detail::decay_t<T>
	> static inline void draw_content(const String & label)
	{
		if (!ML_Content.data<TT>().empty() && ImGui::CollapsingHeader(label.c_str()))
		{
			ImGui::PushID(ML_ADDRESSOF(&ML_Content.data<TT>()));
			ImGui::BeginGroup();
			for (auto & pair : ML_Content.data<TT>())
			{
				if (pair.first.size() >= 2 && pair.first.substr(0, 2) == "##")
				{
					continue; // hidden items
				}

				ImGui::PushID(pair.first.c_str());
				if (ImGui::TreeNode((pair.first + "##" + PropertyDrawer<TT>::tag).c_str()))
				{
					if (pair.second)
					{
						ImGui::PushID(ML_ADDRESSOF(pair.second));
						PropertyDrawer<TT>()(pair.first, (T)*pair.second);
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
			draw_content<Entity		&>("Entities");
			draw_content<Font		&>("Fonts");
			draw_content<Image		&>("Images");
			draw_content<Material	&>("Materials");
			draw_content<Model		&>("Models");
			draw_content<Shader		&>("Shaders");
			draw_content<Script		&>("Scripts");
			draw_content<Sprite		&>("Sprites");
			draw_content<Surface	&>("Surfaces");
			draw_content<Texture	&>("Textures");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}