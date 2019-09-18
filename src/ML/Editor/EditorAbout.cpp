#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorAbout::EditorAbout(Editor & editor)
		: EditorWindow { editor, "About", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorAbout::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorAbout::onGui(const GuiEvent & ev)
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_Once);

		if (!beginDraw(ImGuiWindowFlags_None)) { return endDraw(); }

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::CollapsingHeader("About MemeLib"))
		{
			ImGui::Text("MemeLib %s", ML_PROJECT_VER);
			ImGui::Text("By Melody Gurman");
			ImGui::Text("MemeLib is licensed under the MIT License, see LICENSE for more information.");
			ImGui::Separator();

			auto draw_def = ([](C_String label, C_String fmt, auto data, bool last = false)
			{
				ImGui::Text("define: %s", label); 
				ImGui::NextColumn();
				ImGui::Text(fmt, data);
				if (!last) ImGui::NextColumn();
			});

			ImGui::Columns(2);
			draw_def("ML_PROJECT_VER", "%s", ML_PROJECT_VER);
			draw_def("ML_PROJECT_URL", "%s", ML_PROJECT_URL);
			draw_def("ML_BUILD_DATE", "%s", ML_BUILD_DATE);
			draw_def("ML_BUILD_TIME", "%s", ML_BUILD_TIME);
			draw_def("ML_CC_NAME", "%s", ML_CC_NAME);
			draw_def("ML_CC_VER", "%u", ML_CC_VER);
			draw_def("ML_CONFIGURATION", "%s", ML_CONFIGURATION);
			draw_def("ML_CPLUSPLUS", "%u", ML_CPLUSPLUS);
			draw_def("ML_PLATFORM_TARGET", "%s", ML_PLATFORM_TARGET);
			draw_def("ML_SYSTEM_NAME", "%s", ML_SYSTEM_NAME, true);
			ImGui::Columns(1);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::CollapsingHeader("About Dear ImGui"))
		{
			ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
			ImGui::Text("By Omar Cornut and all dear imgui contributors.");
			ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
			ImGui::Separator();

			static bool show_config_info = false;
			ImGui::Checkbox("Config/Build Information", &show_config_info);
			if (show_config_info)
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGuiStyle& style = ImGui::GetStyle();

				bool copy_to_clipboard = ImGui::Button("Copy to clipboard");
				ImGui::BeginChildFrame(
					ImGui::GetID("cfginfos"), 
					ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18), 
					ImGuiWindowFlags_NoMove
				);
				if (copy_to_clipboard)
					ImGui::LogToClipboard();

				ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
				ImGui::Separator();
				ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
				ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);

#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
				ImGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
				ImGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
				ImGui::Text("define: _WIN32");
#endif
#ifdef _WIN64
				ImGui::Text("define: _WIN64");
#endif
#ifdef __linux__
				ImGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
				ImGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
				ImGui::Text("define: _MSC_VER=%d", ML_CC_MSC);
#endif
#ifdef __MINGW32__
				ImGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
				ImGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
				ImGui::Text("define: __GNUC__=%d", ML_CC_GNUC);
#endif
#ifdef __clang_version__
				ImGui::Text("define: __clang_version__=%s", __clang_version__);
#endif
#ifdef IMGUI_HAS_VIEWPORT
				ImGui::Text("define: IMGUI_HAS_VIEWPORT");
#endif
#ifdef IMGUI_HAS_DOCK
				ImGui::Text("define: IMGUI_HAS_DOCK");
#endif
				ImGui::Separator();
				ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
				ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
				ImGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        ImGui::Text(" NavEnableKeyboard");
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         ImGui::Text(" NavEnableGamepad");
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)     ImGui::Text(" NavEnableSetMousePos");
				if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)     ImGui::Text(" NavNoCaptureKeyboard");
				if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  ImGui::Text(" NoMouse");
				if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      ImGui::Text(" NoMouseCursorChange");
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)            ImGui::Text(" DockingEnable");
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)          ImGui::Text(" ViewportsEnable");
				if (io.ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)  ImGui::Text(" DpiEnableScaleViewports");
				if (io.ConfigFlags & ImGuiConfigFlags_DpiEnableScaleFonts)      ImGui::Text(" DpiEnableScaleFonts");
				if (io.MouseDrawCursor)                                         ImGui::Text("io.MouseDrawCursor");
				if (io.ConfigViewportsNoAutoMerge)                              ImGui::Text("io.ConfigViewportsNoAutoMerge");
				if (io.ConfigViewportsNoTaskBarIcon)                            ImGui::Text("io.ConfigViewportsNoTaskBarIcon");
				if (io.ConfigViewportsNoDecoration)                             ImGui::Text("io.ConfigViewportsNoDecoration");
				if (io.ConfigViewportsNoDefaultParent)                          ImGui::Text("io.ConfigViewportsNoDefaultParent");
				if (io.ConfigDockingNoSplit)                                    ImGui::Text("io.ConfigDockingNoSplit");
				if (io.ConfigDockingWithShift)                                  ImGui::Text("io.ConfigDockingWithShift");
				if (io.ConfigDockingTabBarOnSingleWindows)                      ImGui::Text("io.ConfigDockingTabBarOnSingleWindows");
				if (io.ConfigDockingTransparentPayload)                         ImGui::Text("io.ConfigDockingTransparentPayload");
				if (io.ConfigMacOSXBehaviors)                                   ImGui::Text("io.ConfigMacOSXBehaviors");
				if (io.ConfigInputTextCursorBlink)                              ImGui::Text("io.ConfigInputTextCursorBlink");
				if (io.ConfigWindowsResizeFromEdges)                            ImGui::Text("io.ConfigWindowsResizeFromEdges");
				if (io.ConfigWindowsMoveFromTitleBarOnly)                       ImGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
				ImGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
				if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             ImGui::Text(" HasGamepad");
				if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        ImGui::Text(" HasMouseCursors");
				if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         ImGui::Text(" HasSetMousePos");
				if (io.BackendFlags & ImGuiBackendFlags_PlatformHasViewports)   ImGui::Text(" PlatformHasViewports");
				if (io.BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport)ImGui::Text(" HasMouseHoveredViewport");
				if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)   ImGui::Text(" RendererHasVtxOffset");
				if (io.BackendFlags & ImGuiBackendFlags_RendererHasViewports)   ImGui::Text(" RendererHasViewports");
				ImGui::Separator();
				ImGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
				ImGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
				ImGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
				ImGui::Separator();
				ImGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
				ImGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
				ImGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
				ImGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
				ImGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
				ImGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
				ImGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

				if (copy_to_clipboard)
					ImGui::LogFinish();
				ImGui::EndChildFrame();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}