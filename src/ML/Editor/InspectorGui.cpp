#include <ML/Editor/InspectorGui.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	InspectorGui::InspectorGui(EventSystem & eventSystem)
		: EditorGui(eventSystem, "Inspector")
	{
	}

	InspectorGui::~InspectorGui()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool InspectorGui::drawGui(const GuiEvent & ev)
	{
		return beginDraw(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}