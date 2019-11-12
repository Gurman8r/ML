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
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor()
		: m_about		{ }
		, m_dockspace	{ }
		, m_explorer	{ }
		, m_content		{ }
		, m_inspector	{ }
		, m_manual		{ }
		, m_profiler	{ }
		, m_terminal	{ }
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<BeginGuiEvent>(this);
		ML_EventSystem.addListener<GuiEvent>(this);
		ML_EventSystem.addListener<EndGuiEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<ExitEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID:	return onEnter(*value.as<EnterEvent>());
		case LoadEvent::ID:		return onLoad(*value.as<LoadEvent>());
		case UpdateEvent::ID:	return onUpdate(*value.as<UpdateEvent>());
		case BeginGuiEvent::ID:	return onBeginGui(*value.as<BeginGuiEvent>());
		case GuiEvent::ID:		return onGui(*value.as<GuiEvent>());
		case EndGuiEvent::ID:	return onEndGui(*value.as<EndGuiEvent>());
		case UnloadEvent::ID:	return onUnload(*value.as<UnloadEvent>());
		case ExitEvent::ID:		return onExit(*value.as<ExitEvent>());

			// Dockspace
		case DockspaceEvent::ID:
			if (auto ev = value.as<DockspaceEvent>())
			{
				EditorDockspace & d { ev->dockspace };
				d.dockWindow(m_explorer	.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_profiler	.getTitle(), d.getNode(d.LeftDn));
				d.dockWindow(m_inspector.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_content	.getTitle(), d.getNode(d.RightDn));
				d.dockWindow(m_terminal	.getTitle(), d.getNode(d.LeftDn));
				d.dockWindow(m_about	.getTitle(), d.getNode(d.LeftUp));
			}
			break;

			// Key
		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				constexpr KeyEvent::Mods ctrl_alt { { 0, 1, 1, 0 } };

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
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEnter(const EnterEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		/* * * * * * * * * * * * * * * * * * * * */

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
		if (ML_Engine.prefs().get_bool("Editor", "disable_ini", false))
		{
			io.IniFilename = nullptr;
		}
		
		// Log File
		if (ML_Engine.prefs().get_bool("Editor", "disable_log", false))
		{
			io.LogFilename = nullptr;
		}

		// Style
		const String styleConf{ ML_Engine.prefs().get_string(
			"Editor", "editor_style", "Classic"
		) };
		if (styleConf == "Classic") { ImGui::StyleColorsClassic(); }
		else if (styleConf == "Dark") { ImGui::StyleColorsDark(); }
		else if (styleConf == "Light") { ImGui::StyleColorsLight(); }
		else if (!ImGuiStyleLoader{ ML_FS.pathTo(styleConf) })
		{
			Debug::logError("Failed loading ImGui style");
		}

		// Font
		if (String fontFile { ML_Engine.prefs().get_string("Editor", "font_file", "") })
		{
			float_t fontSize { ML_Engine.prefs().get_float("Editor", "font_size", 20.f) };
			if (fontSize > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)ML_Engine.window().getHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

		/* * * * * * * * * * * * * * * * * * * * */

		m_about		.setOpen(ML_Engine.prefs().get_bool("Editor", "show_about",		false));
		m_content	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_content",	false));
		m_explorer	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_explorer",	false));
		m_inspector	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_inspector",	false));
		m_manual	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_manual",	false));
		m_profiler	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_profiler",	false));
		m_terminal	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_terminal",	false));

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Editor::onLoad(const LoadEvent & ev)
	{
		// by now resources should be loaded
		m_terminal.redirect(cout);
	}

	void Editor::onUpdate(const UpdateEvent & ev)
	{
		m_about.update();
		m_content.update();
		m_explorer.update();
		m_dockspace.update();
		m_inspector.update();
		m_manual.update();
		m_profiler.update();
		m_terminal.update();
	}

	void Editor::onBeginGui(const BeginGuiEvent & ev)
	{
		//ML_ImGuiImpl.NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::onGui(const GuiEvent & ev)
	{
		ImGui::PushID(ML_ADDRESSOF(this));

		// Main Menu Bar
		if (m_show_main_menu_bar) { draw_main_menu_bar(); }

		// ImGui Demo
		if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }

		/*	Dockspace	*/	if (m_dockspace.isOpen())	m_dockspace.draw();
		/*	About		*/	if (m_about.isOpen())		m_about.draw();
		/*	Content		*/	if (m_content.isOpen())		m_content.draw();
		/*	Explorer	*/	if (m_explorer.isOpen())	m_explorer.draw();
		/*	Inspector	*/	if (m_inspector.isOpen())	m_inspector.draw();
		/*	Manual		*/	if (m_manual.isOpen())		m_manual.draw();
		/*	Profiler	*/	if (m_profiler.isOpen())	m_profiler.draw();
		/*	Terminal	*/	if (m_terminal.isOpen())	m_terminal.draw();

		ImGui::PopID();
	}

	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		auto window{ static_cast<GLFWwindow *>(ML_Engine.window().getHandle()) };
		auto & io{ ImGui::GetIO() };
		ImVec4 clear_color{ 0, 0, 0, 1 };

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow * backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Editor::onUnload(const UnloadEvent & ev)
	{
		//ML_ImGuiImpl.Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ML_AssetPreview.dispose();
	}

	void Editor::onExit(const ExitEvent & ev)
	{
		// Release Cout
		m_terminal.redirect(cout);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::draw_main_menu_bar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			draw_file_menu		(true);
			draw_edit_menu		(false);
			draw_view_menu		(false);
			draw_window_menu	(true);
			draw_options_menu	(true);
			draw_plugins_menu	(true);
			draw_help_menu		(true);
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::User);
			ImGui::EndMainMenuBar();
		}
	}

	void Editor::draw_file_menu(bool enabled)
	{
		if (ImGui::BeginMenu("File"))
		{
			// File -> Create
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("New"))
			{
				ptr_t<void> temp { nullptr };
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

			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::File);

			ImGui::Separator();

			// File -> Quit
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				ML_EventSystem.fireEvent<WindowKillEvent>();
			}

			ImGui::EndMenu();
		}
	}

	void Editor::draw_edit_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("Edit"))
		{
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Edit);

			ImGui::EndMenu();
		}
	}

	void Editor::draw_view_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("View"))
		{
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::View);

			ImGui::EndMenu();
		}
	}

	void Editor::draw_window_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("Window"))
		{
			m_content.MenuItem();
			
			m_explorer.MenuItem();
			
			m_inspector.MenuItem();
			
			m_profiler.MenuItem();
			
			m_terminal.MenuItem();
			
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Window);
			
			ImGui::EndMenu();
		}
	}

	void Editor::draw_options_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("Options"))
		{
			auto & io { ImGui::GetIO() };

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::BeginMenu("Style Editor"))
				{
					ImGui::ShowStyleEditor();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			bool fullScreen { ML_Engine.window().isFullscreen() };
			if (ImGui::MenuItem("Fullscreen", "F11", &fullScreen))
			{
				ML_Engine.window().setFullscreen(fullScreen);
			}
			
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Options);
			
			ImGui::EndMenu();
		}
	}

	void Editor::draw_plugins_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("Plugins"))
		{
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Plugins);
			
			ImGui::EndMenu();
		}
	}

	void Editor::draw_help_menu(bool enabled)
	{
		if (enabled && ImGui::BeginMenu("Help"))
		{
			auto & io { ImGui::GetIO() };

			m_about.MenuItem();

			ImGui::Separator();

			if (ImGui::MenuItem("Repository", "http://"))
				Debug::execute("open", ML_PROJECT_URL);

			if (ImGui::MenuItem("Downloads", "http://"))
				Debug::execute("open", "https://bit.ly/ml_noobs");

			ImGui::Separator();

			ImGui::MenuItem("ImGui Demo", "", &m_show_imgui_demo);

			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Help);

			ImGui::EndMenu();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}