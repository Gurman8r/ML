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
			Util::get_vector,
			static_cast<ptr_t<void>>(&std::remove_cv_t<List<String> &>(arr)),
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

	int32_t ImGuiExt::Confirm(bool trigger, C_String label, bool * p_open, C_String message, int32_t flags)
	{
		int32_t retval{ 0 };
		ImGui::PushID(label);
		if (trigger)
		{
			ImGui::OpenPopup(label);
		}
		if (ImGui::BeginPopupModal(label, p_open, flags))
		{
			if (message) { ImGui::Text(message); }

			const bool submit{ ImGui::Button("Submit") };

			ImGui::SameLine();

			const bool cancel{ ImGui::Button("Cancel") };

			if (submit || cancel) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();

			retval = GetState(submit, cancel);
		}
		ImGui::PopID();
		return retval;
	}

	int32_t ImGuiExt::Confirm(const String & label, bool trigger, const String & message)
	{
		return Confirm(
			trigger, 
			(label + "##ImGuiExt##Confirm").c_str(),
			nullptr,
			message.c_str(), 
			ImGuiWindowFlags_AlwaysAutoResize
		);
	}

	void ImGuiExt::HelpMarker(const String & message)
	{
		ImGui::TextDisabled("(?)");
		ImGuiExt::Tooltip(message);
	}

	int32_t ImGuiExt::OpenFile(const String & label, String & path, const vec2 & size)
	{
		int32_t retval{ 0 };
		ImGui::PushID(label.c_str());
		if (ImGui::Button(label.c_str()))
		{
			ImGui::OpenPopup(("Open File##" + label).c_str());
		}
		if (ImGui::BeginPopupModal(
			("Open File##" + label).c_str(),
			nullptr,
			ImGuiWindowFlags_AlwaysAutoResize
		))
		{
			static FileBrowser browser {};
			browser.update();
			browser.render(("File Browser##" + label), size, true, ImGuiWindowFlags_MenuBar);

			const bool submit { ImGui::Button(("Open##" + label).c_str()) };

			ImGui::SameLine();

			const bool cancel{ ImGui::Button(("Cancel##" + label).c_str()) };

			if (submit)
			{
				retval = (path = browser.get_selected_path());
			}
			
			if (submit || cancel) { ImGui::CloseCurrentPopup(); }
			
			ImGui::EndPopup();

			retval = GetState(submit, cancel);
		}
		ImGui::PopID();
		return retval;
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