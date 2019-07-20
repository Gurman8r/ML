#ifndef _ML_IMGUI_IMPL_HPP_
#define _ML_IMGUI_IMPL_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Window/Window.hpp>

#define ML_ImGui_Impl _ML ImGui_Impl::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ImGui_Impl final
		: public I_Singleton<ImGui_Impl>
	{
		friend struct I_Singleton<ImGui_Impl>;

	private:
		ImGui_Impl();
		~ImGui_Impl();

	public:
		bool Startup(C_String version, Window * window, bool install_callbacks, C_String ini);
		void Shutdown();
		void NewFrame();
		void Render(void * value);
		bool LoadStyle(const String & filename);

	private:
		bool CreateFontsTexture();
		void DestroyFontsTexture();

		bool CreateDeviceObjects();
		void DestroyDeviceObjects();

		bool CompileShader(uint32_t & obj, const C_String * vs, const C_String * fs);
		void HandleInput();

		template <
			class Ev
		> inline void fireEvent(const Ev & value)
		{
			if (m_Window) m_Window->eventSystem().fireEvent(value);
		}

		static void MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods);
		static void ScrollCallback(void * window, float64_t xoffset, float64_t yoffset);
		static void KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void CharCallback(void * window, uint32_t c);

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		bool m_Running;

		/* * * * * * * * * * * * * * * * * * * * */

		enum ClientAPI
		{
			API_Unknown, API_OpenGL, API_Vulkan
		};

		Window *	m_Window;
		ClientAPI	m_ClientApi;
		float64_t   m_Time;
		bool        m_MousePressed[5];
		void *		m_MouseCursors[(size_t)Cursor::Shape::NUM_SHAPE];

		/* * * * * * * * * * * * * * * * * * * * */

		char        m_GlslVersion[32];
		uint32_t    m_FontTexture;
		uint32_t    m_ShaderHandle;
		uint32_t	m_VertHandle;
		uint32_t	m_FragHandle;
		int32_t     m_AttribTex;
		int32_t		m_AttribProjMtx;
		int32_t     m_AttribPosition;
		int32_t		m_AttribUV;
		int32_t		m_AttribColor;
		uint32_t	m_VboHandle;
		uint32_t	m_ElementsHandle;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMGUI_IMPL_HPP_