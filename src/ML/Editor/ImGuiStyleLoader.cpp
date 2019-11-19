#include <ML/Editor/ImGuiStyleLoader.hpp>
#include <ML/Core/File.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	static const HashMap<String, int32_t> ColorTable = 
	{
		{ "Text",					ImGuiCol_Text					},
		{ "TextDisabled",			ImGuiCol_TextDisabled			},
		{ "WindowBg",				ImGuiCol_WindowBg				},
		{ "ChildBg",				ImGuiCol_ChildBg				},
		{ "PopupBg",				ImGuiCol_PopupBg				},
		{ "Border",					ImGuiCol_Border					},
		{ "BorderShadow",			ImGuiCol_BorderShadow			},
		{ "FrameBg",				ImGuiCol_FrameBg				},
		{ "FrameBgHovered",			ImGuiCol_FrameBgHovered			},
		{ "FrameBgActive",			ImGuiCol_FrameBgActive			},
		{ "TitleBg",				ImGuiCol_TitleBg				},
		{ "TitleBgActive",			ImGuiCol_TitleBgActive			},
		{ "TitleBgCollapsed",		ImGuiCol_TitleBgCollapsed		},
		{ "MenuBarBg",				ImGuiCol_MenuBarBg				},
		{ "ScrollbarBg",			ImGuiCol_ScrollbarBg			},
		{ "ScrollbarGrab",			ImGuiCol_ScrollbarGrab			},
		{ "ScrollbarGrabHovered",	ImGuiCol_ScrollbarGrabHovered	},
		{ "ScrollbarGrabActive",	ImGuiCol_ScrollbarGrabActive	},
		{ "CheckMark",				ImGuiCol_CheckMark				},
		{ "SliderGrab",				ImGuiCol_SliderGrab				},
		{ "SliderGrabActive",		ImGuiCol_SliderGrabActive		},
		{ "Button",					ImGuiCol_Button					},
		{ "ButtonHovered",			ImGuiCol_ButtonHovered			},
		{ "ButtonActive",			ImGuiCol_ButtonActive			},
		{ "Header",					ImGuiCol_Header					},
		{ "HeaderHovered",			ImGuiCol_HeaderHovered			},
		{ "HeaderActive",			ImGuiCol_HeaderActive			},
		{ "Separator",				ImGuiCol_Separator				},
		{ "SeparatorHovered",		ImGuiCol_SeparatorHovered		},
		{ "SeparatorActive",		ImGuiCol_SeparatorActive		},
		{ "ResizeGrip",				ImGuiCol_ResizeGrip				},
		{ "ResizeGripHovered",		ImGuiCol_ResizeGripHovered		},
		{ "ResizeGripActive",		ImGuiCol_ResizeGripActive		},
		{ "Tab",					ImGuiCol_Tab					},
		{ "TabHovered",				ImGuiCol_TabHovered				},
		{ "TabActive",				ImGuiCol_TabActive				},
		{ "TabUnfocused",			ImGuiCol_TabUnfocused			},
		{ "TabUnfocusedActive",		ImGuiCol_TabUnfocusedActive		},
		{ "DockingPreview",			ImGuiCol_DockingPreview			},
		{ "DockingEmptyBg",			ImGuiCol_DockingEmptyBg			},
		{ "PlotLines",				ImGuiCol_PlotLines				},
		{ "PlotLinesHovered",		ImGuiCol_PlotLinesHovered		},
		{ "PlotHistogram",			ImGuiCol_PlotHistogram			},
		{ "PlotHistogramHovered",	ImGuiCol_PlotHistogramHovered	},
		{ "TextSelectedBg",			ImGuiCol_TextSelectedBg			},
		{ "DragDropTarget",			ImGuiCol_DragDropTarget			},
		{ "NavHighlight",			ImGuiCol_NavHighlight			},
		{ "NavWindowingHighlight",	ImGuiCol_NavWindowingHighlight	},
		{ "NavWindowingDimBg",		ImGuiCol_NavWindowingDimBg		},
		{ "ModalWindowDimBg",		ImGuiCol_ModalWindowDimBg		},
	};
}

namespace ml
{
	static inline bool readValue(String const & src, char lhs, char rhs, String & out)
	{
		size_t a;
		if ((a = src.find_first_of(lhs)) != String::npos)
		{
			size_t b;
			if ((b = src.find_last_of(rhs)) != String::npos)
			{
				if (a != b)
				{
					return (bool)(out = String(src
						.substr((a + 1), (b - a - 1)))
						.replace_all(",", "")
					);
				}
			}
		}
		return (bool)(out = String());
	}

	static inline auto readBool(String const & line)
	{
		bool out = false;
		String temp;
		if (readValue(line, '{', '}', temp))
		{
			SStream ss(temp);
			ss >> out;
		}
		return out;
	}

	static inline auto readFloat(String const & line)
	{
		float_t out = 0.0f;
		String temp;
		if (readValue(line, '{', '}', temp))
		{
			SStream ss(temp);
			ss >> out;
		}
		return out;
	}

