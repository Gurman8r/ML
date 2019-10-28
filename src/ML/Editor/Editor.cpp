#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/AssetPreview.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiImpl.hpp>
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
		// Initialize Implementation Instance
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Style
		/* * * * * * * * * * * * * * * * * * * * */
		const String styleConf = ML_Engine.prefs().get_string(
			"Editor", "style_config", "Classic"
		);
		if (styleConf == "Classic")  ImGui::StyleColorsClassic();
		else if (styleConf == "Dark") ImGui::StyleColorsDark();
		else if (styleConf == "Light") ImGui::StyleColorsLight();
		else if (!ML_ImGuiImpl.LoadStyle(ML_FS.pathTo(styleConf)))
		{
			Debug::logError("Failed loading ImGui style");
		}

		// Setup Fonts
		/* * * * * * * * * * * * * * * * * * * * */
		if (String fontFile { ML_Engine.prefs().get_string("Editor", "font_file", "") })
		{
			float_t fontSize { ML_Engine.prefs().get_float("Editor", "font_size", 12.0f) };
			if (fontSize > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize);
			}
		}

		// Startup
		/* * * * * * * * * * * * * * * * * * * * */
		ML_ASSERT(
			"Starting ImGui" &&
			ML_ImGuiImpl.Startup(
				"#version 130",
				&ML_Engine.window(),
				true,
				ML_Engine.prefs().get_bool("Editor", "use_imgui_ini", false),
				ML_Engine.prefs().get_bool("Editor", "use_imgui_log", false)
			)
		);

		// Get Preferences
		m_about		.setOpen(ML_Engine.prefs().get_bool("Editor", "show_about",		false));
		m_content	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_content",	false));
		m_explorer	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_explorer",	false));
		m_inspector	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_inspector",	false));
		m_manual	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_manual",	false));
		m_profiler	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_profiler",	false));
		m_terminal	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_terminal",	false));
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
		ML_ImGuiImpl.NewFrame();
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
		ImGui::Render();
		ML_Engine.window().makeContextCurrent();
		ML_ImGuiImpl.Render(ImGui::GetDrawData());
	}

	void Editor::onUnload(const UnloadEvent & ev)
	{
		ML_ImGuiImpl.Shutdown();
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
				void * temp { nullptr };
				if ((PropertyDrawer<Entity>()("Entity##File##Create", (Entity *&)temp)) ||
					(PropertyDrawer<Font>()("Font##File##Create", (Font *&)temp)) ||
					(PropertyDrawer<Image>()("Image##File##Create", (Image *&)temp)) ||
					(PropertyDrawer<Material>()("Material##File##Create", (Material *&)temp)) ||
					(PropertyDrawer<Model>()("Model##File##Create", (Model *&)temp)) ||
					(PropertyDrawer<Shader>()("Shader##File##Create", (Shader *&)temp)) ||
					(PropertyDrawer<Script>()("Script##File##Create", (Script *&)temp)) ||
					(PropertyDrawer<Texture>()("Texture##File##Create", (Texture *&)temp))
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

			if (ImGui::BeginMenu("Backend Flags"))
			{
				ImGuiExt::HelpMarker("These flags are set internally and specify the backend's capabilities.");

				ImGuiBackendFlags backend_flags { io.BackendFlags }; // Make a local copy to avoid modifying actual back-end flags.

				ImGui::CheckboxFlags("Has Gamepad", (uint32_t *)&backend_flags, ImGuiBackendFlags_HasGamepad);
				ImGuiExt::Tooltip(
					"Platform supports gamepad and currently has one connected."
				);

				ImGui::CheckboxFlags("Has Mouse Cursors", (uint32_t *)&backend_flags, ImGuiBackendFlags_HasMouseCursors);
				ImGuiExt::Tooltip(
					"Platform supports honoring GetMouseCursor() value to change the OS cursor shape."
				);

				ImGui::CheckboxFlags("Has Set Mouse Position", (uint32_t *)&backend_flags, ImGuiBackendFlags_HasSetMousePos);
				ImGuiExt::Tooltip(
					"Platform supports io.WantSetMousePos requests to reposition the OS mouse position"
				);

				ImGui::CheckboxFlags("Platform Has Viewports", (uint32_t *)&backend_flags, ImGuiBackendFlags_PlatformHasViewports);
				ImGuiExt::Tooltip(
					"Platform supports multiple viewports."
				);

				ImGui::CheckboxFlags("Has Mouse Hovered Viewport", (uint32_t *)&backend_flags, ImGuiBackendFlags_HasMouseHoveredViewport);
				ImGuiExt::Tooltip(
					"Back-end Platform supports setting io.MouseHoveredViewport to the viewport directly under the mouse IGNORING viewports with the \'No Inputs\' flag and REGARDLESS of whether another viewport is focused and may be capturing the mouse.\n"
					"This information is NOT EASY to provide correctly with most high-level engines! Don't set this without studying how the examples/ back-end handle it!"
				);

				ImGui::CheckboxFlags("Renderer Has Vertex Offset", (uint32_t *)&backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
				ImGuiExt::Tooltip(
					"Renderer supports ImDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bits indices."
				);

				ImGui::CheckboxFlags("Renderer Has Viewports", (uint32_t *)&backend_flags, ImGuiBackendFlags_RendererHasViewports);
				ImGuiExt::Tooltip(
					"Renderer supports multiple viewports."
				);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::BeginMenu("Style Editor"))
				{
					ImGui::ShowStyleEditor();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			bool fullScreen { ML_Engine.window().is_fullscreen() };
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