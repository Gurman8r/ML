#include <ML/Editor/Editor_Explorer.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/SizeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Explorer::Editor_Explorer()
		: EditorComponent	{ "Explorer", "Ctrl+Alt+E", false }
		, m_browser		{ }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Explorer::update()
	{
		m_browser.update();
	}

	bool Editor_Explorer::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

		if (beginDraw(ImGuiWindowFlags_AlwaysAutoResize))
		{
			m_browser.render(
				(getTitle() + "##FileBrowser"_s).c_str(), 
				{ 0, 0 }, 
				true, 
				ImGuiWindowFlags_MenuBar
			);
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}