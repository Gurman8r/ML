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

	void EditorManual::update()
	{
	}

	bool EditorManual::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

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