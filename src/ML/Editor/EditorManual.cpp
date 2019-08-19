#include <ML/Editor/EditorManual.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorManual::EditorManual(Editor & editor)
		: EditorForm { editor, "Manual", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorManual::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorManual::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			ImGui::Text("%s (WORK IN PROGRESS)", getTitle());
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}