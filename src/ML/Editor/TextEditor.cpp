#include <ML/Editor/TextEditor.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	TextEditor::TextEditor(EventSystem & eventSystem)
		: BaseWidget(eventSystem, "Text Editor")
		, m_selected(-1)
	{
	}

	TextEditor::~TextEditor()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TextEditor::onEvent(const IEvent * value)
	{
	}

	bool TextEditor::drawGui(const GuiEvent * ev, bool * p_open)
	{
		if (beginDraw(p_open,
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_AlwaysHorizontalScrollbar |
			ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			draw_menu();
			draw_tabs();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TextEditor::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", NULL, false, true))
				{
					m_files.push_back(Document(
						String("New File ({0})").format(m_files.size()).c_str()
					));
				}

				if (ImGui::MenuItem("Save", NULL, false, true))
				{
					if (Document * doc = get_selected_document())
					{
						doc->dirty = false;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Text("( %d/%u )", (m_selected + 1), m_files.size());

			ImGui::EndMenuBar();
		}
	}

	void TextEditor::draw_tabs()
	{
		if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_Reorderable))
		{
			m_selected = -1;
			for (size_t i = 0; i < m_files.size(); i++)
			{
				if (!edit_document((int32_t)i))
				{
					m_files.erase(m_files.begin() + i);
					break;
				}
			}
			ImGui::EndTabBar();
		}
	}

	bool TextEditor::edit_document(int32_t index)
	{
		if (Document * doc = get_document((size_t)index))
		{
			ImGuiTabItemFlags flags = (doc->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);

			if (ImGui::BeginTabItem(doc->name, &doc->open, flags))
			{
				// Name
				/* * * * * * * * * * * * * * * * * * * * */
				Document::Name buf;
				if (edit_document_name(buf, doc))
				{
					strcpy(doc->name, buf);
				}

				ImGui::Separator();

				// Data
				/* * * * * * * * * * * * * * * * * * * * */
				if (edit_document_data(doc))
				{
					doc->dirty = true;
				}

				ImGui::Separator();

				// Info
				/* * * * * * * * * * * * * * * * * * * * */
				ImGui::Text("%u/%u", doc->sizeUsed(), doc->DataSize);
				ImGui::Text("Data:\n\"%s\"", String(doc->data).c_str());

				ImGui::EndTabItem();

				m_selected = index;
			}
			
			if (!doc->open)
			{
				return false; // Indicates file removed
			}
		}
		return true;
	}

	bool TextEditor::edit_document_name(char * buf, Document * doc)
	{
		strcpy(buf, doc->name);
		return ImGui::InputText(
			("Name"),
			(buf),
			(Document::NameSize),
			(ImGuiInputTextFlags_EnterReturnsTrue),
			(NULL),
			(NULL));
	}

	bool TextEditor::edit_document_data(Document * doc)
	{
		return ImGui::InputTextMultiline(
			("##Data"),
			(doc->data),
			(Document::DataSize),
			(ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16)),
			(ImGuiInputTextFlags_AllowTabInput),
			(NULL),
			(NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}