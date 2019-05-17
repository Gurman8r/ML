#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector()
		: GUI_Window("Inspector")
	{
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Inspector::onEvent(const IEvent * value)
	{
	}

	bool Inspector::drawGui(bool * p_open)
	{
		return beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}