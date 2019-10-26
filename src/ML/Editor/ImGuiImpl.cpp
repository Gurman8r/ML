/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/ImGuiImpl.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiStyleLoader.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Shader.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	ImGuiImpl::ImGuiImpl()
		: g_Window				{ nullptr }
		, g_ClientApi			{ API_Unknown }
		, g_Time				{ 0.0 }
		, g_MousePressed		{ false, false, false, false, false }
		, g_MouseCursors		{ 0 }
		, g_WantUpdateMonitors	{ true }
		, g_GlslVersion			{ "" }
		, g_FontTexture			{ NULL }
		, g_ShaderHandle		{ NULL }
		, g_VertHandle			{ NULL }
		, g_FragHandle			{ NULL }
		, g_AttribTex			{ NULL }
		, g_AttribProjMtx		{ NULL }
		, g_AttribPosition		{ NULL }
		, g_AttribUV			{ NULL }
		, g_AttribColor			{ NULL }
		, g_VboHandle			{ NULL }
		, g_ElementsHandle		{ NULL }
	{
	}

	ImGuiImpl::~ImGuiImpl() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ImGuiImpl::Startup(C_String glsl_version, Window * window, bool install_callbacks, bool use_ini, bool use_log)
	{
		g_ClientApi = API_OpenGL;

		glsl_version = (!glsl_version ? "#version 130" : glsl_version);
		IM_ASSERT((int32_t)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersion));
		std::strcpy(g_GlslVersion, glsl_version);
		std::strcat(g_GlslVersion, "\n");

		g_Window = window;
		g_Time = 0.0;

		ImGuiIO & io { ImGui::GetIO() };

		// Backend Flags
		io.BackendPlatformName = "imgui_impl_glfw3";
		io.BackendRendererName = "imgui_impl_opengl3";
		//io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		//io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
