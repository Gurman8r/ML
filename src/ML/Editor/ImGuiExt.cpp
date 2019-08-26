#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/FileBrowser.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	vec2 ImGuiExt::GetContentRegionAvail()
	{
		return { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	bool ImGuiExt::OpenFile(const String & label, String & path, const vec2 & size)
	{
		bool good { false };
		ImGui::PushID(label.c_str());
		if (ImGui::Button(label.c_str()))
		{
			ImGui::OpenPopup(("Open File Dialog##" + label).c_str());
		}
		if (ImGui::BeginPopupModal(
			("Open File Dialog##" + label).c_str(),
			nullptr,
			ImGuiWindowFlags_AlwaysAutoResize
		))
		{
			static FileBrowser browser {};
			browser.update();
			browser.render(("File Browser##" + label), size, true);

			const bool submit { ImGui::Button(("Open##" + label).c_str()) };
			if (submit)
			{
				good = (path = browser.get_selected_path());
			}
			ImGui::SameLine();
			if (submit || ImGui::Button(("Cancel##" + label).c_str()))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
		return good;
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