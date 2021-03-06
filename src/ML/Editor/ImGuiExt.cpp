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

	bool ImGuiExt::Combo(C_String label, int32_t * index, std::vector<String> const & arr, int32_t max_height)
	{
		ImGui::PushID(label);
		const bool retval { ImGui::Combo(
			label,
			index,
			Util::get_vector,
			static_cast<void *>(&std::remove_cv_t<std::vector<String> &>(arr)),
			static_cast<int32_t>(arr.size()),
			max_height
		) };
		ImGui::PopID();
		return retval;
	}

	bool ImGuiExt::Combo(C_String label, int32_t * index, C_String const * arr, int32_t max_height)
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

	int32_t ImGuiExt::Confirm(bool trigger, C_String label, bool * p_open, C_String message, C_String msgYes, C_String msgNo, int32_t flags)
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

			const bool submit{ ImGui::Button(msgYes) };

			ImGui::SameLine();

			const bool cancel{ ImGui::Button(msgNo) };

			if (submit || cancel) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();

			retval = GetState(submit, cancel);
		}
		ImGui::PopID();
		return retval;
	}

	int32_t ImGuiExt::Confirm(bool trigger, C_String label, bool * p_open, C_String message, int32_t flags)
	{
		return Confirm(
			trigger,
			label,
			p_open,
			message,
			"Submit",
			"Cancel",
			flags
		);
	}

	int32_t ImGuiExt::Confirm(String const & label, bool trigger, String const & message)
	{
		return Confirm(
			trigger, 
			(label + "##ImGuiExt##Confirm").c_str(),
			nullptr,
			message.c_str(), 
			ImGuiWindowFlags_AlwaysAutoResize
		);
	}

	void ImGuiExt::HelpMarker(String const & message)
	{
		ImGui::TextDisabled("(?)");
		ImGuiExt::Tooltip(message);
	}

	int32_t ImGuiExt::OpenFile(String const & label, String & path, vec2 const & size)
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

	void ImGuiExt::Tooltip(String const & message)
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

	bool ImGuiExt::DragMat2(C_String label, mat2 & v, float_t v_speed, float_t v_min, float_t v_max, C_String format)
	{
		ImGui::PushID(ML_ADDRESSOF(&v));
		ImGui::PushID(label);

		ImGui::PushID("Row1");
		const bool row1{ ImGui::DragFloat2(label, &v[0], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row2");
		const bool row2{ ImGui::DragFloat2(label, &v[2], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PopID();
		ImGui::PopID();

		return (row1 ? 1 : row2 ? 2 : 0);
	}

	bool ImGuiExt::DragMat3(C_String label, mat3 & v, float_t v_speed, float_t v_min, float_t v_max, C_String format)
	{
		ImGui::PushID(ML_ADDRESSOF(&v));
		ImGui::PushID(label);

		ImGui::PushID("Row1");
		const bool row1{ ImGui::DragFloat3(label, &v[0], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row2");
		const bool row2{ ImGui::DragFloat3(label, &v[3], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row3");
		const bool row3{ ImGui::DragFloat3(label, &v[6], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PopID();
		ImGui::PopID();

		return (row1 ? 1 : row2 ? 2 : row3 ? 3 : 0);
	}

	bool ImGuiExt::DragMat4(C_String label, mat4 & v, float_t v_speed, float_t v_min, float_t v_max, C_String format)
	{
		ImGui::PushID(ML_ADDRESSOF(&v));
		ImGui::PushID(label);

		ImGui::PushID("Row1");
		const bool row1{ ImGui::DragFloat4(label, &v[0], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row2");
		const bool row2{ ImGui::DragFloat4(label, &v[4], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row3");
		const bool row3{ ImGui::DragFloat4(label, &v[8], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PushID("Row4");
		const bool row4{ ImGui::DragFloat4(label, &v[12], v_speed, v_min, v_max, format) };
		ImGui::PopID();

		ImGui::PopID();
		ImGui::PopID();

		return (row1 ? 1 : row2 ? 2 : row3 ? 3 : row4 ? 4 : 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}