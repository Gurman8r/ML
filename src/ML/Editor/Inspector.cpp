#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector(Editor & editor)
		: BaseWidget("Inspector", editor)
	{
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Inspector::drawGui(const GuiEvent * ev, bool * p_open)
	{
		return beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}