#include <ML/Editor/BaseWidget.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BaseWidget::BaseWidget(CString title)
		: m_good	(false)
		, m_title	(title)
		, m_open	(NULL)
		, m_flags	(ImGuiWindowFlags_None)
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