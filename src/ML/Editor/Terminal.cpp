#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/ScriptEvents.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Terminal::Terminal(EventSystem & eventSystem)
		: EditorGui		(eventSystem, "Terminal")
		, m_coutBuf		(nullptr)
		, m_coutPtr		(nullptr)
		, m_coutStr		()
		, m_inputBuf	()
		, m_lines		()
		, m_scrollBottom()
		, m_history		()
		, m_historyPos	(-1)
		, m_autoFill	()
		, m_paused		(false)
	{
		this->clear();

		std::memset(m_inputBuf, 0, sizeof(m_inputBuf));

		m_autoFill.push_back("clear");
		m_autoFill.push_back("exit");
		m_autoFill.push_back("history");

		for (auto & pair : ML_Interpreter.commands())
		{
			m_autoFill.push_back(pair.first.c_str());
		}

		this->printf("# Using this feature may result in crashes or system instability.");
		this->printf("# Type \'help\' for a list of commands.");
	}
	
	Terminal::~Terminal()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Terminal::drawGui(const GuiEvent & ev)
	{
		if (m_coutBuf)
		{
			this->printss(m_coutStr);
		}

		if (beginDraw(ImGuiWindowFlags_MenuBar))
		{
			// Filter
			static ImGuiTextFilter filter("-wrn");
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
			filter.Draw("Filter (\"incl,-excl\")", 180);
			ImGui::PopStyleVar();

			// Clear Button
			ImGui::SameLine();
			if (ImGui::Button("Clear##Terminal"))
			{
				this->clear();
			}

			// Toggle Pause
			ImGui::SameLine();
			ImGui::Checkbox("Paused##Terminal", &m_paused);

			ImGui::Separator();

			// Text
			const float_t footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild(
				"ScrollingRegion",
				{ 0, -footer_height },
				false,
				ImGuiWindowFlags_HorizontalScrollbar
			);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4, 1 });
			
			enum Mode { LOG, WRN, ERR, SYS, MAX_MODE };

			static const ImVec4 colors[MAX_MODE]
			{
				{ 0.0f, 1.0f,  0.4f,  1.0f },	// LOG | green
				{ 1.0f, 1.0f,  0.4f,  1.0f },	// WRN | yellow
				{ 1.0f, 0.4f,  0.4f,  1.0f },	// ERR | red
				{ 1.0f, 0.78f, 0.58f, 1.0f },	// SYS | orange
			};

			static const String labels[MAX_MODE]
			{
				String("[ LOG ]"),
				String("[ WRN ]"),
				String("[ ERR ]"),
				String("# "),
			};

			auto check_label = [](const Mode mode, const String & str)
			{
				return 
					(mode < Mode::MAX_MODE) &&
					(!str.empty()) &&
					(str.size() >= labels[mode].size()) && 
					(str.substr(0, labels[mode].size()) == labels[mode]);
			};
			
			const ImVec4 & col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);

			for (const String & item : m_lines)
			{
				ImVec4 col = col_default_text;

				if (!filter.PassFilter(item.c_str())) continue;
				else if (check_label(LOG, item)) { col = colors[LOG]; }
				else if (check_label(WRN, item)) { col = colors[WRN]; }
				else if (check_label(ERR, item)) { col = colors[ERR]; }
				else if (check_label(SYS, item)) { col = colors[SYS]; }

				ImGui::PushStyleColor(ImGuiCol_Text, col);
				ImGui::TextUnformatted(item.c_str());
				ImGui::PopStyleColor();
			}

			if (m_scrollBottom)
			{
				ImGui::SetScrollHereY(1.0f);
			}

			m_scrollBottom = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();
			
			// Input
			bool reclaim_focus;
			if (reclaim_focus = ImGui::InputText(
				"Input", 
				m_inputBuf,
				IM_ARRAYSIZE(m_inputBuf),
				(
					ImGuiInputTextFlags_EnterReturnsTrue |
					ImGuiInputTextFlags_CallbackCompletion |
					ImGuiInputTextFlags_CallbackHistory
				),
				[](auto data) { return ((Terminal *)(data->UserData))->inputCallback(data); },
				static_cast<void *>(this))
			)
			{
				auto strtrim = [](char * str)
				{
					char * str_end = str + std::strlen(str);
					while (str_end > str && str_end[-1] == ' ') str_end--;
					*str_end = 0;
					return str;
				};

				char * s = strtrim(m_inputBuf);
				if (s[0])
				{
					execute(s);
				}
				std::strcpy(s, "");
			}
			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
			{
				// Auto focus previous widget
				ImGui::SetKeyboardFocusHere(-1);
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::clear()
	{
		m_lines.clear();
		m_scrollBottom = true;
	}

	void Terminal::execute(C_String value)
	{
		this->printf("# %s\n", value);

		// Insert into history.
		m_historyPos = -1;
		for (int32_t i = (int32_t)m_history.size() - 1; i >= 0; i--)
		{
			if (std::strcmp(m_history[i], value) == 0)
			{
				free(m_history[i]);
				m_history.erase(m_history.begin() + i);
				break;
			}
		}
		m_history.push_back(strdup(value));

		if (!std::strcmp(value, "clear"))
		{
			this->clear();
		}
		else if (!std::strcmp(value, "exit"))
		{
			eventSystem().fireEvent(WindowKillEvent());
		}
		else if (!std::strcmp(value, "history"))
		{
			for (C_String e : m_history)
			{
				cout << e << endl;
			}
		}
		else
		{
			eventSystem().fireEvent(CommandEvent(value));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::printf(C_String value, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, value);
		vsnprintf(buf, IM_ARRAYSIZE(buf), value, args);
		buf[IM_ARRAYSIZE(buf) - 1] = '\0';
		va_end(args);
		this->printl(buf);
	}

	void Terminal::printl(const String & value)
	{
		if (m_paused) return;
		m_lines.push_back(value);
		m_scrollBottom = true;
	}

	void Terminal::printss(SStream & value)
	{
		if (const String & text = value.str())
		{
			SStream sink(text);
			String	line;
			while (std::getline(sink, line))
			{
				this->printl(line);
			}
			value.str(String());
		}
	}

	bool Terminal::redirect(Ostream & value)
	{
		if (m_coutBuf && (m_coutPtr == &value))
		{
			// Release Ostream
			value.rdbuf(m_coutBuf);
			m_coutBuf = nullptr;
			m_coutPtr = nullptr;
			return true;
		}
		else if (m_coutBuf = value.rdbuf(m_coutStr.rdbuf()))
		{
			// Capture Ostream
			m_coutPtr = &value;
			return true;
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	int32_t Terminal::inputCallback(void * value)
	{
		ImGuiInputTextCallbackData * data;
		if (!(data = (ImGuiInputTextCallbackData *)(value))) 
		{ 
			return 0; 
		}

		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			C_String word_end = data->Buf + data->CursorPos;
			C_String word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
				{
					break;
				}
				word_start--;
			}

			// Build a list of candidates
			ImVector<C_String> candidates;
			for (int32_t i = 0; i < (int32_t)m_autoFill.size(); i++)
			{
				if (std::strncmp(m_autoFill[i], word_start, (int32_t)(word_end - word_start)) == 0)
				{
					candidates.push_back(m_autoFill[i]);
				}
			}

			if (candidates.Size == 0)
			{
				// No match
				this->printf(
					"No match for \"%.*s\"!\n", 
					(int32_t)(word_end - word_start),
					word_start
				);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
				int32_t match_len = (int32_t)(word_end - word_start);
				while (true)
				{
					int32_t c = 0;
					bool all_candidates_matches = true;
					for (int32_t i = 0; i < candidates.Size && all_candidates_matches; i++)
					{
						if (i == 0)
						{
							c = toupper(candidates[i][match_len]);
						}
						else if (c == 0 || c != toupper(candidates[i][match_len]))
						{
							all_candidates_matches = false;
						}
					}

					if (!all_candidates_matches)
					{
						break;
					}
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				this->printf("Possible matches:\n");
				
				for (int32_t i = 0; i < candidates.Size; i++)
				{
					this->printf("- %s\n", candidates[i]);
				}
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int32_t prev_history_pos = m_historyPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (m_historyPos == -1)
				{
					m_historyPos = (int32_t)m_history.size() - 1;
				}
				else if (m_historyPos > 0)
				{
					m_historyPos--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (m_historyPos != -1)
				{
					if (++m_historyPos >= (int32_t)m_history.size())
					{
						m_historyPos = -1;
					}
				}
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != m_historyPos)
			{
				C_String history_str = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}