#ifdef ML_SYSTEM_WINDOWS
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
#endif


		// Config Flags
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// File Names
		io.IniFilename = (use_ini ? "imgui.ini" : nullptr);
		io.LogFilename = (use_log ? "imgui.log" : nullptr);

		// Keyboard
		io.KeyMap[ImGuiKey_Tab]			= KeyCode::Tab;
		io.KeyMap[ImGuiKey_LeftArrow]	= KeyCode::Left;
		io.KeyMap[ImGuiKey_RightArrow]	= KeyCode::Right;
		io.KeyMap[ImGuiKey_UpArrow]		= KeyCode::Up;
		io.KeyMap[ImGuiKey_DownArrow]	= KeyCode::Down;
		io.KeyMap[ImGuiKey_PageUp]		= KeyCode::PageUp;
		io.KeyMap[ImGuiKey_PageDown]	= KeyCode::PageDown;
		io.KeyMap[ImGuiKey_Home]		= KeyCode::Home;
		io.KeyMap[ImGuiKey_End]			= KeyCode::End;
		io.KeyMap[ImGuiKey_Insert]		= KeyCode::Insert;
		io.KeyMap[ImGuiKey_Delete]		= KeyCode::Delete;
		io.KeyMap[ImGuiKey_Backspace]	= KeyCode::Backspace;
		io.KeyMap[ImGuiKey_Space]		= KeyCode::Space;
		io.KeyMap[ImGuiKey_Enter]		= KeyCode::Enter;
		io.KeyMap[ImGuiKey_Escape]		= KeyCode::Escape;
		io.KeyMap[ImGuiKey_A]			= KeyCode::A;
		io.KeyMap[ImGuiKey_C]			= KeyCode::C;
		io.KeyMap[ImGuiKey_V]			= KeyCode::V;
		io.KeyMap[ImGuiKey_X]			= KeyCode::X;
		io.KeyMap[ImGuiKey_Y]			= KeyCode::Y;
		io.KeyMap[ImGuiKey_Z]			= KeyCode::Z;

		// Clipboard
		io.SetClipboardTextFn = [](void * user_data, C_String text)
		{
			static_cast<Window *>(user_data)->setClipboardString(text);
		};
		io.GetClipboardTextFn = [](void * user_data)
		{
			return static_cast<const Window *>(user_data)->getClipboardString();
		};
		io.ClipboardUserData = g_Window;

		// Cursors
		g_MouseCursors[ImGuiMouseCursor_Arrow] = g_Window->createStandardCursor(Cursor::Shape::Arrow);
		g_MouseCursors[ImGuiMouseCursor_TextInput] = g_Window->createStandardCursor(Cursor::Shape::TextInput);
		g_MouseCursors[ImGuiMouseCursor_ResizeAll] = g_Window->createStandardCursor(Cursor::Shape::Arrow);   // FIXME: GLFW doesn't have this.
		g_MouseCursors[ImGuiMouseCursor_ResizeNS] = g_Window->createStandardCursor(Cursor::Shape::ResizeNS);
		g_MouseCursors[ImGuiMouseCursor_ResizeEW] = g_Window->createStandardCursor(Cursor::Shape::ResizeEW);
		g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = g_Window->createStandardCursor(Cursor::Shape::ResizeNESW);  // FIXME: GLFW doesn't have this.
		g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = g_Window->createStandardCursor(Cursor::Shape::ResizeNWSE);  // FIXME: GLFW doesn't have this.
		g_MouseCursors[ImGuiMouseCursor_Hand] = g_Window->createStandardCursor(Cursor::Shape::Hand);

		// Callbacks
		if (install_callbacks)
		{
			window->setMouseButtonCallback(this->MouseButtonCallback);
			window->setScrollCallback(this->ScrollCallback);
			window->setKeyCallback(this->KeyCallback);
			window->setCharCallback(this->CharCallback);
		}

		return true;
	}

	bool ImGuiImpl::Shutdown()
	{
		this->DestroyDeviceObjects();

		for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
		{
			g_Window->destroyCursor(g_MouseCursors[cursor_n]);
			g_MouseCursors[cursor_n] = NULL;
		}
		g_ClientApi = API_Unknown;

		return true;
	}

	void ImGuiImpl::NewFrame()
	{
		if (!g_FontTexture)
			this->CreateDeviceObjects();

		ImGuiIO & io = ImGui::GetIO();
		IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

		// Setup display size (every frame to accommodate for window resizing)
		vec2 size = vec2(g_Window->getSize());
		vec2 display = vec2(g_Window->getFrameSize());
		io.DisplaySize = ImVec2(size[0], size[1]);
		io.DisplayFramebufferScale = ImVec2(size[0] > 0 ? (display[0] / size[0]) : 0, size[1] > 0 ? (display[1] / size[1]) : 0);

		// Setup time step
		float64_t current_time = g_Window->getTime();
		io.DeltaTime = g_Time > 0.0 ? (float_t)(current_time - g_Time) : (float_t)(1.0f / 60.0f);
		g_Time = current_time;

		this->HandleInput();
	}

	void ImGuiImpl::Render(void * value)
	{
		ImDrawData * draw_data = static_cast<ImDrawData *>(value);

		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO & io = ImGui::GetIO();
		int32_t fb_width = (int32_t)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
		int32_t fb_height = (int32_t)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
	
		if (fb_width <= 0 || fb_height <= 0)
		{
			return;
		}
		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		// Backup GL state
		uint32_t last_active_texture = ML_GL.getInt(GL::ActiveTexture);
		ML_GL.activeTexture(GL::Texture0);
		int32_t last_program = ML_GL.getInt(GL::CurrentProgram);
		int32_t last_texture = ML_GL.getInt(GL::TextureBinding2D);
		int32_t last_sampler = ML_GL.getInt(GL::SamplerBinding);
		int32_t last_array_buffer = ML_GL.getInt(GL::ArrayBufferBinding);
		int32_t last_vertex_array = ML_GL.getInt(GL::VertexArrayBinding);

		int32_t last_polygon_mode[2]; 
		ML_GL.getIntv(GL::PolygonMode, last_polygon_mode);

		int32_t last_viewport[4]; 
		ML_GL.getIntv(GL::Viewport, last_viewport);

		int32_t last_scissor_box[4]; 
		ML_GL.getIntv(GL::ScissorBox, last_scissor_box);

		uint32_t last_blend_src_rgb = ML_GL.getInt(GL::BlendSourceRGB);
		uint32_t last_blend_dst_rgb = ML_GL.getInt(GL::BlendDestRGB);
		uint32_t last_blend_src_alpha = ML_GL.getInt(GL::BlendSourceAlpha);
		uint32_t last_blend_dst_alpha = ML_GL.getInt(GL::BlendDestAlpha);
		uint32_t last_blend_equation_rgb = ML_GL.getInt(GL::BlendEquationRGB);
		uint32_t last_blend_equation_alpha = ML_GL.getInt(GL::BlendEquationAlpha);

		bool last_enable_blend = ML_GL.isEnabled(GL::Blend);
		bool last_enable_cull_face = ML_GL.isEnabled(GL::CullFace);
		bool last_enable_depth_test = ML_GL.isEnabled(GL::DepthTest);
		bool last_enable_scissor_test = ML_GL.isEnabled(GL::ScissorTest);
		bool clip_origin_lower_left = true;

		uint32_t last_clip_origin = ML_GL.getInt(GL::ClipOrigin); // Support for GL 4.5's glClipControl(GL::UpperLeft)
		if (last_clip_origin == GL::UpperLeft)
		{
			clip_origin_lower_left = false;
		}

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
		ML_GL.enable(GL::Blend);
		ML_GL.blendEquation(GL::FuncAdd);
		ML_GL.blendFunc(GL::SrcAlpha, GL::OneMinusSrcAlpha);
		ML_GL.disable(GL::CullFace);
		ML_GL.disable(GL::DepthTest);
		ML_GL.enable(GL::ScissorTest);
		ML_GL.polygonMode(GL::FrontAndBack, GL::Fill);

		// Setup viewport, orthographic projection matrix
		// Our visible imgui space lies from draw_data->DisplayPos (top left) 
		// to draw_data->DisplayPos+data_data->DisplaySize (bottom right). 
		// DisplayMin is typically (0,0) for single viewport apps.
		ML_GL.viewport(0, 0, (int32_t)fb_width, (int32_t)fb_height);
		float_t L = draw_data->DisplayPos.x;
		float_t R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		float_t T = draw_data->DisplayPos.y;
		float_t B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

		const float_t m00 = 2.0f / (R - L);
		const float_t m05 = 2.0f / (T - B);
		const float_t m12 = (R + L) / (L - R);
		const float_t m13 = (T + B) / (B - T);
		const float_t ortho_projection[4][4] =
		{
			{	m00,	0.0f,	0.0f,	0.0f },
			{	0.0f,	m05,	0.0f,	0.0f },
			{	0.0f,	0.0f,	-1.0f,	0.0f },
			{	m12,	m13,	0.0f,   1.0f },
		};
		ML_GL.useShader(g_ShaderHandle);
		ML_GL.uniform1i(g_AttribTex, 0);
		ML_GL.uniformMatrix4fv(g_AttribProjMtx, 1, false, &ortho_projection[0][0]);
		ML_GL.bindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
		// Recreate the VAO every time
		// (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
	
		uint32_t vao_handle = ML_GL.genVertexArrays(1);
		ML_GL.bindVertexArray(vao_handle);
		ML_GL.bindBuffer(GL::ArrayBuffer, g_VboHandle);
		ML_GL.enableVertexAttribArray(g_AttribPosition);
		ML_GL.enableVertexAttribArray(g_AttribUV);
		ML_GL.enableVertexAttribArray(g_AttribColor);
		ML_GL.vertexAttribPointer(g_AttribPosition, 2, GL::Float, false, sizeof(ImDrawVert), (void *)IM_OFFSETOF(ImDrawVert, pos));
		ML_GL.vertexAttribPointer(g_AttribUV, 2, GL::Float, false, sizeof(ImDrawVert), (void *)IM_OFFSETOF(ImDrawVert, uv));
		ML_GL.vertexAttribPointer(g_AttribColor, 4, GL::UnsignedByte, true, sizeof(ImDrawVert), (void *)IM_OFFSETOF(ImDrawVert, col));

		// Draw
		ImVec2 pos = draw_data->DisplayPos;
		for (int32_t n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList * cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx * idx_buffer_offset = 0;

			ML_GL.bindBuffer(GL::ArrayBuffer, g_VboHandle);
			ML_GL.bufferData(
				GL::ArrayBuffer, 
				(int32_t)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), 
				(const void *)cmd_list->VtxBuffer.Data, 
				GL::StreamDraw);

			ML_GL.bindBuffer(GL::ElementArrayBuffer, g_ElementsHandle);
			ML_GL.bufferData(
				GL::ElementArrayBuffer,
				(int32_t)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), 
				(const void *)cmd_list->IdxBuffer.Data,
				GL::StreamDraw);

			for (int32_t cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd * pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
					if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
					{
						// Apply scissor/clipping rectangle
						if (clip_origin_lower_left)
						{
							ML_GL.scissor(
								(int32_t)clip_rect.x,
								(int32_t)(fb_height - clip_rect.w),
								(int32_t)(clip_rect.z - clip_rect.x),
								(int32_t)(clip_rect.w - clip_rect.y));
						}
						else
						{
							// Support for GL 4.5's glClipControl(GL::UpperLeft)
							ML_GL.scissor(
								(int32_t)clip_rect.x, 
								(int32_t)clip_rect.y,
								(int32_t)clip_rect.z,
								(int32_t)clip_rect.w);
						}

						// Bind texture, Draw
						ML_GL.bindTexture(GL::Texture2D, (uint32_t)(intptr_t)pcmd->TextureId);
						ML_GL.drawElements(
							GL::Triangles,
							(int32_t)pcmd->ElemCount,
							sizeof(ImDrawIdx) == 2 ? GL::UnsignedShort : GL::UnsignedInt,
							idx_buffer_offset);
					}
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}
		ML_GL.deleteVertexArrays(1, &vao_handle);

		// Restore modified GL state
		ML_GL.useShader(last_program);
		ML_GL.bindTexture(GL::Texture2D, last_texture);
		ML_GL.bindSampler(0, last_sampler);
		ML_GL.activeTexture(last_active_texture);
		ML_GL.bindVertexArray(last_vertex_array);
		ML_GL.bindBuffer(GL::ArrayBuffer, last_array_buffer);
		ML_GL.blendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		ML_GL.blendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	
		if (last_enable_blend) ML_GL.enable(GL::Blend); else ML_GL.disable(GL::Blend);
		if (last_enable_cull_face) ML_GL.enable(GL::CullFace); else ML_GL.disable(GL::CullFace);
		if (last_enable_depth_test) ML_GL.enable(GL::DepthTest); else ML_GL.disable(GL::DepthTest);
		if (last_enable_scissor_test) ML_GL.enable(GL::ScissorTest); else ML_GL.disable(GL::ScissorTest);
	
		ML_GL.polygonMode(GL::FrontAndBack, (uint32_t)last_polygon_mode[0]);
		ML_GL.viewport(last_viewport[0], last_viewport[1], (int32_t)last_viewport[2], (int32_t)last_viewport[3]);
		ML_GL.scissor(last_scissor_box[0], last_scissor_box[1], (int32_t)last_scissor_box[2], (int32_t)last_scissor_box[3]);
	}

	bool ImGuiImpl::LoadStyle(const String & filename)
	{
		return ImGuiStyleLoader { filename };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ImGuiImpl::CreateFontsTexture()
	{
		// Build texture atlas
		ImGuiIO & io = ImGui::GetIO();
		uint8_t* pixels;
		int32_t width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

		// Upload texture to graphics system
		int32_t last_texture = ML_GL.getInt(GL::TextureBinding2D);
		g_FontTexture = ML_GL.genTexture();
		ML_GL.bindTexture(GL::Texture2D, g_FontTexture);
		ML_GL.texParameter(GL::Texture2D, GL::TexMinFilter, GL::Linear);
		ML_GL.texParameter(GL::Texture2D, GL::TexMagFilter, GL::Linear);
		ML_GL.pixelStore(GL::UnpackRowLength, 0);
		ML_GL.texImage2D(GL::Texture2D, 0, GL::RGBA, width, height, 0, GL::RGBA, GL::UnsignedByte, pixels);

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

		// Restore state
		ML_GL.bindTexture(GL::Texture2D, last_texture);

		return true;
	}

	void ImGuiImpl::DestroyFontsTexture()
	{
		if (g_FontTexture)
		{
			ImGuiIO & io = ImGui::GetIO();
			ML_GL.deleteTextures(1, &g_FontTexture);
			io.Fonts->TexID = 0;
			g_FontTexture = 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ImGuiImpl::CreateDeviceObjects()
	{
		// Backup GL state
		int32_t last_texture = ML_GL.getInt(GL::TextureBinding2D);
		int32_t last_array_buffer = ML_GL.getInt(GL::ArrayBufferBinding);
		int32_t last_vertex_array = ML_GL.getInt(GL::VertexArrayBinding);

		// Parse GLSL version string
		int32_t glsl_version = 130;
		sscanf(g_GlslVersion, "#version %d", &glsl_version);

		static const C_String vertex_shader_glsl_120 =
			"uniform mat4 ProjMtx;\n"
			"attribute vec2 Position;\n"
			"attribute vec2 UV;\n"
			"attribute vec4 Color;\n"
			"varying vec2 Frag_UV;\n"
			"varying vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		static const C_String vertex_shader_glsl_130 =
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		static const C_String vertex_shader_glsl_300_es =
			"precision mediump float_t;\n"
			"layout (location = 0) in vec2 Position;\n"
			"layout (location = 1) in vec2 UV;\n"
			"layout (location = 2) in vec4 Color;\n"
			"uniform mat4 ProjMtx;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		static const C_String vertex_shader_glsl_410_core =
			"layout (location = 0) in vec2 Position;\n"
			"layout (location = 1) in vec2 UV;\n"
			"layout (location = 2) in vec4 Color;\n"
			"uniform mat4 ProjMtx;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		static const C_String fragment_shader_glsl_120 =
			"#ifdef GL_ES\n"
			"    precision mediump float_t;\n"
			"#endif\n"
			"uniform sampler2D Texture;\n"
			"varying vec2 Frag_UV;\n"
			"varying vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
			"}\n";

		static const C_String fragment_shader_glsl_130 =
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		static const C_String fragment_shader_glsl_300_es =
			"precision mediump float_t;\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"layout (location = 0) out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		static const C_String fragment_shader_glsl_410_core =
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"uniform sampler2D Texture;\n"
			"layout (location = 0) out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		// Select shaders matching our GLSL versions
		C_String vertex_shader = NULL;
		C_String fragment_shader = NULL;
		if (glsl_version < 130)
		{
			vertex_shader = vertex_shader_glsl_120;
			fragment_shader = fragment_shader_glsl_120;
		}
		else if (glsl_version == 410)
		{
			vertex_shader = vertex_shader_glsl_410_core;
			fragment_shader = fragment_shader_glsl_410_core;
		}
		else if (glsl_version == 300)
		{
			vertex_shader = vertex_shader_glsl_300_es;
			fragment_shader = fragment_shader_glsl_300_es;
		}
		else
		{
			vertex_shader = vertex_shader_glsl_130;
			fragment_shader = fragment_shader_glsl_130;
		}

		// Create shaders
		C_String vertex_shader_with_version[2] = { g_GlslVersion, vertex_shader };
		C_String fragment_shader_with_version[2] = { g_GlslVersion, fragment_shader };
		
		this->CompileShader(
			g_ShaderHandle,
			vertex_shader_with_version,
			fragment_shader_with_version
		);

		g_AttribTex		= ML_GL.getUniformLocation(g_ShaderHandle, "Texture");
		g_AttribProjMtx = ML_GL.getUniformLocation(g_ShaderHandle, "ProjMtx");
		g_AttribPosition= ML_GL.getAttribLocation(g_ShaderHandle, "Position");
		g_AttribUV		= ML_GL.getAttribLocation(g_ShaderHandle, "UV");
		g_AttribColor	= ML_GL.getAttribLocation(g_ShaderHandle, "Color");

		// Create buffers
		g_VboHandle = ML_GL.genBuffers(1);
		g_ElementsHandle = ML_GL.genBuffers(1);

		this->CreateFontsTexture();

		// Restore modified GL state
		ML_GL.bindTexture(GL::Texture2D, last_texture);
		ML_GL.bindBuffer(GL::ArrayBuffer, last_array_buffer);
		ML_GL.bindVertexArray(last_vertex_array);

		return true;
	}

	void ImGuiImpl::DestroyDeviceObjects()
	{
		if (g_VboHandle)
		{
			ML_GL.deleteBuffers(1, &g_VboHandle);
		}
		g_VboHandle = NULL;

		if (g_ElementsHandle)
		{
			ML_GL.deleteBuffers(1, &g_ElementsHandle);
		}
		g_ElementsHandle = NULL;

		if (g_ShaderHandle && g_VertHandle)
		{
			ML_GL.detachShader(g_ShaderHandle, g_VertHandle);
		}

		if (g_VertHandle)
		{
			ML_GL.deleteShader(g_VertHandle);
		}
		g_VertHandle = NULL;

		if (g_ShaderHandle && g_FragHandle)
		{
			ML_GL.detachShader(g_ShaderHandle, g_FragHandle);
		}

		if (g_FragHandle)
		{
			ML_GL.deleteShader(g_FragHandle);
		}
		g_FragHandle = NULL;

		if (g_ShaderHandle)
		{
			ML_GL.deleteShader(g_ShaderHandle);
		}
		g_ShaderHandle = NULL;

		this->DestroyFontsTexture();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ImGuiImpl::HandleInput()
	{
		// Update buttons
		ImGuiIO & io = ImGui::GetIO();
		for (int32_t i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{
			// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
			io.MouseDown[i] = g_MousePressed[i] || g_Window->getMouseButton(i) != 0;
			g_MousePressed[i] = false;
		}

		// Update mouse position
		const ImVec2 mouse_pos_backup = io.MousePos;
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

		if (g_Window->is_focused())
		{
			if (io.WantSetMousePos)
			{
				g_Window->setCursorPos({ (int32_t)mouse_pos_backup.x, (int32_t)mouse_pos_backup.y });
			}
			else
			{
				vec2 mousePos = g_Window->getCursorPos();
				io.MousePos = ImVec2(mousePos[0], mousePos[1]);
			}
		}

		// Mouse
		if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || 
			(g_Window->getInputMode() == (int32_t)Cursor::Mode::Disabled))
			return;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			g_Window->setCursorMode(Cursor::Mode::Hidden);
		}
		else
		{
			// Show OS mouse cursor
			// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with ML 3.2, but 3.3 works here.
			g_Window->setCursor(g_MouseCursors[imgui_cursor] 
				? g_MouseCursors[imgui_cursor] 
				: g_MouseCursors[ImGuiMouseCursor_Arrow]);
			g_Window->setCursorMode(Cursor::Mode::Normal);
		}
	}

	bool ImGuiImpl::CompileShader(uint32_t & obj, const C_String * vs, const C_String * fs)
	{
		if (!ML_GL.shadersAvailable())
		{
			return Debug::logError("ImGui: Shaders are not available on your system.");
		}

		/* * * * * * * * * * * * * * * * * * * * */

		auto disposeShader = [&]()
		{
			ML_GL.useShader(NULL);
			if (obj)
			{
				ML_GL.deleteShader(obj);
				obj = NULL;
			}
			return (!obj);
		};

		/* * * * * * * * * * * * * * * * * * * * */

		// Create Program
		if (disposeShader() && (obj = ML_GL.createProgramObject()))
		{
			// Compile Vertex
			switch (ML_GL.compileShader(g_VertHandle, GL::VertexShader, 2, vs))
			{
			case ML_SUCCESS: ML_GL.attachShader(obj, g_VertHandle); break;
			case ML_FAILURE: ML_GL.deleteShader(obj); return false;
			}

			// Compile Fragment
			switch (ML_GL.compileShader(g_FragHandle, GL::FragmentShader, 2, fs))
			{
			case ML_SUCCESS: ML_GL.attachShader(obj, g_FragHandle); break;
			case ML_FAILURE: ML_GL.deleteShader(obj); return false;
			}

			// Link the program
			if (!ML_GL.linkShader(obj))
			{
				C_String log = ML_GL.getProgramInfoLog(obj);
				ML_GL.deleteShader(obj);
				return Debug::logError("ImGui: Failed linking shader source:\n{0}", log);
			}

			// Refresh OpenGL
			ML_GL.flush();
			return true;
		}
		else
		{
			return Debug::logError("ImGui: Failed compiling shader");
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ImGuiImpl::MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods)
	{
		if ((action == ML_KEY_PRESS) &&
			(button >= 0) &&
			(button < IM_ARRAYSIZE(g_MousePressed)))
			ML_ImGuiImpl.g_MousePressed[button] = true;
		ML_EventSystem.fireEvent<MouseButtonEvent>(button, action, mods);
	}

	void ImGuiImpl::ScrollCallback(void * window, float64_t xoffset, float64_t yoffset)
	{
		ImGuiIO & io = ImGui::GetIO();
		io.MouseWheelH += (float_t)xoffset;
		io.MouseWheel += (float_t)yoffset;
		ML_EventSystem.fireEvent<ScrollEvent>(xoffset, yoffset);
	}

	void ImGuiImpl::KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		ImGuiIO & io = ImGui::GetIO();
		if (action == ML_KEY_PRESS) { io.KeysDown[key] = true; }
		if (action == ML_KEY_RELEASE) { io.KeysDown[key] = false; }
		if (key == KeyCode::KP_Enter) { io.KeysDown[KeyCode::Enter] = io.KeysDown[key]; }
		ML_EventSystem.fireEvent<KeyEvent>(key, scancode, action, bitmask_8 { {
			io.KeyShift = io.KeysDown[KeyCode::LeftShift]	|| io.KeysDown[KeyCode::RightShift],
			io.KeyCtrl	= io.KeysDown[KeyCode::LeftControl] || io.KeysDown[KeyCode::RightControl],
			io.KeyAlt	= io.KeysDown[KeyCode::LeftAlt]		|| io.KeysDown[KeyCode::RightAlt],
			io.KeySuper = io.KeysDown[KeyCode::LeftSuper]	|| io.KeysDown[KeyCode::RightSuper],
			false,	// caps lock
			false	// num lock
		} });
	}

	void ImGuiImpl::CharCallback(void * window, uint32_t value)
	{
		ImGuiIO & io = ImGui::GetIO();
		if ((value > 0) && (value < 0x10000)) { io.AddInputCharacter((uint16_t)value); }
		ML_EventSystem.fireEvent<CharEvent>(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}