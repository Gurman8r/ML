#include <ML/Editor/StyleLoader.hpp>
#include <ML/Core/File.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	static const HashMap<String, int32_t> StyleTable = 
	{
		{ "Alpha",					ImGuiStyleVar_Alpha },
		{ "WindowPadding",			ImGuiStyleVar_WindowPadding },
		{ "WindowRounding",			ImGuiStyleVar_WindowRounding },
		{ "WindowBorderSize",		ImGuiStyleVar_WindowBorderSize },
		{ "WindowMinSize",			ImGuiStyleVar_WindowMinSize },
		{ "WindowTitleAlign",		ImGuiStyleVar_WindowTitleAlign },
		{ "ChildRounding",			ImGuiStyleVar_ChildRounding },
		{ "ChildBorderSize",		ImGuiStyleVar_ChildBorderSize },
		{ "PopupRounding",			ImGuiStyleVar_PopupRounding },
		{ "PopupBorderSize",		ImGuiStyleVar_PopupBorderSize },
		{ "FramePadding",			ImGuiStyleVar_FramePadding },
		{ "FrameRounding",			ImGuiStyleVar_FrameRounding },
		{ "FrameBorderSize",		ImGuiStyleVar_FrameBorderSize },
		{ "ItemSpacing",			ImGuiStyleVar_ItemSpacing },
		{ "ItemInnerSpacing",		ImGuiStyleVar_ItemInnerSpacing },
		{ "IndentSpacing",			ImGuiStyleVar_IndentSpacing },
		{ "ScrollbarSize",			ImGuiStyleVar_ScrollbarSize },
		{ "ScrollbarRounding",		ImGuiStyleVar_ScrollbarRounding },
		{ "GrabMinSize",			ImGuiStyleVar_GrabMinSize },
		{ "GrabRounding",			ImGuiStyleVar_GrabRounding },
		{ "TabRounding",			ImGuiStyleVar_TabRounding },
		{ "ButtonTextAlign",		ImGuiStyleVar_ButtonTextAlign },
		{ "SelectableTextAlign",	ImGuiStyleVar_SelectableTextAlign },
	};

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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	StyleLoader::StyleLoader()
	{
	}

	StyleLoader::~StyleLoader()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool StyleLoader::loadFromFile(const String & filename)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseWrapped = [](const String & src, const char lhs, const char rhs, String & out)
		{
			size_t a;
			if ((a = src.find_first_of(lhs)) != String::npos)
			{
				size_t b;
				if ((b = src.find_last_of(rhs)) != String::npos)
				{
					if (a != b)
					{
						return (bool)(out = src.substr((a + 1), (b - a - 1)));
					}
				}
			}
			return (bool)(out = String());
		};

		/* * * * * * * * * * * * * * * * * * * * */

		static const bool log = true;

		/* * * * * * * * * * * * * * * * * * * * */

		if (auto file = std::ifstream(filename))
		{
			String line;
			while (std::getline(file, line))
			{
				if (line.empty() || (line.trim().front() == '#'))
					continue;

				// Tag
				String tag;
				if (parseWrapped(line, '[', ']', tag))
				{
					line.erase(0, tag.size() + 2);
					line.trim();

					if (log) cout << "[" << tag << "] ";

					size_t i;
					if ((i = line.trim().find('=')) != String::npos)
					{
						// Name
						const String name = String(line.substr(0, i)).trim();
						line = String(line.substr(i + 1, line.size() - i - 1)).trim();
						
						if (log) cout << "\'" << name << "\' ";

						// Colors
						/* * * * * * * * * * * * * * * * * * * * */
						if (tag == "Color")
						{
							HashMap<String, int32_t>::const_iterator it;
							if ((it = ColorTable.find(name)) != ColorTable.end())
							{
								// Values
								String value;
								if (parseWrapped(line, '{', '}', value))
								{
									SStream ss(value);
									ImVec4 c;
									ss >> c.x >> c.y >> c.z >> c.w;
									ImGui::GetStyle().Colors[it->second] = c;
									
									if (log) cout
										<< "{ "
										<< c.x << " " << c.y << " " << c.z << " " << c.w
										<< " }";
								}
							}
						}
						if (log) cout << endl;
					}
				}
			}
			file.close();
			return true;
		}
		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void StyleLoader::serialize(std::ostream & out) const
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}