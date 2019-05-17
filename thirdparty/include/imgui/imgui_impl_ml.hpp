#ifndef _IMGUI_IMPL_ML_HPP_
#define _IMGUI_IMPL_ML_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Window/Window.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_EDITOR_API bool ImGui_ML_Init(ml::CString version, ml::Window * window, bool install_callbacks, ml::CString iniName);
ML_EDITOR_API void ImGui_ML_Shutdown();
ML_EDITOR_API void ImGui_ML_NewFrame();
ML_EDITOR_API void ImGui_ML_Render(void * draw_data);

/* * * * * * * * * * * * * * * * * * * * */

ML_EDITOR_API bool ImGui_ML_CreateFontsTexture();
ML_EDITOR_API void ImGui_ML_DestroyFontsTexture();
ML_EDITOR_API bool ImGui_ML_CreateDeviceObjects();
ML_EDITOR_API void ImGui_ML_DestroyDeviceObjects();

/* * * * * * * * * * * * * * * * * * * * */

ML_EDITOR_API void ImGui_ML_MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods);
ML_EDITOR_API void ImGui_ML_ScrollCallback(void * window, double xoffset, double yoffset);
ML_EDITOR_API void ImGui_ML_KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
ML_EDITOR_API void ImGui_ML_CharCallback(void * window, uint32_t c);

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_IMGUI_IMPL_ML_HPP_