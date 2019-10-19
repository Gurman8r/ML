#include <ML/Editor/EditorExplorer.hpp>
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

	EditorExplorer::EditorExplorer()
		: EditorWindow	{ "Explorer", "Ctrl+Alt+E", false }
		, m_browser		{ }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorExplorer::update()
	{
		m_browser.update();
	}

	bool EditorExplorer::draw()
	{
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