#include <ML/Editor/EditorGui.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorGui::EditorGui(EventSystem & eventSystem, CString title)
		: EventListener	(eventSystem)
		, m_title		(title)
		, m_open		(true)
		, m_good		(false)
		, m_flags		(ImGuiWindowFlags_None)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorGui::beginDraw(int32_t flags)
	{
		return m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags));
	}

	bool EditorGui::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}