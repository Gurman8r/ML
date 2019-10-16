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
#include <ML/Core/OS.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Editor/PropertyDrawer.hpp>

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
		case EnterEvent::ID		: if (auto ev = value.as<EnterEvent>())		return onEnter(*ev);
		case UpdateEvent::ID	: if (auto ev = value.as<UpdateEvent>())	return onUpdate(*ev);
		case BeginGuiEvent::ID	: if (auto ev = value.as<BeginGuiEvent>())	return onBeginGui(*ev);
		case GuiEvent::ID		: if (auto ev = value.as<GuiEvent>())		return onGui(*ev);
		case EndGuiEvent::ID	: if (auto ev = value.as<EndGuiEvent>())	return onEndGui(*ev);
		case UnloadEvent::ID	: if (auto ev = value.as<UnloadEvent>())	return onUnload(*ev);
		case ExitEvent::ID		: if (auto ev = value.as<ExitEvent>())		return onExit(*ev);

			// Dockspace
		case DockspaceEvent::ID:
			if (auto ev = value.as<DockspaceEvent>())
			{
				EditorDockspace & d { ev->dockspace };
				d.dockWindow(m_explorer	.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_profiler	.getTitle(), d.getNode(d.RightUp));
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
		String fontFile = ML_Engine.prefs().get_string("Editor", "font_file", "");
		if (fontFile)
		{
			float_t fontSize = ML_Engine.prefs().get_float("Editor", "font_size", 12.0f);
			if (fontSize > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(
					fontFile.c_str(), fontSize
				);
			}
		}

		// Startup
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_ImGuiImpl.Startup("#version 410", &ML_Engine.window(), true))
		{
			return Debug::fatal("Failed starting ImGui instance");
		}

		// Capture Cout
		m_terminal.redirect(cout);

		// Get Preferences
		m_about		.setOpen(ML_Engine.prefs().get_bool("Editor", "show_about",		false));
		m_content	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_content",	false));
		m_explorer	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_explorer",	false));
		m_inspector	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_inspector",	false));
		m_manual	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_manual",		false));
		m_profiler	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_profiler",	false));
		m_terminal	.setOpen(ML_Engine.prefs().get_bool("Editor", "show_terminal",	false));
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
		static bool show_imgui_demo = false;
		static bool show_imgui_style_editor = false;
		
		// Main Menu Bar
		if (ImGui::BeginMainMenuBar())
		{
			// Menu -> File
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				// File -> Create
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginMenu("Create"))
				{
					void * temp { nullptr };
					PropertyDrawer<Entity>()("Entity##File##Create", (Entity *&)temp, true);
					PropertyDrawer<Font>()("Font##File##Create", (Font *&)temp, true);
					//PropertyDrawer<Image>()("Image##File##Create", (Image *&)temp, true);
					PropertyDrawer<Material>()("Material##File##Create", (Material *&)temp, true);
					PropertyDrawer<Model>()("Model##File##Create", (Model *&)temp, true);
					PropertyDrawer<Shader>()("Shader##File##Create", (Shader *&)temp, true);
					PropertyDrawer<Script>()("Script##File##Create", (Script *&)temp, true);
					PropertyDrawer<Texture>()("Texture##File##Create", (Texture *&)temp, true);
					ImGui::EndMenu();
				}

				ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::File);

				// File -> Quit
				/* * * * * * * * * * * * * * * * * * * * */
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					ML_EventSystem.fireEvent<WindowKillEvent>();
				}

				ImGui::EndMenu();
			}

			// Menu -> Window
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				m_content.MenuItem();
				m_explorer.MenuItem();
				m_inspector.MenuItem();
				m_profiler.MenuItem();
				m_terminal.MenuItem();
				ImGui::Separator();
				ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Window);
				ImGui::EndMenu();
			}

			// Menu -> Help
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				m_about.MenuItem();
				
				if (ImGui::MenuItem("Repository", "http://"))
					OS::execute("open", ML_PROJECT_URL);
				
				if (ImGui::MenuItem("Downloads", "http://")) 
					OS::execute("open", "https://bit.ly/ml_noobs");

				ImGui::MenuItem("ImGui Demo", "", &show_imgui_demo);

				ImGui::MenuItem("Style Editor", "", &show_imgui_style_editor);

				ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::Help);
				
				ImGui::EndMenu();
			}

			// User Menu Bars
			ML_EventSystem.fireEvent<MainMenuBarEvent>(MainMenuBarEvent::User);

			ImGui::EndMainMenuBar();
		}

		// ImGui Demo
		if (show_imgui_demo)
		{
			ImGui::ShowDemoWindow(&show_imgui_demo);
		}

		// Style Editor
		if (show_imgui_style_editor)
		{
			ImGui::Begin("Style Editor", &show_imgui_style_editor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		/*	Dockspace	*/	if (m_dockspace.isOpen())	m_dockspace.draw();
		/*	About		*/	if (m_about.isOpen())		m_about.draw();
		/*	Content		*/	if (m_content.isOpen())		m_content.draw();
		/*	Explorer	*/	if (m_explorer.isOpen())	m_explorer.draw();
		/*	Inspector	*/	if (m_inspector.isOpen())	m_inspector.draw();
		/*	Manual		*/	if (m_manual.isOpen())		m_manual.draw();
		/*	Profiler	*/	if (m_profiler.isOpen())	m_profiler.draw();
		/*	Terminal	*/	if (m_terminal.isOpen())	m_terminal.draw();
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
}