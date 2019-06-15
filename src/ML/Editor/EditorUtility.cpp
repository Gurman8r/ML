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