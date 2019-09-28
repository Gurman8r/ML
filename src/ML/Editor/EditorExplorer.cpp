#include <ML/Editor/EditorExplorer.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/SizeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr size_of MaxPreviewSize { 15_MB };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorExplorer::EditorExplorer(Editor & editor)
		: EditorWindow	{ editor, "Explorer", "Ctrl+Alt+E", false }
		, m_browser		{ }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorExplorer::onUpdate(const UpdateEvent & ev)
	{
		m_browser.update();
	}

	bool EditorExplorer::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_AlwaysAutoResize))
		{
			m_browser.render((String(getTitle()) + "##FileBrowser"), { 0 }, true, 0);
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}