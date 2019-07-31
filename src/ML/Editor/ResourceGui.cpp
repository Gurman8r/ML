#include <ML/Editor/ResourceGui.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Editor/PropertyDrawer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T, class TT = detail::decay_t<T>
	> static inline void draw_content(const String & label)
	{
		if (!ImGui::CollapsingHeader(label.c_str()))
			return;

		ImGui::BeginGroup();
		for (auto & pair : ML_Content.data<TT>())
		{
			if (ImGui::TreeNode((pair.first + "##" + PropertyDrawer<TT>::tag).c_str()))
			{
				if (pair.second)
				{
					PropertyDrawer<TT>()(pair.first, (T)*pair.second);
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ResourceGui::ResourceGui(EventSystem & eventSystem)
		: EditorGui(eventSystem, "Resources")
	{
	}

	ResourceGui::~ResourceGui() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ResourceGui::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			draw_content<Entity		&>("Entities");
			draw_content<Font		&>("Fonts");
			draw_content<Image		&>("Images");
			draw_content<Material	&>("Materials");
			draw_content<Mesh		&>("Meshes");
			draw_content<Model		&>("Models");
			draw_content<Shader		&>("Shaders");
			draw_content<Script		&>("Scripts");
			draw_content<Sprite		&>("Sprites");
			draw_content<Surface	&>("Surfaces");
			draw_content<Texture	&>("Textures");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}