	static inline auto readVec2(String const & line)
	{
		ImVec2 out { };
		String temp;
		if (readValue(line, '{', '}', temp))
		{
			SStream ss(temp);
			ss >> out.x >> out.y;
		}
		return out;
	}

	static inline auto readVec4(String const & line)
	{
		ImVec4 out { };
		String temp;
		if (readValue(line, '{', '}', temp))
		{
			SStream ss(temp);
			ss >> out.x >> out.y >> out.z >> out.w;
		}
		return out;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ImGuiStyleLoader::ImGuiStyleLoader()
		: m_good(false)
	{
	}

	ImGuiStyleLoader::ImGuiStyleLoader(String const & filename)
		: ImGuiStyleLoader()
	{
		loadFromFile(filename);
	}

	ImGuiStyleLoader::ImGuiStyleLoader(ImGuiStyleLoader && copy) noexcept
		: ImGuiStyleLoader {}
	{
		std::swap(m_good, copy.m_good);
	}

	ImGuiStyleLoader::~ImGuiStyleLoader() { }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ImGuiStyleLoader::loadFromFile(String const & filename)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		if (std::ifstream file { filename })
		{
			String line;
			while (std::getline(file, line))
			{
				if (line.empty() || (line.trim().front() == '#'))
					continue;

				// Tag
				String tag;
				if (readValue(line, '[', ']', tag))
				{
					line.erase(0, tag.size() + 2);
					line.trim();

					size_t i;
					if ((i = line.trim().find('=')) != String::npos)
					{
						// Name
						const String name = String(line.substr(0, i)).trim();
						line = String(line.substr(i + 1, line.size() - i - 1)).trim();

						// Colors
						/* * * * * * * * * * * * * * * * * * * * */
						if (tag == "ImGuiCol")
						{
							HashMap<String, int32_t>::const_iterator it;
							if ((it = ColorTable.find(name)) != ColorTable.end())
							{
								ImGui::GetStyle().Colors[it->second] = readVec4(line);
							}
						}
						else if (tag == "ImGuiStyle")
						{
							ImGuiStyle & s { ImGui::GetStyle() };
							/**/ if (name == "Alpha") { s.Alpha = readFloat(line); }
							else if (name == "WindowPadding") { s.WindowPadding = readVec2(line); }
							else if (name == "WindowRounding") { s.WindowRounding = readFloat(line); }
							else if (name == "WindowBorderSize") { s.WindowBorderSize = readFloat(line); }
							else if (name == "WindowMinSize") { s.WindowMinSize = readVec2(line); }
							else if (name == "WindowTitleAlign") { s.WindowTitleAlign = readVec2(line); }
							else if (name == "ChildRounding") { s.ChildRounding = readFloat(line); }
							else if (name == "ChildBorderSize") { s.ChildBorderSize = readFloat(line); }
							else if (name == "PopupRounding") { s.PopupRounding = readFloat(line); }
							else if (name == "PopupBorderSize") { s.PopupBorderSize = readFloat(line); }
							else if (name == "FramePadding") { s.FramePadding = readVec2(line); }
							else if (name == "FrameRounding") { s.FrameRounding = readFloat(line); }
							else if (name == "FrameBorderSize") { s.FrameBorderSize = readFloat(line); }
							else if (name == "ItemSpacing") { s.ItemInnerSpacing = readVec2(line); }
							else if (name == "ItemInnerSpacing") { s.ItemInnerSpacing = readVec2(line); }
							else if (name == "TouchExtraPadding") { s.TouchExtraPadding = readVec2(line); }
							else if (name == "IndentSpacing") { s.IndentSpacing = readFloat(line); }
							else if (name == "ColumnsMinSpacing") { s.ColumnsMinSpacing = readFloat(line); }
							else if (name == "ScrollbarSize") { s.ScrollbarSize = readFloat(line); }
							else if (name == "ScrollbarRounding") { s.ScrollbarRounding = readFloat(line); }
							else if (name == "GrabMinSize") { s.GrabMinSize = readFloat(line); }
							else if (name == "GrabRounding") { s.GrabRounding = readFloat(line); }
							else if (name == "TabRounding") { s.TabRounding = readFloat(line); }
							else if (name == "TabBorderSize") { s.TabBorderSize = readFloat(line); }
							else if (name == "ButtonTextAlign") { s.ButtonTextAlign = readVec2(line); }
							else if (name == "SelectableTextAlign") { s.SelectableTextAlign = readVec2(line); }
							else if (name == "DisplayWindowPadding") { s.DisplayWindowPadding = readVec2(line); }
							else if (name == "DisplaySafeAreaPadding") { s.DisplaySafeAreaPadding = readVec2(line); }
							else if (name == "MouseCursorScale") { s.MouseCursorScale = readFloat(line); }
							else if (name == "AntiAliasedLines") { s.AntiAliasedLines = readBool(line); }
							else if (name == "AntiAliasedFill") { s.AntiAliasedFill = readBool(line); }
							else if (name == "CurveTessellationTol") { s.CurveTessellationTol = readFloat(line); }
						}
					}
				}
			}
			file.close();
			return (m_good = true);
		}
		return (m_good = false);

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}