#include <ML/Editor/GUI_Window.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GUI_Window::GUI_Window(CString title)
		: GUI_Base()
		, m_title(title)
		, m_open (NULL)
		, m_flags(ImGuiWindowFlags_None)
	{
	}
	GUI_Window::~GUI_Window()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI_Window::beginDraw(bool * p_open, int32_t flags)
	{
		return goodCheck(ImGui::Begin(
			(m_title),
			(m_open = p_open),
			(m_flags = flags))
		);
	}

	bool GUI_Window::endDraw()
	{
		ImGui::End();
		return good();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t GUI_Window::getID() const
	{
		return ImGui::GetID(m_title);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}