#include <ML/Editor/EditorWindow.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorWindow::EditorWindow(EventSystem & eventSystem, CString title, bool startOpen)
		: IEventListener(eventSystem)
		, m_title		(title)
		, m_open		(startOpen)
		, m_good		(false)
		, m_flags		(ImGuiWindowFlags_None)
	{
	}

	EditorWindow::~EditorWindow()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool EditorWindow::beginDraw(int32_t flags)
	{
		return m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags));
	}

	bool EditorWindow::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t EditorWindow::getID() const { return ImGui::GetID(getTitle()); }

	/* * * * * * * * * * * * * * * * * * * * */
}