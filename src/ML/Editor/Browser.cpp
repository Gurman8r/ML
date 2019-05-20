#include <ML/Editor/Browser.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const Bytes Browser::MaxPreviewSize(15_MB);

	/* * * * * * * * * * * * * * * * * * * * */

	Browser::Browser(Editor & editor, bool open)
		: BaseWidget("Browser", editor, open)
		, m_path	()
		, m_dir		()
		, m_type	(T_Dir)
		, m_index	(0)
		, m_preview	()
		, m_isDouble(false)
	{
	}

	Browser::~Browser()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Browser::drawGui(const DrawGuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_MenuBar))
		{
			const ml::String cwd = ML_FS.getWorkingDir();
			if (((!m_path) || (m_path != cwd)) && (m_path = cwd))
			{
				if (ML_FS.getDirContents(m_path, m_dir))
				{
					set_selected(T_Dir, 0);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s", m_path.c_str());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			draw_directory();

			ImGui::SameLine();

			draw_file();

			// Handle Double Clicks
			if (m_isDouble || (m_isDouble = false))
			{
				switch (m_type)
				{
				case T_Dir: 
					ML_FS.setWorkingDir(get_selected_name()); 
					break;
				}
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					OS::execute("open", get_selected_path());
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void Browser::draw_directory()
	{
		ImGui::BeginChild("Directory View", { 224, 0 }, true);
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

	void Browser::draw_file()
	{
		ImGui::BeginGroup();
		{
			// Information
			ImGui::BeginChild("Tabs");
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
		ImGui::EndGroup();
	}

	void Browser::draw_file_preview()
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

	void Browser::draw_file_details()
	{
		if (ImGui::BeginTabItem("Details"))
		{
			const Bytes size(get_selected_size());

			ImGui::Text("Name: %s", get_selected_name().c_str());
			ImGui::Text("Type: %s", get_selected_type().c_str());
			ImGui::Text("Size: %s", size.ToString().c_str());
			ImGui::EndTabItem();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::set_selected(char type, size_t index)
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

	String Browser::get_selected_name() const
	{
		const String * file;
		return ((file = get_selected()) ? (*file) : String());
	}

	String Browser::get_selected_path() const
	{
		return pathTo(get_selected_name());
	}

	String Browser::get_selected_type() const
	{
		switch (m_type)
		{
		case T_Reg	: return ML_FS.getFileType(get_selected_path());
		case T_Dir	: return String("Directory");
		case T_Lnk	: return String("Link");
		default		: return String("?");
		}
	}

	size_t Browser::get_selected_size() const
	{
		return ML_FS.getFileSize(get_selected_path());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}