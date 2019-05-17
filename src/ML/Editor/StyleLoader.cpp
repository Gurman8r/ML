#include <ML/Editor/StyleLoader.hpp>
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
	/* * * * * * * * * * * * * * * * * * * * */

	StyleLoader::StyleLoader()
	{
	}

	StyleLoader::~StyleLoader()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool StyleLoader::dispose()
	{
		return m_file.dispose();
	}

	bool StyleLoader::loadFromFile(const String & filename)
	{
		if (m_file.loadFromFile(filename))
		{
			m_file.ToStream() >> (*this);
		}
		return m_file;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void StyleLoader::serialize(std::ostream & out) const
	{
	}

	void StyleLoader::deserialize(std::istream & in)
	{
		String line;
		while (std::getline(in, line))
		{
			if (line.trim().front() != '#')
			{

			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}