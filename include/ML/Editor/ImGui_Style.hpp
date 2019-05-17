#ifndef _ML_IMGUI_STYLE_HPP_
#define _ML_IMGUI_STYLE_HPP_

#include <ML/Editor/ImGui.hpp>

// https://github.com/ocornut/imgui/issues/707

namespace ImGui
{
	class StyleHelper final
	{
	public:
		inline static void Style0()
		{
			ImGui::StyleColorsDark();
			ImGui::GetStyle().FrameBorderSize = 1;
		}

		inline static void Style1()
		{
			ImGuiStyle & style = ImGui::GetStyle();
			ImVec4 * colors = style.Colors;

			/// 0 = FLAT APPEARENCE
			/// 1 = MORE "3D" LOOK
			int is3D = 0;

			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
			colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
			colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

			style.PopupRounding = 3;

			style.WindowPadding = ImVec2(4, 4);
			style.FramePadding = ImVec2(6, 4);
			style.ItemSpacing = ImVec2(6, 2);

			style.ScrollbarSize = 18;

			style.WindowBorderSize = 1;
			style.ChildBorderSize = 1;
			style.PopupBorderSize = 1;
			style.FrameBorderSize = (float)is3D;

			style.WindowRounding = 3;
			style.ChildRounding = 3;
			style.FrameRounding = 3;
			style.ScrollbarRounding = 2;
			style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
			style.TabBorderSize = (float)is3D;
			style.TabRounding = 3;

			colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
			colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
#endif
		}

