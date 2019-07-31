#include <ML/Editor/EditorImporter.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorImporter::EditorImporter(Editor & editor)
		: EditorGui { editor, "Importer" }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorImporter::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			ImGui::Text("%s (WORK IN PROGRESS)", getTitle());
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}