#include <ML/Editor/SceneGui.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SceneGui::SceneGui(EventSystem & eventSystem)
		: EditorGui(eventSystem, "Scene")
	{
	}

	SceneGui::~SceneGui()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool SceneGui::drawGui(const GuiEvent & ev)
	{
		return beginDraw(ImGuiWindowFlags_MenuBar);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneGui::beginDraw(int32_t flags)
	{
		if (EditorGui::beginDraw(flags))
		{
			ImGui::BeginChild("Viewport", { -1, -1 });
		}
		return m_good;
	}

	bool SceneGui::endDraw()
	{
		if (m_good)
		{
			ImGui::EndChild();
		}
		return EditorGui::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneGui::updateTexture(Texture * texture)
	{
		if (m_good && (texture && (*texture)))
		{
			// Texture Size
			const ml::vec2 src = texture->size();

			// Window Size
			const ml::vec2 dst = ml::vec2 {
				ImGui::GetWindowSize().x,
				ImGui::GetWindowSize().y
			};

			auto scaleToFit = [](const ml::vec2 & src, const ml::vec2 & dst)
			{
				const ml::vec2
					hs = (dst[0] / src[0]),
					vs = (dst[1] / src[1]);
				return (src * (((hs) < (vs)) ? (hs) : (vs)));
			};

			const ml::vec2 scl = scaleToFit(src, dst);
			const ml::vec2 pos = ((dst - scl) * 0.5f);

			ImGui::SetCursorPos({ pos[0], pos[1] });

			ImGui::Image(texture->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}