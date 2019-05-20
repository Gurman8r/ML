#include <ML/Editor/BaseWidget.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BaseWidget::BaseWidget(CString title, Editor & editor, bool open)
		: m_editor	(editor)
		, m_title	(title)
		, m_open	(open)
		, m_good	(false)
		, m_flags	(ImGuiWindowFlags_None)
	{
	}

	BaseWidget::~BaseWidget()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool BaseWidget::beginDraw(int32_t flags)
	{
		return m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags));
	}

	bool BaseWidget::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t BaseWidget::getID() const { return ImGui::GetID(m_title); }

	/* * * * * * * * * * * * * * * * * * * * */

	Editor		& BaseWidget::editor()		const { return m_editor; }
	Engine		& BaseWidget::engine()		const { return editor().engine(); }
	EventSystem & BaseWidget::eventSystem() const { return engine().eventSystem(); }
	Preferences & BaseWidget::prefs()		const { return engine().prefs(); }
	Resources	& BaseWidget::resources()	const { return engine().resources(); }

	/* * * * * * * * * * * * * * * * * * * * */
}