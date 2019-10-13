#include <ML/Editor/EditorManual.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorManual::EditorManual()
		: EditorWindow { "Manual", "Ctrl+Alt+M", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorManual::update(const UpdateEvent & ev)
	{
	}

	bool EditorManual::draw(const GuiEvent & ev)
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_Once);
		if (beginDraw(0))
		{
			ImGui::Columns(2);
			ImGui::Text("Categories"); ImGui::NextColumn();
			ImGui::Text("Content");
			ImGui::Columns(1);
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}