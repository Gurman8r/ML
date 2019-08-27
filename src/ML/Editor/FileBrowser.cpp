#include <ML/Editor/FileBrowser.hpp>
#include <ML/Core/Bytes.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr Bytes MaxPreviewSize { 15_MB };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FileBrowser::FileBrowser()
		: m_label		{}
		, m_path		{}
		, m_dir			{}
		, m_type		{ '\0' }
		, m_index		{ 0 }
		, m_preview		{}
		, m_doubleClick { false }
	{
	}

	FileBrowser::~FileBrowser()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileBrowser::update()
	{
		const String cwd { ML_FS.getPath() };
		const bool shouldUpdate { 
			(!cwd.empty()) &&
			(m_doubleClick) ||
			(m_path.empty()) ||
			(m_path != cwd) ||
			(m_path && m_dir.empty())
		};
		if (shouldUpdate)
		{
			m_path = cwd;
			if (ML_FS.getDirContents(m_path, m_dir))
			{
				set_selected(T_Dir, 0);
			}
		}
	}

	void FileBrowser::render()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(2, ("FileBrowser##Columns##" + m_label).c_str(), true);

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::BeginChild(("FileBrowser##Content##" + m_label).c_str()))
		{
			m_doubleClick = false;
			for (const auto & pair : m_dir)
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
						((name + type + "##" + m_label).c_str()),
						((m_type == type) && (m_index == i)),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							(m_doubleClick = true); break;
						}
						else
						{
							set_selected(pair.first, (int32_t)i);
						}
					}
				}
				ImGui::PopStyleColor();
				ImGui::Separator();

				if (m_doubleClick) break;
			}
		}
		ImGui::EndChild();

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::BeginChild(("FileBrowser##Tabs##" + m_label).c_str()))
		{
			if (ImGui::BeginTabBar(("FileBrowser##Tabs##" + m_label).c_str()))
			{
				// File Preview
				if (ImGui::BeginTabItem(("Preview##FileBrowser##" + m_label).c_str()))
				{
					if (ImGui::BeginChild(
						("##Preview##Content##FileBrowser##" + m_label).c_str(),
						{ 0, 0 },
						true
					))
					{
						ImGui::TextUnformatted(&m_preview[0], &m_preview[m_preview.size()]);
						ImGui::EndChild();
					}
					ImGui::EndTabItem();
				}

				// File Details
				if (ImGui::BeginTabItem(("Details##FileBrowser##" + m_label).c_str()))
				{
					if (ImGui::BeginChild(
						("##Details##Content##FileBrowser##" + m_label).c_str(),
						{ 0, 0 },
						true
					))
					{
						const Bytes size { get_selected_size() };
						ImGui::Text("Name: %s", get_selected_name().c_str());
						ImGui::Text("Type: %s", get_selected_type().c_str());
						ImGui::Text("Size: %s", size.str().c_str());
						ImGui::EndChild();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void FileBrowser::render(const String & label, const vec2 & size, bool border, int32_t flags)
	{
		ImGui::PushID(ML_ADDRESSOF(this));
		ImGui::PushID(label.c_str());
		if (ImGui::BeginChild(
			(m_label = label).c_str(), { size[0], size[1] }, border, flags
		))
		{
			render();

			if ((m_doubleClick || (m_doubleClick = false)) && (m_type == T_Dir))
			{
				ML_FS.setPath(get_selected_name());
			}
		}
		ImGui::EndChild();
		ImGui::PopID();
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileBrowser::set_selected(char type, size_t index)
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
				m_preview = (String("File size of {0} exceeds preview limit of {1}.").format(
					Bytes(get_selected_size()),
					MaxPreviewSize
				));
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

	String FileBrowser::get_selected_name() const
	{
		const String * file;
		return ((file = get_selected()) ? (*file) : String());
	}

	String FileBrowser::get_selected_path() const
	{
		const String * file;
		return (file = get_selected()) ? pathTo(*file) : String(); }

	String FileBrowser::get_selected_type() const
	{
		switch (m_type)
		{
		case T_Reg	: return ML_FS.getFileType(get_selected_path());
		case T_Dir	: return String("Directory");
		case T_Lnk	: return String("Link");
		default		: return String("?");
		}
	}

	size_t FileBrowser::get_selected_size() const
	{
		return ML_FS.getFileSize(get_selected_path());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}