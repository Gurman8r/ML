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

	vec2 EditorUtility::getCursorScreenPos()
	{
		return {
			ImGui::GetCursorScreenPos().x,
			ImGui::GetCursorScreenPos().y
		};
	}

	vec2 EditorUtility::getMousePos()
	{
		return {
			ImGui::GetMousePos().x,
			ImGui::GetMousePos().y
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

	void EditorUtility::HelpMarker(const String & desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool EditorUtility::StringCombo(C_String label, int32_t & index, const List<String>& keys)
	{
		return ImGui::Combo(
			label,
			&index,
			vector_getter,
			static_cast<void *>(&std::remove_cv_t<List<String> &>(keys)),
			(int32_t)(keys.size())
		);
	}

	bool EditorUtility::BeginWindow(C_String name, bool & open, const int32_t flags)
	{
		return ImGui::Begin(name, &open, flags);
	}

	void EditorUtility::EndWindow()
	{
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}