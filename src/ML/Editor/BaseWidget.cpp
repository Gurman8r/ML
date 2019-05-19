#include <ML/Editor/BaseWidget.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BaseWidget::BaseWidget(CString title, Editor & editor)
		: m_editor	(editor)
		, m_title	(title)
		, m_good	(false)
		, m_flags	(ImGuiWindowFlags_None)
		, m_open	(NULL)
	{
	}

	BaseWidget::~BaseWidget()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool BaseWidget::beginDraw(bool * p_open, int32_t flags)
	{
		return goodCheck(ImGui::Begin(
			(m_title),
			(m_open = p_open),
			(m_flags = flags))
		);
	}

	bool BaseWidget::endDraw()
	{
		ImGui::End();
		return good();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t BaseWidget::getID() const
	{
		return ImGui::GetID(m_title);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Editor & BaseWidget::editor() const
	{
		return m_editor;
	}

	Engine & BaseWidget::engine() const
	{
		return editor().engine();
	}

	EventSystem & BaseWidget::eventSystem() const
	{
		return engine().eventSystem();
	}

	Preferences & BaseWidget::prefs() const
	{
		return engine().prefs();
	}

	Resources & BaseWidget::resources() const
	{
		return engine().resources();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}