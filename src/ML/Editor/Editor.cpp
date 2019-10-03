#include <ML/Editor/Editor.hpp>
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
		: m_about			{ *this }
		, m_dockspace		{ *this }
		, m_explorer		{ *this }
		, m_content			{ *this }
		, m_inspector		{ *this }
		, m_manual			{ *this }
		, m_profiler		{ *this }
		, m_terminal		{ *this }
	{
		ML_EventSystem.addListener(EnterEvent		::ID, this);
		ML_EventSystem.addListener(UpdateEvent		::ID, this);
		ML_EventSystem.addListener(BeginGuiEvent	::ID, this);
		ML_EventSystem.addListener(GuiEvent		::ID, this);
		ML_EventSystem.addListener(EndGuiEvent		::ID, this);
		ML_EventSystem.addListener(UnloadEvent		::ID, this);
		ML_EventSystem.addListener(ExitEvent		::ID, this);
		ML_EventSystem.addListener(DockspaceEvent	::ID, this);
		ML_EventSystem.addListener(KeyEvent		::ID, this);
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

				constexpr KeyEvent::Mods ctrl_alt { 0, 1, 1, 0 };

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
		const String styleConf = ev.prefs.get_string(
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
		String fontFile = ev.prefs.get_string("Editor", "font_file", "");
		if (fontFile)
		{
			float_t fontSize = ev.prefs.get_float("Editor", "font_size", 12.0f);
			if (fontSize > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(
					fontFile.c_str(), fontSize
				);
			}
		}

		// Startup
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_ImGuiImpl.Startup("#version 410", &ev.window, true))
		{
			return Debug::fatal("Failed starting ImGui instance");
		}

		// Capture Cout
		m_terminal.redirect(cout);

		// Get Preferences
		m_about		.setOpen(ev.prefs.get_bool("Editor", "show_about",		false));
		m_content	.setOpen(ev.prefs.get_bool("Editor", "show_content",	false));
		m_explorer	.setOpen(ev.prefs.get_bool("Editor", "show_explorer",	false));
		m_inspector	.setOpen(ev.prefs.get_bool("Editor", "show_inspector",	false));
		m_manual	.setOpen(ev.prefs.get_bool("Editor", "show_manual",		false));
		m_profiler	.setOpen(ev.prefs.get_bool("Editor", "show_profiler",	false));
		m_terminal	.setOpen(ev.prefs.get_bool("Editor", "show_terminal",	false));
	}

	void Editor::onUpdate(const UpdateEvent & ev)
	{
		m_about		.onUpdate(ev);
		m_content	.onUpdate(ev);
		m_explorer	.onUpdate(ev);
		m_dockspace	.onUpdate(ev);
		m_inspector	.onUpdate(ev);
		m_manual	.onUpdate(ev);
		m_profiler	.onUpdate(ev);
		m_terminal	.onUpdate(ev);
	}

	void Editor::onBeginGui(const BeginGuiEvent & ev)
	{
		ML_ImGuiImpl.NewFrame();
		ImGui::NewFrame();
	}

	void Editor::onGui(const GuiEvent & ev)
	{
		// ImGui Demo
		/* * * * * * * * * * * * * * * * * * * * */
		static bool show_imgui_demo = false;
		if (show_imgui_demo) 
		{
			ImGui::ShowDemoWindow(&show_imgui_demo); 
		}
		
		// ImGui Style Editor
		/* * * * * * * * * * * * * * * * * * * * */
		static bool show_imgui_style_editor = false;
		if (show_imgui_style_editor) 
		{
			ImGui::Begin("Style Editor", &show_imgui_style_editor);
			ImGui::ShowStyleEditor(); 
			ImGui::End(); 
		}

		// Main Menu Bar
		/* * * * * * * * * * * * * * * * * * * * */
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

				ML_EventSystem.fireEvent<MainMenuBarEvent>((*this), MainMenuBarEvent::File);

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
				ML_EventSystem.fireEvent<MainMenuBarEvent>((*this), MainMenuBarEvent::Window);
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

				ML_EventSystem.fireEvent<MainMenuBarEvent>((*this), MainMenuBarEvent::Help);
				
				ImGui::EndMenu();
			}

			// User Menu Bars
			ML_EventSystem.fireEvent<MainMenuBarEvent>((*this), MainMenuBarEvent::User);

			ImGui::EndMainMenuBar();
		}

		/*	Dockspace	*/	if (m_dockspace	.isOpen()) m_dockspace	.onGui(ev);
		/*	About		*/	if (m_about		.isOpen()) m_about		.onGui(ev);
		/*	Content		*/	if (m_content	.isOpen()) m_content	.onGui(ev);
		/*	Explorer	*/	if (m_explorer	.isOpen()) m_explorer	.onGui(ev);
		/*	Inspector	*/	if (m_inspector	.isOpen()) m_inspector	.onGui(ev);
		/*	Manual		*/	if (m_manual	.isOpen()) m_manual		.onGui(ev);
		/*	Profiler	*/	if (m_profiler	.isOpen()) m_profiler	.onGui(ev);
		/*	Terminal	*/	if (m_terminal	.isOpen()) m_terminal	.onGui(ev);
	}

	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		ImGui::Render();
		ev.window.makeContextCurrent();
		ML_ImGuiImpl.Render(ImGui::GetDrawData());
	}

	void Editor::onUnload(const UnloadEvent & ev)
	{
		// Shutdown ImGui
		ML_ImGuiImpl.Shutdown();
	}

	void Editor::onExit(const ExitEvent & ev)
	{
		// Release Cout
		m_terminal.redirect(cout);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}