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

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(EventSystem & eventSystem)
		: I_EventListener	{ eventSystem }
		, m_explorer		{ *this }
		, m_content			{ *this }
		, m_dockspace		{ *this }
		, m_importer		{ *this }
		, m_profiler		{ *this }
		, m_manual			{ *this }
		, m_terminal		{ *this }
	{
		eventSystem.addListener(EnterEvent		::ID, this);
		eventSystem.addListener(UpdateEvent		::ID, this);
		eventSystem.addListener(BeginGuiEvent	::ID, this);
		eventSystem.addListener(GuiEvent		::ID, this);
		eventSystem.addListener(EndGuiEvent		::ID, this);
		eventSystem.addListener(UnloadEvent		::ID, this);
		eventSystem.addListener(ExitEvent		::ID, this);

		eventSystem.addListener(DockspaceEvent	::ID, this);
		eventSystem.addListener(KeyEvent		::ID, this);
		eventSystem.addListener(File_New_Event	::ID, this);
		eventSystem.addListener(File_Open_Event	::ID, this);
		eventSystem.addListener(File_Save_Event	::ID, this);
		eventSystem.addListener(File_Quit_Event	::ID, this);
		eventSystem.addListener(Edit_Undo_Event	::ID, this);
		eventSystem.addListener(Edit_Redo_Event	::ID, this);
		eventSystem.addListener(Edit_Cut_Event	::ID, this);
		eventSystem.addListener(Edit_Copy_Event	::ID, this);
		eventSystem.addListener(Edit_Paste_Event::ID, this);
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
				d.dockWindow(m_content	.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_importer	.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_terminal	.getTitle(), d.getNode(d.LeftDn));
			}
			break;

			// Key
		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Show Terminal | Ctrl + Alt + T
				if (ev->getPress(KeyCode::T, { 0, 1, 1, 0 })) m_terminal.toggleOpen();

				// Show Explorer | Ctrl + Alt + E
				if (ev->getPress(KeyCode::E, { 0, 1, 1, 0 })) m_explorer.toggleOpen();

				// Show Importer | Ctrl + Alt + I
				if (ev->getPress(KeyCode::I, { 0, 1, 1, 0 })) m_importer.toggleOpen();

				// Show Profiler | Ctrl + Alt + P
				if (ev->getPress(KeyCode::P, { 0, 1, 1, 0 })) m_profiler.toggleOpen();

				// Show Resources | Ctrl + Alt + C
				if (ev->getPress(KeyCode::C, { 0, 1, 1, 0 })) m_content.toggleOpen();

				// Show Resources | Ctrl + Alt + M
				if (ev->getPress(KeyCode::M, { 0, 1, 1, 0 })) m_manual.toggleOpen();

				/* * * * * * * * * * * * * * * * * * * * */

				// File -> New | Ctrl + N
				if (ev->isNew()) eventSystem().fireEvent(File_New_Event());

				// File -> Open | Ctrl + O
				if (ev->isOpen()) eventSystem().fireEvent(File_Open_Event());

				// File -> Save | Ctrl + S
				if (ev->isSave()) eventSystem().fireEvent(File_Save_Event());

				/* * * * * * * * * * * * * * * * * * * * */

				// Edit -> Undo	| Ctrl + Z
				if (ev->isUndo()) eventSystem().fireEvent(Edit_Undo_Event());

				// Edit -> Redo	| Ctrl + Y / Ctrl + Shift + Z
				if (ev->isRedo()) eventSystem().fireEvent(Edit_Redo_Event());

				// Edit -> Cut	| Ctrl + X / Shift + Insert
				if (ev->isCut()) eventSystem().fireEvent(Edit_Cut_Event());

				// Edit -> Copy | Ctrl + C / Ctrl + Insert
				if (ev->isCopy()) eventSystem().fireEvent(Edit_Copy_Event());

				// Edit -> Paste | Ctrl + V / Shift + Insert
				if (ev->isPaste()) eventSystem().fireEvent(Edit_Paste_Event());

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;


			// File -> New
		case File_New_Event::ID:
			if (auto ev = value.as<File_New_Event>()) {}
			break;

			// File -> Open
		case File_Open_Event::ID:
			if (auto ev = value.as<File_Open_Event>())
			{
				if (m_explorer.isOpen())
				{
					OS::execute("open", m_explorer.get_selected_path());
				}
			}
			break;

			// File -> Save
		case File_Save_Event::ID:
			if (auto ev = value.as<File_Save_Event>()) {}
			break;

			// File -> Exit
		case File_Quit_Event::ID:
			if (auto ev = value.as<File_Quit_Event>())
			{
				eventSystem().fireEvent(WindowKillEvent());
			}
			break;

			// Edit -> Undo
		case Edit_Undo_Event::ID:
			if (auto ev = value.as<Edit_Undo_Event>()) {}
			break;

			// Edit -> Redo
		case Edit_Redo_Event::ID:
			if (auto ev = value.as<Edit_Redo_Event>()) {}
			break;

			// Edit -> Cut
		case Edit_Cut_Event::ID:
			if (auto ev = value.as<Edit_Cut_Event>()) {}
			break;

			// Edit -> Copy
		case Edit_Copy_Event::ID:
			if (auto ev = value.as<Edit_Copy_Event>()) {}
			break;

			// Edit -> Paste
		case Edit_Paste_Event::ID: 
			if (auto ev = value.as<Edit_Paste_Event>()) {}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEnter(const EnterEvent & ev)
	{
		m_redirect_cout = ev.prefs.get_bool("Editor", "redirect_cout", false);
		m_show_menubar = ev.prefs.get_bool("Editor", "show_menubar", true);

		// Initialize Implementation Instance
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Style
		/* * * * * * * * * * * * * * * * * * * * */
		const String styleFile = ev.prefs.get_string(
			"Editor", "style_file", "Classic"
		);
		if (styleFile == "Classic")  ImGui::StyleColorsClassic();
		else if (styleFile == "Dark") ImGui::StyleColorsDark();
		else if (styleFile == "Light") ImGui::StyleColorsLight();
		else if (!ML_ImGuiImpl.LoadStyle(ML_FS.pathTo(styleFile)))
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
		if (m_redirect_cout) m_terminal.redirect(cout);

		// Configure Builtin Windows
		m_content	.setOpen(ev.prefs.get_bool("Editor", "show_content",	false));
		m_explorer	.setOpen(ev.prefs.get_bool("Editor", "show_explorer",	false));
		m_importer	.setOpen(ev.prefs.get_bool("Editor", "show_importer",	false));
		m_profiler	.setOpen(ev.prefs.get_bool("Editor", "show_profiler",	false));
		m_manual	.setOpen(ev.prefs.get_bool("Editor", "show_manual",		false));
		m_terminal	.setOpen(ev.prefs.get_bool("Editor", "show_terminal",	false));
	}

	void Editor::onUpdate(const UpdateEvent & ev)
	{
		m_content	.onUpdate(ev);
		m_explorer	.onUpdate(ev);
		m_dockspace	.onUpdate(ev);
		m_importer	.onUpdate(ev);
		m_profiler	.onUpdate(ev);
		m_manual	.onUpdate(ev);
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
		if (m_show_imgui_demo)	{ ImGui::ShowDemoWindow(&m_show_imgui_demo); }
		if (m_show_imgui_metrics) { ImGui::ShowMetricsWindow(&m_show_imgui_metrics); }
		if (m_show_imgui_style)	{ ImGui::Begin("Dear ImGui Style Editor", &m_show_imgui_style); ImGui::ShowStyleEditor(); ImGui::End(); }
		if (m_show_imgui_about)	{ ImGui::ShowAboutWindow(&m_show_imgui_about); }

		// Main Menu Bar
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_show_menubar && ImGui::BeginMainMenuBar())
		{
			// Menu -> File
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				// File -> New
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					eventSystem().fireEvent(File_New_Event());
				}
				// File -> Open
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					eventSystem().fireEvent(File_Open_Event());
				}
				// File -> Save
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					eventSystem().fireEvent(File_Save_Event());
				}
				ImGui::Separator();
				// File -> Quit
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					eventSystem().fireEvent(File_Quit_Event());
				}

				eventSystem().fireEvent(MainMenuBarEvent(MainMenuBarEvent::File));

				ImGui::EndMenu();
			}

			// Menu -> Edit
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				// Edit -> Undo
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
					eventSystem().fireEvent(Edit_Undo_Event());
				}
				// Edit -> Redo
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
					eventSystem().fireEvent(Edit_Redo_Event());
				}
				ImGui::Separator();
				// Edit -> Cut
				if (ImGui::MenuItem("Cut", "Ctrl+X"))
				{
					eventSystem().fireEvent(Edit_Cut_Event());
				}
				// Edit -> Copy
				if (ImGui::MenuItem("Copy", "Ctrl+C"))
				{
					eventSystem().fireEvent(Edit_Copy_Event());
				}
				// Edit -> Paste
				if (ImGui::MenuItem("Paste", "Ctrl+V"))
				{
					eventSystem().fireEvent(Edit_Paste_Event());
				}

				eventSystem().fireEvent(MainMenuBarEvent(MainMenuBarEvent::Edit));

				ImGui::EndMenu();
			}

			// Menu -> Window
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(m_content.getTitle(),	"Ctrl+Alt+C", m_content	.openPtr());
				ImGui::MenuItem(m_explorer.getTitle(),	"Ctrl+Alt+E", m_explorer.openPtr());
				//ImGui::MenuItem(m_importer.getTitle(),	"Ctrl+Alt+I", m_importer.openPtr());
				ImGui::MenuItem(m_profiler.getTitle(),	"Ctrl+Alt+P", m_profiler.openPtr());
				//ImGui::MenuItem(m_manual.getTitle(),	"Ctrl+Alt+M", m_manual	.openPtr());
				ImGui::MenuItem(m_terminal.getTitle(),	"Ctrl+Alt+T", m_terminal.openPtr());

				eventSystem().fireEvent(MainMenuBarEvent(MainMenuBarEvent::Window));
				
				ImGui::EndMenu();
			}

			// Menu -> Help
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					OS::execute("open", ML_PROJECT_URL);
				}
				if (ImGui::MenuItem("Downloads"))
				{
					OS::execute("open", "https://mega.nz/#F!kDIkQQIL!mByWlNs89zlwh9WHi3VUcw");
				}
				if (ImGui::BeginMenu("Third Party Software"))
				{
					if (ImGui::MenuItem("assimp")) OS::execute("open", "https://github.com/assimp/assimp");
					if (ImGui::MenuItem("cpython")) OS::execute("open", "https://github.com/python/cpython");
					if (ImGui::MenuItem("dirent")) OS::execute("open", "https://github.com/tronkko/dirent");
					if (ImGui::MenuItem("flac")) OS::execute("open", "https://github.com/xiph/flac");
					if (ImGui::MenuItem("FreeType")) OS::execute("open", "https://www.freetype.org/");
					if (ImGui::MenuItem("GCEM")) OS::execute("open", "https://github.com/kthohr/gcem");
					if (ImGui::MenuItem("GLEW")) OS::execute("open", "http://glew.sourceforge.net/");
					if (ImGui::MenuItem("GLFW")) OS::execute("open", "https://www.glfw.org/");
					if (ImGui::MenuItem("GLM")) OS::execute("open", "https://glm.g-truc.net/0.9.9/index.html");
					if (ImGui::MenuItem("ImGui")) OS::execute("open", "https://github.com/ocornut/imgui");
					if (ImGui::MenuItem("ImGuiColorTextEdit")) OS::execute("open", "https://github.com/BalazsJako/ImGuiColorTextEdit");
					if (ImGui::MenuItem("INIReader")) OS::execute("open", "https://github.com/benhoyt/inih");
					if (ImGui::MenuItem("Lua")) OS::execute("open", "https://github.com/lua/lua");
					if (ImGui::MenuItem("ogg")) OS::execute("open", "https://github.com/xiph/ogg");
					if (ImGui::MenuItem("OpenAL")) OS::execute("open", "https://www.openal.org/");
					if (ImGui::MenuItem("pdcurses")) OS::execute("open", "https://github.com/wmcbrine/PDCurses");
					if (ImGui::MenuItem("pybind11")) OS::execute("open", "https://github.com/pybind/pybind11");
					if (ImGui::MenuItem("RakNet")) OS::execute("open", "http://www.jenkinssoftware.com/");
					if (ImGui::MenuItem("stb")) OS::execute("open", "https://github.com/nothings/stb");
					if (ImGui::MenuItem("vorbis")) OS::execute("open", "https://github.com/xiph/vorbis");

					ImGui::EndMenu();
				}
				ImGui::Separator();

				ImGui::MenuItem("ImGui Demo", "", &m_show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", "", &m_show_imgui_metrics);
				ImGui::MenuItem("Style Editor", "", &m_show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", "", &m_show_imgui_about);

				eventSystem().fireEvent(MainMenuBarEvent(MainMenuBarEvent::Help));

				ImGui::EndMenu();
			}

			// User Menu Bars
			eventSystem().fireEvent(MainMenuBarEvent(MainMenuBarEvent::User));

			ImGui::EndMainMenuBar();
		}

		/*	Dockspace	*/	if (m_dockspace	.isOpen()) m_dockspace	.onGui(ev);
		/*	Content		*/	if (m_content	.isOpen()) m_content	.onGui(ev);
		/*	Explorer	*/	if (m_explorer	.isOpen()) m_explorer	.onGui(ev);
		/*	Importer	*/	if (m_importer	.isOpen()) m_importer	.onGui(ev);
		/*	Profiler	*/	if (m_profiler	.isOpen()) m_profiler	.onGui(ev);
		/*	Manual		*/	if (m_manual	.isOpen()) m_manual		.onGui(ev);
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
		if (m_redirect_cout) m_terminal.redirect(cout);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}