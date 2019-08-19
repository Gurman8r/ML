#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	vec2 ImGuiExt::GetContentRegionAvail()
	{
		return { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
	}

	bool ImGuiExt::Combo(C_String label, int32_t * index, const List<String> & arr)
	{
		return ImGui::Combo(
			label,
			index,
			ImGuiExt::vector_getter,
			static_cast<void *>(&std::remove_cv_t<List<String> &>(arr)),
			static_cast<int32_t>(arr.size())
		);
	}

	bool ImGuiExt::Combo(C_String label, int32_t * index, const C_String * arr, int32_t size)
	{
		return ImGui::Combo(label, index, arr, size);
	}

	void ImGuiExt::HelpMarker(const String & message)
	{
		ImGui::TextDisabled("(?)");
		ImGuiExt::Tooltip(message);
	}

	void ImGuiExt::Tooltip(const String & message)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(message.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}