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

	bool ImGuiExt::Combo(C_String label, int32_t * index, const List<String> & arr, int32_t max_height)
	{
		ImGui::PushID(label);
		const bool retval { ImGui::Combo(
			label,
			index,
			vector_getter,
			static_cast<void *>(&std::remove_cv_t<List<String> &>(arr)),
			static_cast<int32_t>(arr.size()),
			max_height
		) };
		ImGui::PopID();
		return retval;
	}

	bool ImGuiExt::Combo(C_String label, int32_t * index, const C_String * arr, int32_t max_height)
	{
		ImGui::PushID(label);
		const bool retval { ImGui::Combo(label, index, arr, max_height) };
		ImGui::PopID();
		return retval;
	}

	bool ImGuiExt::Combo(C_String label, int32_t * index, C_String items, int32_t max_height)
	{
		ImGui::PushID(label);
		const bool retval { ImGui::Combo(label, index, items, max_height) };
		ImGui::PopID();
		return retval;
	}

	int32_t ImGuiExt::Confirm(const String & label, bool trigger, const String & message)
	{
		int32_t retval { 0 };
		const String name { label + "##ConfirmSubmissionPopupModal" };
		ImGui::PushID(name.c_str());
		if (trigger)
		{
			ImGui::OpenPopup(name.c_str());
		}
		if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("%s", message.c_str());

			const bool submit { ImGui::Button(("Submit##" + name).c_str()) };
			
			ImGui::SameLine();
			
			const bool cancel { ImGui::Button(("Cancel##" + name).c_str()) };
			
			if (submit || cancel) { ImGui::CloseCurrentPopup(); }
			
			ImGui::EndPopup();

			retval = (submit ? 1 : (cancel ? -1 : 0));
		}
		ImGui::PopID();
		return retval;
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
			ImGui::OpenPopup(("Open File##" + label).c_str());
		}
		if (ImGui::BeginPopupModal(
			("Open File##" + label).c_str(),
			nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_MenuBar
		))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s", ML_FS.getPath().c_str());
				ImGui::EndMenuBar();
			}

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