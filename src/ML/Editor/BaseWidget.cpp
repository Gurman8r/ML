#include <ML/Editor/BaseWidget.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BaseWidget::BaseWidget(EventSystem & eventSystem, CString title)
		: m_eventSystem	(eventSystem)
		, m_good		(false)
		, m_title		(title)
		, m_flags		(ImGuiWindowFlags_None)
		, m_open		(NULL)
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
}