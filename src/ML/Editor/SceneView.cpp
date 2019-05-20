#include <ML/Editor/SceneView.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SceneView::SceneView(Editor & editor, bool open)
		: BaseWidget("Scene", editor, open)
	{
	}

	SceneView::~SceneView()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool SceneView::drawGui(const GuiEvent & ev)
	{
		return beginDraw(&m_open, ImGuiWindowFlags_MenuBar);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneView::beginDraw(bool * p_open, int32_t flags)
	{
		if (BaseWidget::beginDraw(&m_open, flags))
		{
			ImGui::BeginChild("Viewport", { -1, -1 });
		}
		return good();
	}

	bool SceneView::endDraw()
	{
		if (good())
		{
			ImGui::EndChild();
		}
		return BaseWidget::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneView::updateTexture(Texture * texture)
	{
		if (good() && (texture && (*texture)))
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