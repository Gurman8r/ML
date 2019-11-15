#include <ML/Editor/Editor_Manual.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Manual::Editor_Manual()
		: EditorComponent { "Manual", "Ctrl+Alt+M", ML_Engine.prefs().get_bool("Editor", "show_manual", false) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Manual::update()
	{
	}

	bool Editor_Manual::draw()
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