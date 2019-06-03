#ifndef _ML_IMGUI_STYLE_HPP_
#define _ML_IMGUI_STYLE_HPP_

#include <ML/Editor/ImGui.hpp>

// https://github.com/ocornut/imgui/issues/707

namespace ml
{
	struct ImGui_Style final
	{
		inline static void Default()
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
	};
}

#endif // !_ML_IMGUI_STYLE_HPP_