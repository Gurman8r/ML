#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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

	bool EditorUtility::BeginWindow(CString name, bool * p_open, const int32_t flags)
	{
		return ImGui::Begin(name, p_open, flags);
	}

	void EditorUtility::EndWindow()
	{
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}