#include <ML/Editor/EditorExplorer.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/Bytes.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr uint64_t MaxPreviewSize { 15_MB };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorExplorer::EditorExplorer(Editor & editor)
		: EditorGui		{ editor, "Explorer", false }
		, m_path		{ ML_FS.getPath() }
		, m_dir			{ }
		, m_type		{ T_Dir }
		, m_index		{ 0 }
		, m_preview		{  }
		, m_isDouble	{ false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorExplorer::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(0))
		{
			// Update Working Dir
			const String workingDir = ML_FS.getPath();

			if ((m_isDouble) ||
				(!m_path) ||
				(m_path != workingDir) ||
				(m_path && m_dir.empty()))
			{
				m_path = workingDir;
				if (ML_FS.getDirContents(m_path, m_dir))
				{
					set_selected(T_Dir, 0);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ImGui::Columns(2, "Directory Columns", true);

			draw_directory(); 
			
			ImGui::NextColumn();

			draw_file();

			ImGui::Columns(1);

			// Handle Double Clicks
			if (m_isDouble || (m_isDouble = false))
			{
				switch (m_type)
				{
				case T_Dir: 
					ML_FS.setPath(get_selected_name()); 
					break;
				}
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorExplorer::draw_directory()
	{
		ImGui::BeginChild("Directory View", { 0, 0 });
		{
			m_isDouble = false;

			for (const Pair<char, List<String>> & pair : m_dir)
			{
				const char & type = pair.first;
				const List<String> & list = pair.second;

				ImVec4 color;
				switch (type)
				{
				case T_Dir: color = ImColor(0.0f, 0.4f, 1.0f, 1.0f); break; // blue
				case T_Reg: color = ImColor(0.0f, 1.0f, 0.4f, 1.0f); break; // green
				case T_Lnk: color = ImColor(0.0f, 1.0f, 1.0f, 0.0f); break; // cyan
				default: 
					color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					break;
				}

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				for (size_t i = 0, imax = list.size(); i < imax; i++)
				{
					const String & name = list.at(i);

					if (ImGui::Selectable(
						((name + type).c_str()),
						((m_type == type) && (m_index == i)),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							(m_isDouble = true); break;
						}
						else
						{
							set_selected(pair.first, (int32_t)i);
						}
					}
				}
				ImGui::PopStyleColor();
				ImGui::Separator();

				if (m_isDouble) break;
			}
		}
		ImGui::EndChild();
	}

	void EditorExplorer::draw_file()
	{
		// Information
		ImGui::BeginChild("Tabs", { 0, 0 });
		{
			// Tabs
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				// Preview
				draw_file_preview();

				// Details
				draw_file_details();

				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();
	}

	void EditorExplorer::draw_file_preview()
	{
		if (ImGui::BeginTabItem("Preview"))
		{
			if (ImGui::BeginChild(
				"Content", 
				{ -1.f, -1.f }, 
				true, 
				ImGuiWindowFlags_AlwaysHorizontalScrollbar))
			{
				ImGui::TextUnformatted(&m_preview[0], &m_preview[m_preview.size()]);
				
				ImGui::EndChild();
			}
			ImGui::EndTabItem();
		}
	}

	void EditorExplorer::draw_file_details()
	{
		if (ImGui::BeginTabItem("Details"))
		{
			const Bytes size(get_selected_size());

			ImGui::Text("Name: %s", get_selected_name().c_str());
			ImGui::Text("Type: %s", get_selected_type().c_str());
			ImGui::Text("Size: %s", size.str().c_str());
			ImGui::EndTabItem();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorExplorer::set_selected(char type, size_t index)
	{
		m_type = type;
		m_index = index;

		switch (m_type)
		{
		case T_Reg:
		{
			if (get_selected_size() <= MaxPreviewSize)
			{
				if (ML_FS.getFileContents(get_selected_path(), m_preview))
				{
					m_preview.pop_back(); // remove last char, shows up as '?'
				}
				else
				{
					m_preview = get_selected_path();
				}
			}
			else
			{
				m_preview = String(
					"File size of {0} exceeds preview limit of {1}."
				).format(
					Bytes(get_selected_size()), 
					MaxPreviewSize);
			}
		}
		break;

		case T_Dir:
			if (!ML_FS.getDirContents(get_selected_path(), m_preview))
			{
				m_preview = get_selected_path();
			}
			break;

		default: 
			m_preview = get_selected_path();
			break;
		}
	}

	String EditorExplorer::get_selected_name() const
	{
		const String * file;
		return ((file = get_selected()) ? (*file) : String());
	}

	String EditorExplorer::get_selected_path() const
	{
		return pathTo(get_selected_name());
	}

	String EditorExplorer::get_selected_type() const
	{
		switch (m_type)
		{
		case T_Reg	: return ML_FS.getFileType(get_selected_path());
		case T_Dir	: return String("Directory");
		case T_Lnk	: return String("Link");
		default		: return String("?");
		}
	}

	size_t EditorExplorer::get_selected_size() const
	{
		return ML_FS.getFileSize(get_selected_path());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}