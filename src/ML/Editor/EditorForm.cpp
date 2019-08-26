#include <ML/Editor/EditorForm.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorForm::EditorForm(Editor & editor, C_String title, bool open)
		: m_editor	{ editor }
		, m_title	{ title }
		, m_open	{ open }
		, m_good	{ false }
		, m_flags	{ ImGuiWindowFlags_None }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorForm::beginDraw(int32_t flags)
	{
		ImGui::PushID(ML_ADDRESSOF(this));
		ImGui::PushID(getTitle());
		return m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags));
	}

	bool EditorForm::endDraw()
	{
		ImGui::End();
		ImGui::PopID();
		ImGui::PopID();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}