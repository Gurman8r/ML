#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	vec2 EditorUtility::getCursorPos()
	{
		return vec2 {
			ImGui::GetCursorPosX(),
			ImGui::GetCursorPosY()
		};
	}

	vec2 EditorUtility::getWindowPos()
	{
		return vec2 {
			ImGui::GetWindowSize().x,
			ImGui::GetWindowSize().y
		};
	}

	vec2 EditorUtility::getWindowSize()
	{
		return vec2 {
			ImGui::GetWindowSize().x,
			ImGui::GetWindowSize().y
		};
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorUtility::HelpMarker(CString desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool EditorUtility::BeginWindow(CString name, bool & open, const int32_t flags)
	{
		return ImGui::Begin(name, &open, flags);
	}

	void EditorUtility::EndWindow()
	{
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}