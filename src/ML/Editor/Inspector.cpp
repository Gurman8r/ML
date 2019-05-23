#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector(EventSystem & eventSystem, bool startOpen)
		: EditorWindow(eventSystem, "Inspector", startOpen)
	{
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Inspector::drawGui(const GuiEvent & ev)
	{
		return beginDraw(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}