		inline static void Style2()
		{
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;

			colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
			colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
			colors[ImGuiCol_WindowBg]               = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
			colors[ImGuiCol_ChildBg]                = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
			colors[ImGuiCol_PopupBg]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
			colors[ImGuiCol_Border]                 = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
			colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
			colors[ImGuiCol_FrameBg]                = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
			colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
			colors[ImGuiCol_FrameBgActive]          = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
			colors[ImGuiCol_TitleBg]                = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
			colors[ImGuiCol_TitleBgActive]          = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
			colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
			colors[ImGuiCol_MenuBarBg]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
			colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
			colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
			colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
			colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_CheckMark]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
			colors[ImGuiCol_SliderGrab]             = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
			colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_Button]                 = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
			colors[ImGuiCol_ButtonHovered]          = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
			colors[ImGuiCol_ButtonActive]           = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
			colors[ImGuiCol_Header]                 = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
			colors[ImGuiCol_HeaderHovered]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
			colors[ImGuiCol_HeaderActive]           = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
			colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
			colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
			colors[ImGuiCol_SeparatorActive]        = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
			colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
			colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_Tab]                    = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
			colors[ImGuiCol_TabHovered]             = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
			colors[ImGuiCol_TabActive]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
			colors[ImGuiCol_TabUnfocused]           = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
			colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
			colors[ImGuiCol_DockingPreview]         = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
			colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
			colors[ImGuiCol_PlotLines]              = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
			colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_PlotHistogram]          = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
			colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
			colors[ImGuiCol_DragDropTarget]         = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_NavHighlight]           = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
			colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
			colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

			style->ChildRounding = 4.0f;
			style->FrameBorderSize = 1.0f;
			style->FrameRounding = 2.0f;
			style->GrabMinSize = 7.0f;
			style->PopupRounding = 2.0f;
			style->ScrollbarRounding = 12.0f;
			style->ScrollbarSize = 13.0f;
			style->TabBorderSize = 1.0f;
			style->TabRounding = 0.0f;
			style->WindowRounding = 4.0f;
		}

		inline static void Style3()
		{
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;

			style->WindowRounding    = 2.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows
			style->ScrollbarRounding = 3.0f;             // Radius of grab corners rounding for scrollbar
			style->GrabRounding      = 2.0f;             // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
			style->AntiAliasedLines  = true;
			style->AntiAliasedFill   = true;
			style->WindowRounding    = 2;
			style->ChildRounding     = 2;
			style->ScrollbarSize     = 16;
			style->ScrollbarRounding = 3;
			style->GrabRounding      = 2;
			style->ItemSpacing.x     = 10;
			style->ItemSpacing.y     = 4;
			style->IndentSpacing     = 22;
			style->FramePadding.x    = 6;
			style->FramePadding.y    = 4;
			style->Alpha             = 1.0f;
			style->FrameRounding     = 3.0f;

			colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			colors[ImGuiCol_WindowBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
			//colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg]                = ImVec4(0.93f, 0.93f, 0.93f, 0.98f);
			colors[ImGuiCol_Border]                = ImVec4(0.71f, 0.71f, 0.71f, 0.08f);
			colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
			colors[ImGuiCol_FrameBg]               = ImVec4(0.71f, 0.71f, 0.71f, 0.55f);
			colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.94f, 0.94f, 0.94f, 0.55f);
			colors[ImGuiCol_FrameBgActive]         = ImVec4(0.71f, 0.78f, 0.69f, 0.98f);
			colors[ImGuiCol_TitleBg]               = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.82f, 0.78f, 0.78f, 0.51f);
			colors[ImGuiCol_TitleBgActive]         = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
			colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
			colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.61f);
			colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.90f, 0.90f, 0.90f, 0.30f);
			colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.92f, 0.92f, 0.78f);
			colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_CheckMark]             = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
			colors[ImGuiCol_SliderGrab]            = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
			colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Button]                = ImVec4(0.71f, 0.78f, 0.69f, 0.40f);
			colors[ImGuiCol_ButtonHovered]         = ImVec4(0.725f, 0.805f, 0.702f, 1.00f);
			colors[ImGuiCol_ButtonActive]          = ImVec4(0.793f, 0.900f, 0.836f, 1.00f);
			colors[ImGuiCol_Header]                = ImVec4(0.71f, 0.78f, 0.69f, 0.31f);
			colors[ImGuiCol_HeaderHovered]         = ImVec4(0.71f, 0.78f, 0.69f, 0.80f);
			colors[ImGuiCol_HeaderActive]          = ImVec4(0.71f, 0.78f, 0.69f, 1.00f);
			colors[ImGuiCol_Column]                = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			colors[ImGuiCol_ColumnHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
			colors[ImGuiCol_ColumnActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
			colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
			colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
			colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
			colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
			colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
			colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		}

		inline static void Style4()
		{
			ImGui::GetStyle().FrameBorderSize	= 1.0f;
			ImGui::GetStyle().FramePadding		= { 4.0f, 2.0f };
			ImGui::GetStyle().ItemSpacing		= { 8.0f, 2.0f };
			ImGui::GetStyle().WindowBorderSize	= 1.0f;
			ImGui::GetStyle().TabBorderSize		= 1.0f;
			ImGui::GetStyle().WindowRounding	= 1.0f;
			ImGui::GetStyle().ChildRounding		= 1.0f;
			ImGui::GetStyle().FrameRounding		= 1.0f;
			ImGui::GetStyle().ScrollbarRounding = 1.0f;
			ImGui::GetStyle().GrabRounding		= 1.0f;
			ImGui::GetStyle().TabRounding		= 1.0f;

			// Setup style
			ImGui::GetStyle().Colors[ImGuiCol_Text]					= { 1.00f, 1.00f, 1.00f, 0.95f };
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]			= { 0.50f, 0.50f, 0.50f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg]				= { 0.13f, 0.12f, 0.12f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg]				= { 1.00f, 1.00f, 1.00f, 0.00f };
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg]				= { 0.05f, 0.05f, 0.05f, 0.94f };
			ImGui::GetStyle().Colors[ImGuiCol_Border]				= { 0.53f, 0.53f, 0.53f, 0.46f };
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow]			= { 0.00f, 0.00f, 0.00f, 0.00f };
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg]				= { 0.00f, 0.00f, 0.00f, 0.85f };
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered]		= { 0.22f, 0.22f, 0.22f, 0.40f };
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive]		= { 0.16f, 0.16f, 0.16f, 0.53f };
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg]				= { 0.00f, 0.00f, 0.00f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive]		= { 0.00f, 0.00f, 0.00f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed]		= { 0.00f, 0.00f, 0.00f, 0.51f };
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]			= { 0.12f, 0.12f, 0.12f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg]			= { 0.02f, 0.02f, 0.02f, 0.53f };
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab]		= { 0.31f, 0.31f, 0.31f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.41f, 0.41f, 0.41f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive]	= { 0.48f, 0.48f, 0.48f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark]			= { 0.79f, 0.79f, 0.79f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab]			= { 0.48f, 0.47f, 0.47f, 0.91f };
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive]		= { 0.56f, 0.55f, 0.55f, 0.62f };
			ImGui::GetStyle().Colors[ImGuiCol_Button]				= { 0.50f, 0.50f, 0.50f, 0.63f };
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]		= { 0.67f, 0.67f, 0.68f, 0.63f };
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]			= { 0.26f, 0.26f, 0.26f, 0.63f };
			ImGui::GetStyle().Colors[ImGuiCol_Header]				= { 0.54f, 0.54f, 0.54f, 0.58f };
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]		= { 0.64f, 0.65f, 0.65f, 0.80f };
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]			= { 0.25f, 0.25f, 0.25f, 0.80f };
			ImGui::GetStyle().Colors[ImGuiCol_Separator]			= { 0.58f, 0.58f, 0.58f, 0.50f };
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered]		= { 0.81f, 0.81f, 0.81f, 0.64f };
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive]		= { 0.81f, 0.81f, 0.81f, 0.64f };
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip]			= { 0.87f, 0.87f, 0.87f, 0.53f };
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered]	= { 0.87f, 0.87f, 0.87f, 0.74f };
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive]		= { 0.87f, 0.87f, 0.87f, 0.74f };
			ImGui::GetStyle().Colors[ImGuiCol_Tab]					= { 0.01f, 0.01f, 0.01f, 0.86f };
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered]			= { 0.29f, 0.29f, 0.29f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TabActive]			= { 0.31f, 0.31f, 0.31f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused]			= { 0.02f, 0.02f, 0.02f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive]	= { 0.19f, 0.19f, 0.19f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_DockingPreview]		= { 0.38f, 0.48f, 0.60f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_DockingEmptyBg]		= { 0.20f, 0.20f, 0.20f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_PlotLines]			= { 0.61f, 0.61f, 0.61f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_PlotLinesHovered]		= { 0.68f, 0.68f, 0.68f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram]		= { 0.90f, 0.77f, 0.33f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_PlotHistogramHovered] = { 0.87f, 0.55f, 0.08f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg]		= { 0.47f, 0.60f, 0.76f, 0.47f };
			ImGui::GetStyle().Colors[ImGuiCol_DragDropTarget]		= { 0.58f, 0.58f, 0.58f, 0.90f };
			ImGui::GetStyle().Colors[ImGuiCol_NavHighlight]			= { 0.60f, 0.60f, 0.60f, 1.00f };
			ImGui::GetStyle().Colors[ImGuiCol_NavWindowingHighlight]= { 1.00f, 1.00f, 1.00f, 0.70f };
			ImGui::GetStyle().Colors[ImGuiCol_NavWindowingDimBg]	= { 0.80f, 0.80f, 0.80f, 0.20f };
			ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDimBg]		= { 0.80f, 0.80f, 0.80f, 0.35f };
		}

		inline static void Style5()
		{
			// cherry colors, 3 intensities
			#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
			#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
			#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
			// backgrounds (@todo: complete with BG_MED, BG_LOW)
			#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
			// text
			#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

			auto &style = ImGui::GetStyle();
			style.Colors[ImGuiCol_Text]                  = TEXT(0.78f);
			style.Colors[ImGuiCol_TextDisabled]          = TEXT(0.28f);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
			style.Colors[ImGuiCol_ChildWindowBg]         = BG( 0.58f);
			style.Colors[ImGuiCol_PopupBg]               = BG( 0.9f);
			style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg]               = BG( 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered]        = MED( 0.78f);
			style.Colors[ImGuiCol_FrameBgActive]         = MED( 1.00f);
			style.Colors[ImGuiCol_TitleBg]               = LOW( 1.00f);
			style.Colors[ImGuiCol_TitleBgActive]         = HI( 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = BG( 0.75f);
			style.Colors[ImGuiCol_MenuBarBg]             = BG( 0.47f);
			style.Colors[ImGuiCol_ScrollbarBg]           = BG( 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = MED( 0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = MED( 1.00f);
			style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_ButtonHovered]         = MED( 0.86f);
			style.Colors[ImGuiCol_ButtonActive]          = MED( 1.00f);
			style.Colors[ImGuiCol_Header]                = MED( 0.76f);
			style.Colors[ImGuiCol_HeaderHovered]         = MED( 0.86f);
			style.Colors[ImGuiCol_HeaderActive]          = HI( 1.00f);
			style.Colors[ImGuiCol_Column]                = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
			style.Colors[ImGuiCol_ColumnHovered]         = MED( 0.78f);
			style.Colors[ImGuiCol_ColumnActive]          = MED( 1.00f);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
			style.Colors[ImGuiCol_ResizeGripHovered]     = MED( 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive]      = MED( 1.00f);
			style.Colors[ImGuiCol_PlotLines]             = TEXT(0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered]      = MED( 1.00f);
			style.Colors[ImGuiCol_PlotHistogram]         = TEXT(0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = MED( 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg]        = MED( 0.43f);
			// [...]
			style.Colors[ImGuiCol_ModalWindowDarkening]  = BG( 0.73f);

			style.WindowPadding            = ImVec2(6, 4);
			style.WindowRounding           = 0.0f;
			style.FramePadding             = ImVec2(5, 2);
			style.FrameRounding            = 3.0f;
			style.ItemSpacing              = ImVec2(7, 1);
			style.ItemInnerSpacing         = ImVec2(1, 1);
			style.TouchExtraPadding        = ImVec2(0, 0);
			style.IndentSpacing            = 6.0f;
			style.ScrollbarSize            = 12.0f;
			style.ScrollbarRounding        = 16.0f;
			style.GrabMinSize              = 20.0f;
			style.GrabRounding             = 2.0f;

			style.WindowTitleAlign.x = 0.50f;

			style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
			style.FrameBorderSize = 0.0f;
		}

		inline static void Style6()
		{
			ImGuiStyle* style = &ImGui::GetStyle();
			style->WindowRounding = 5.3f;
			style->GrabRounding = style->FrameRounding = 2.3f;
			style->ScrollbarRounding = 5.0f;
			style->FrameBorderSize = 1.0f;
			style->ItemSpacing.y = 6.5f;

			style->Colors[ImGuiCol_Text]                  = {0.73333335f, 0.73333335f, 0.73333335f, 1.00f};
			style->Colors[ImGuiCol_TextDisabled]          = {0.34509805f, 0.34509805f, 0.34509805f, 1.00f};
			style->Colors[ImGuiCol_WindowBg]              = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
			style->Colors[ImGuiCol_ChildBg]               = {0.23529413f, 0.24705884f, 0.25490198f, 0.00f};
			style->Colors[ImGuiCol_PopupBg]               = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
			style->Colors[ImGuiCol_Border]                = {0.33333334f, 0.33333334f, 0.33333334f, 0.50f};
			style->Colors[ImGuiCol_BorderShadow]          = {0.15686275f, 0.15686275f, 0.15686275f, 0.00f};
			style->Colors[ImGuiCol_FrameBg]               = {0.16862746f, 0.16862746f, 0.16862746f, 0.54f};
			style->Colors[ImGuiCol_FrameBgHovered]        = {0.453125f, 0.67578125f, 0.99609375f, 0.67f};
			style->Colors[ImGuiCol_FrameBgActive]         = {0.47058827f, 0.47058827f, 0.47058827f, 0.67f};
			style->Colors[ImGuiCol_TitleBg]               = {0.04f, 0.04f, 0.04f, 1.00f};
			style->Colors[ImGuiCol_TitleBgCollapsed]      = {0.16f, 0.29f, 0.48f, 1.00f};
			style->Colors[ImGuiCol_TitleBgActive]         = {0.00f, 0.00f, 0.00f, 0.51f};
			style->Colors[ImGuiCol_MenuBarBg]             = {0.27058825f, 0.28627452f, 0.2901961f, 0.80f};
			style->Colors[ImGuiCol_ScrollbarBg]           = {0.27058825f, 0.28627452f, 0.2901961f, 0.60f};
			style->Colors[ImGuiCol_ScrollbarGrab]         = {0.21960786f, 0.30980393f, 0.41960788f, 0.51f};
			style->Colors[ImGuiCol_ScrollbarGrabHovered]  = {0.21960786f, 0.30980393f, 0.41960788f, 1.00f};
			style->Colors[ImGuiCol_ScrollbarGrabActive]   = {0.13725491f, 0.19215688f, 0.2627451f, 0.91f};
			// style->Colors[ImGuiCol_ComboBg]               = {0.1f, 0.1f, 0.1f, 0.99f};
			style->Colors[ImGuiCol_CheckMark]             = {0.90f, 0.90f, 0.90f, 0.83f};
			style->Colors[ImGuiCol_SliderGrab]            = {0.70f, 0.70f, 0.70f, 0.62f};
			style->Colors[ImGuiCol_SliderGrabActive]      = {0.30f, 0.30f, 0.30f, 0.84f};
			style->Colors[ImGuiCol_Button]                = {0.33333334f, 0.3529412f, 0.36078432f, 0.49f};
			style->Colors[ImGuiCol_ButtonHovered]         = {0.21960786f, 0.30980393f, 0.41960788f, 1.00f};
			style->Colors[ImGuiCol_ButtonActive]          = {0.13725491f, 0.19215688f, 0.2627451f, 1.00f};
			style->Colors[ImGuiCol_Header]                = {0.33333334f, 0.3529412f, 0.36078432f, 0.53f};
			style->Colors[ImGuiCol_HeaderHovered]         = {0.453125f, 0.67578125f, 0.99609375f, 0.67f};
			style->Colors[ImGuiCol_HeaderActive]          = {0.47058827f, 0.47058827f, 0.47058827f, 0.67f};
			style->Colors[ImGuiCol_Separator]             = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
			style->Colors[ImGuiCol_SeparatorHovered]      = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
			style->Colors[ImGuiCol_SeparatorActive]       = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
			style->Colors[ImGuiCol_ResizeGrip]            = {1.00f, 1.00f, 1.00f, 0.85f};
			style->Colors[ImGuiCol_ResizeGripHovered]     = {1.00f, 1.00f, 1.00f, 0.60f};
			style->Colors[ImGuiCol_ResizeGripActive]      = {1.00f, 1.00f, 1.00f, 0.90f};
			style->Colors[ImGuiCol_PlotLines]             = {0.61f, 0.61f, 0.61f, 1.00f};
			style->Colors[ImGuiCol_PlotLinesHovered]      = {1.00f, 0.43f, 0.35f, 1.00f};
			style->Colors[ImGuiCol_PlotHistogram]         = {0.90f, 0.70f, 0.00f, 1.00f};
			style->Colors[ImGuiCol_PlotHistogramHovered]  = {1.00f, 0.60f, 0.00f, 1.00f};
			style->Colors[ImGuiCol_TextSelectedBg]        = {0.18431373f, 0.39607847f, 0.79215693f, 0.90f};
		}
	};
}

#endif // !_ML_IMGUI_STYLE_HPP_