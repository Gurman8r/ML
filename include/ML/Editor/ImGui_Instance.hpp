#ifndef _ML_IMGUI_IMPL_HPP_
#define _ML_IMGUI_IMPL_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Window/Window.hpp>

#define ML_ImGui_Instance ml::ImGui_Instance::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ImGui_Instance final
		: public ISingleton<ImGui_Instance>
	{
		friend class ISingleton<ImGui_Instance>;

	private:
		ImGui_Instance();
		~ImGui_Instance();

	public:
		bool LoadStyle(const String & filename);
		bool Startup(CString version, Window * window, bool install_callbacks, CString ini);
		void Shutdown();
		void NewFrame();
		void Render(void * value);

	private:
		bool CreateFontsTexture();
		void DestroyFontsTexture();

	private:
		bool CreateDeviceObjects();
		void DestroyDeviceObjects();

	private:
		bool CompileShader(uint32_t & obj, const CString * vs, const CString * fs);
		void HandleInput();

	private:
		static void MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods);
		static void ScrollCallback(void * window, double xoffset, double yoffset);
		static void KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void CharCallback(void * window, uint32_t c);

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		bool m_Running;

		/* * * * * * * * * * * * * * * * * * * * */

		enum ClientAPI
		{
			API_Unknown,
			API_OpenGL,
			API_Vulkan
		};

		Window *	m_Window;
		ClientAPI	m_ClientApi;
		double      m_Time;
		bool        m_MousePressed[5];
		void *		m_MouseCursors[Cursor::NUM_SHAPE];

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