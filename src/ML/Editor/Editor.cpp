#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/AssetPreview.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiStyleLoader.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Editor/ImGuiExt.hpp>

#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor()
		: m_about{}
		, m_dockspace{}
		, m_explorer{}
		, m_content{}
		, m_inspector{}
		, m_manual{}
		, m_mainMenuBar{}
		, m_profiler{}
		, m_terminal{}
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<BeginGuiEvent>(this);
		ML_EventSystem.addListener<GuiEvent>(this);
		ML_EventSystem.addListener<EndGuiEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const Event & value)
	{
		switch (*value)
		{
			case EnterEvent::ID: if (auto ev{ value.as<EnterEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Create ImGui Context
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();

				auto & io{ ImGui::GetIO() };
				auto & style{ ImGui::GetStyle() };

				// Config
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					style.WindowRounding = 0.0f;
					style.Colors[ImGuiCol_WindowBg].w = 1.0f;
				}

				// Ini File
				if (!ML_Engine.prefs().get_bool("Editor", "enable_ini", true))
				{
					io.IniFilename = nullptr;
				}

				// Log File
				if (!ML_Engine.prefs().get_bool("Editor", "enable_log", true))
				{
					io.LogFilename = nullptr;
				}

				// Style
				const String styleConf{ ML_Engine.prefs().get_string(
					"Editor", "editor_style", "Classic"
				) };
				switch (util::to_lower(styleConf).hash())
				{
					case Hash("classic"): { ImGui::StyleColorsClassic(); } break;
					case Hash("dark"): { ImGui::StyleColorsDark(); } break;
					case Hash("light"): { ImGui::StyleColorsLight(); } break;
				}
				ImGuiStyleLoader().loadFromFile(ML_FS.pathTo(styleConf));

				// Font
				if (String fontFile{ ML_Engine.prefs().get_string("Editor", "font_file", "") })
				{
					float_t fontSize{ ML_Engine.prefs().get_float("Editor", "font_size", 20.f) };
					if (fontSize > 0.0f)
					{
						ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize);
					}
				}

				// Imgui GLFW Init for OpenGL
				ML_ASSERT(ImGui_ImplGlfw_InitForOpenGL(
					(GLFWwindow *)ML_Engine.window().getHandle(), true
				));

				// Imgui OpenGL3 Init
				ML_ASSERT(ImGui_ImplOpenGL3_Init(
					"#version 130"
				));

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				m_terminal.redirect(cout);

				m_mainMenuBar.addMenu("File", [&]()
				{
					if (ImGui::BeginMenu("New"))
					{
						ptr_t<void> temp{ nullptr };
						if ((PropertyDrawer<Entity>()("Entity##File##Create", (ptr_t<Entity> &)temp)) ||
							(PropertyDrawer<Font>()("Font##File##Create", (ptr_t<Font> &)temp)) ||
							(PropertyDrawer<Image>()("Image##File##Create", (ptr_t<Image> &)temp)) ||
							(PropertyDrawer<Material>()("Material##File##Create", (ptr_t<Material> &)temp)) ||
							(PropertyDrawer<Model>()("Model##File##Create", (ptr_t<Model> &)temp)) ||
							(PropertyDrawer<Shader>()("Shader##File##Create", (ptr_t<Shader> &)temp)) ||
							(PropertyDrawer<Script>()("Script##File##Create", (ptr_t<Script> &)temp)) ||
							(PropertyDrawer<Texture>()("Texture##File##Create", (ptr_t<Texture> &)temp))
							)
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Quit", "Alt+F4"))
					{
						ML_EventSystem.fireEvent<WindowKillEvent>();
					}
				});
				
				m_mainMenuBar.addMenu("Window", [&]()
				{
					m_content.MenuItem();
					m_explorer.MenuItem();
					m_inspector.MenuItem();
					m_profiler.MenuItem();
					m_terminal.MenuItem();
				});
				
				m_mainMenuBar.addMenu("Options", [&]()
				{
					auto & io{ ImGui::GetIO() };
					if (ImGui::BeginMenu("Style"))
					{
						if (ImGui::BeginMenu("Style Editor"))
						{
							ImGui::ShowStyleEditor();
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					bool fullScreen{ ML_Engine.window().isFullscreen() };
					if (ImGui::MenuItem("Fullscreen", "F11", &fullScreen))
					{
						ML_Engine.window().setFullscreen(fullScreen);
					}
				});
				
				m_mainMenuBar.addMenu("Plugins", [&]() {});
				
				m_mainMenuBar.addMenu("Help", [&]()
				{
					auto & io{ ImGui::GetIO() };
					m_about.MenuItem();
					ImGui::Separator();
					if (ImGui::MenuItem("Repository", "http://")) { Debug::execute("open", ML_PROJECT_URL); }
					if (ImGui::MenuItem("Downloads", "http://")) { Debug::execute("open", "https://bit.ly/ml_noobs"); }
					ImGui::Separator();
					ImGui::MenuItem("ImGui Demo", "", &m_show_imgui_demo);
				});

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				m_about.update();
				m_content.update();
				m_dockspace.update();
				m_explorer.update();
				m_inspector.update();
				m_mainMenuBar.update();
				m_manual.update();
				m_profiler.update();
				m_terminal.update();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case BeginGuiEvent::ID: if (auto ev{ value.as<BeginGuiEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case GuiEvent::ID: if (auto ev{ value.as<GuiEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::PushID(ML_ADDRESSOF(this));
				/*	Menu Bar	*/	if (m_mainMenuBar.isOpen())	m_mainMenuBar.draw();
				/*	Dockspace	*/	if (m_dockspace.isOpen())	m_dockspace.draw();
				/*	ImGui Demo	*/	if (m_show_imgui_demo)		ImGui::ShowDemoWindow(&m_show_imgui_demo);
				/*	About		*/	if (m_about.isOpen())		m_about.draw();
				/*	Content		*/	if (m_content.isOpen())		m_content.draw();
				/*	Explorer	*/	if (m_explorer.isOpen())	m_explorer.draw();
				/*	Inspector	*/	if (m_inspector.isOpen())	m_inspector.draw();
				/*	Manual		*/	if (m_manual.isOpen())		m_manual.draw();
				/*	Profiler	*/	if (m_profiler.isOpen())	m_profiler.draw();
				/*	Terminal	*/	if (m_terminal.isOpen())	m_terminal.draw();
				ImGui::PopID();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case EndGuiEvent::ID: if (auto ev{ value.as<EndGuiEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow * backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				m_mainMenuBar.dispose();
				m_terminal.redirect(cout);
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ML_AssetPreview.dispose();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case DockspaceEvent::ID: if (auto ev = value.as<DockspaceEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				Editor_Dockspace & d{ ev->dockspace };
				if (!d.isOpen()) return;
				d.dockWindow(m_explorer.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_profiler.getTitle(), d.getNode(d.LeftDn));
				d.dockWindow(m_inspector.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_content.getTitle(), d.getNode(d.RightDn));
				d.dockWindow(m_terminal.getTitle(), d.getNode(d.LeftDn));
				d.dockWindow(m_about.getTitle(), d.getNode(d.LeftUp));

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				constexpr KeyEvent::Mods ctrl_alt{ { 0, 1, 1, 0 } };

				// Show About | Ctrl + Alt + A
				if (ev->getPress(KeyCode::A, ctrl_alt)) m_about.toggleOpen();

				// Show Content | Ctrl + Alt + C
				if (ev->getPress(KeyCode::C, ctrl_alt)) m_content.toggleOpen();

				// Show Explorer | Ctrl + Alt + E
				if (ev->getPress(KeyCode::E, ctrl_alt)) m_explorer.toggleOpen();

				// Show Inspector | Ctrl + Alt + I
				if (ev->getPress(KeyCode::I, ctrl_alt)) m_inspector.toggleOpen();

				// Show Manual | Ctrl + Alt + M
				if (ev->getPress(KeyCode::M, ctrl_alt)) m_manual.toggleOpen();

				// Show Profiler | Ctrl + Alt + P
				if (ev->getPress(KeyCode::P, ctrl_alt)) m_profiler.toggleOpen();

				// Show Terminal | Ctrl + Alt + T
				if (ev->getPress(KeyCode::T, ctrl_alt)) m_terminal.toggleOpen();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}