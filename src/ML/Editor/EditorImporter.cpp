#include <ML/Editor/EditorImporter.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Editor/ImGuiExt.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorImporter::EditorImporter(Editor & editor)
		: EditorForm { editor, "Importer", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorImporter::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorImporter::onGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_None))
		{
			const vec2 max_size { ImGuiExt::GetContentRegionAvail() };

			String path;
			if (open_file_dialog("Test Open File", nullptr, path))
			{
			}

			if (ImGui::BeginChild("ImporterContentArea",
				{ max_size[0], max_size[1] },
				true,
				ImGuiWindowFlags_None
			))
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::Button("Create Texture"))
				{
					ImGui::OpenPopup("Create Texture##Popup");
				}
				if (ImGui::BeginPopupModal(
					"Create Texture##Popup", 
					nullptr, 
					ImGuiWindowFlags_AlwaysAutoResize
				))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					const bool submit { ImGui::Button("Submit") };
					if (submit)
					{

					}
					ImGui::SameLine();
					if (submit || ImGui::Button("Close"))
					{
						ImGui::CloseCurrentPopup();
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndPopup();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}
			ImGui::EndChild();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorImporter::open_file_dialog(const String & label, bool * p_open, String & path)
	{
		if (ImGui::BeginPopupModal(
			label.c_str(),
			p_open,
			ImGuiWindowFlags_AlwaysAutoResize
		))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			const bool submit { ImGui::Button("Open") };
			if (submit)
			{

			}
			ImGui::SameLine();
			if (submit || ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::EndPopup();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}