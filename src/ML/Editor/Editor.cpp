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

#if defined(ML_IMPL_RENDERER_OPENGL) && defined(ML_IMPL_PLATFORM_GLFW)
#	include <imgui/examples/imgui_impl_glfw.h>
#	include <imgui/examples/imgui_impl_opengl3.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor()
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<BeginGuiEvent>(this);
		ML_EventSystem.addListener<GuiEvent>(this);
		ML_EventSystem.addListener<EndGuiEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);

		m_dockspace		= new Editor_Dockspace{};
		m_mainMenuBar	= new Editor_MainMenuBar{};
		m_about			= new Editor_About{};
		m_content		= new Editor_Content{};
		m_explorer		= new Editor_Explorer{};
		m_inspector		= new Editor_Inspector{};
		m_manual		= new Editor_Manual{};
		m_profiler		= new Editor_Profiler{};
		m_terminal		= new Editor_Terminal{};
	}

	Editor::~Editor()
	{
		delete m_terminal;
		delete m_profiler;
		delete m_manual;
		delete m_inspector;
		delete m_explorer;
		delete m_content;
		delete m_about;
		delete m_mainMenuBar;
		delete m_dockspace;
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

			// Default Style
			switch (util::to_lower(ML_Engine.prefs().get_string(
				"Editor", "default_style", "Classic"
			)).hash())
			{
			case Hash("classic"): { ImGui::StyleColorsClassic(); } break;
			case Hash("dark")	: { ImGui::StyleColorsDark(); } break;
			case Hash("light")	: { ImGui::StyleColorsLight(); } break;
			}

			// Custom Style
			ImGuiStyleLoader().loadFromFile(ML_FS.pathTo(
				ML_Engine.prefs().get_string("Editor", "custom_style", "")
			));

			// Font
			if (const String fontFile{ ML_Engine.prefs().get_string(
				"Editor", "font_file", ""
			) })
			{
				if (const float_t fontSize{ ML_Engine.prefs().get_float(
					"Editor", "font_size", 20.f
				) }; fontSize > 0.0f)
				{
					io.Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize);
				}
			}

			// Init ImGui Platform
			ML_ASSERT(ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)ML_Engine.window().getHandle(), true));

			// Init ImGui Renderer
			ML_ASSERT(ImGui_ImplOpenGL3_Init("#version 130"));

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			mainMenuBar()
				.addMenu("File", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu("New"))
					{
						voidptr_t temp{ nullptr };
						if ((PropertyDrawer<Entity>()("Entity##File##Create", (ptr_t<Entity> &)temp)) ||
							(PropertyDrawer<Font>()("Font##File##Create", (ptr_t<Font> &)temp)) ||
							(PropertyDrawer<Image>()("Image##File##Create", (ptr_t<Image> &)temp)) ||
							(PropertyDrawer<Material>()("Material##File##Create", (ptr_t<Material> &)temp)) ||
							(PropertyDrawer<Model>()("Model##File##Create", (ptr_t<Model> &)temp)) ||
							(PropertyDrawer<Shader>()("Shader##File##Create", (ptr_t<Shader> &)temp)) ||
							(PropertyDrawer<Script>()("Script##File##Create", (ptr_t<Script> &)temp)) ||
							(PropertyDrawer<Texture>()("Texture##File##Create", (ptr_t<Texture> &)temp))
						) ImGui::CloseCurrentPopup();
						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Quit", "Alt+F4"))
					{
						ML_EventSystem.fireEvent<WindowKillEvent>();
					}
					ImGui::PopID();
				})
				.addMenu("View", nullptr)
				.addMenu("Options", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu("Style Editor"))
					{
						ImGui::ShowStyleEditor();
						ImGui::EndMenu();
					}
					bool fullScreen{ ML_Engine.window().isFullscreen() };
					if (ImGui::MenuItem("Fullscreen", "F11", &fullScreen))
					{
						ML_Engine.window().setFullscreen(fullScreen);
					}
					ImGui::PopID();
				})
				.addMenu("Plugins", nullptr)
				.addMenu("Help", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::MenuItem("Repository", "http://")) { Debug::execute("open", ML_PROJECT_URL); }
					if (ImGui::MenuItem("Downloads", "http://")) { Debug::execute("open", "https://bit.ly/ml_noobs"); }
					ImGui::Separator();
					ImGui::MenuItem("ImGui Demo", "", &m_show_imgui_demo);
					ImGui::PopID();
				});

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
			
			if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }

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
				auto backup_context{ Window::getContextCurrent() };
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				Window::makeContextCurrent(backup_context);
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ML_AssetPreview.dispose();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}