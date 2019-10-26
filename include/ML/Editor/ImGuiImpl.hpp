#ifndef _ML_IMGUI_IMPL_HPP_
#define _ML_IMGUI_IMPL_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Window/Window.hpp>

#define ML_ImGuiImpl ::ml::ImGuiImpl::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API ImGuiImpl final : public Singleton<ImGuiImpl>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct Singleton<ImGuiImpl>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ClientAPI { API_Unknown, API_OpenGL, API_Vulkan };

		Window *	g_Window;
		ClientAPI	g_ClientApi;
		float64_t   g_Time;
		bool        g_MousePressed[5];
		void *		g_MouseCursors[(size_t)Cursor::Shape::NUM_SHAPE];
		bool		g_WantUpdateMonitors;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		char        g_GlslVersion[32];
		uint32_t    g_FontTexture;
		uint32_t    g_ShaderHandle;
		uint32_t	g_VertHandle;
		uint32_t	g_FragHandle;
		int32_t     g_AttribTex;
		int32_t		g_AttribProjMtx;
		int32_t     g_AttribPosition;
		int32_t		g_AttribUV;
		int32_t		g_AttribColor;
		uint32_t	g_VboHandle;
		uint32_t	g_ElementsHandle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool Startup(C_String version, Window * window, bool install_callbacks, bool use_ini, bool use_log);
		bool Shutdown();
		void NewFrame();
		void Render(void * value);
		bool LoadStyle(const String & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImGuiImpl();
		~ImGuiImpl();

		bool CreateFontsTexture();
		void DestroyFontsTexture();

		bool CreateDeviceObjects();
		void DestroyDeviceObjects();

		bool CompileShader(uint32_t & obj, const C_String * vs, const C_String * fs);
		void HandleInput();

		static void MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods);
		static void ScrollCallback(void * window, float64_t xoffset, float64_t yoffset);
		static void KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void CharCallback(void * window, uint32_t c);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMGUI_IMPL_HPP_