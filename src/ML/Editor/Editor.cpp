#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGui_Impl.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(EventSystem & eventSystem)
		: EventListener	(eventSystem)
		, m_dockspace	(eventSystem)
		, m_explorer	(eventSystem)
		, m_browser		(eventSystem)
		, m_profiler	(eventSystem)
		, m_resources	(eventSystem)
		, m_terminal	(eventSystem)
	{
		m_dockspace	.setOpen(true);
		m_explorer	.setOpen(false);
		m_browser	.setOpen(false);
		m_profiler	.setOpen(false);
		m_resources	.setOpen(false);
		m_terminal	.setOpen(true);

		eventSystem.addListener(EnterEvent::ID,			this);
		eventSystem.addListener(ExitEvent::ID,			this);
		eventSystem.addListener(BeginGuiEvent::ID,		this);
		eventSystem.addListener(GuiEvent::ID,			this);
		eventSystem.addListener(EndGuiEvent::ID,		this);
		eventSystem.addListener(BuildDockspaceEvent::ID,this);
		eventSystem.addListener(KeyEvent::ID,			this);
		eventSystem.addListener(File_New_Event::ID,		this);
		eventSystem.addListener(File_Open_Event::ID,	this);
		eventSystem.addListener(File_Save_Event::ID,	this);
		eventSystem.addListener(File_Quit_Event::ID,	this);
		eventSystem.addListener(Edit_Undo_Event::ID,	this);
		eventSystem.addListener(Edit_Redo_Event::ID,	this);
		eventSystem.addListener(Edit_Cut_Event::ID,		this);
		eventSystem.addListener(Edit_Copy_Event::ID,	this);
		eventSystem.addListener(Edit_Paste_Event::ID,	this);
	}

	Editor::~Editor() { }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const Event * value)
	{
		switch (*value)
		{
		case EnterEvent::ID:	return onEnter(*value->as<EnterEvent>());
		case ExitEvent::ID:		return onExit(*value->as<ExitEvent>());
		case BeginGuiEvent::ID:	return onBeginGui(*value->as<BeginGuiEvent>());
		case GuiEvent::ID:		return onGui(*value->as<GuiEvent>());
		case EndGuiEvent::ID:	return onEndGui(*value->as<EndGuiEvent>());

			// Build Dockspace
			/* * * * * * * * * * * * * * * * * * * * */
		case BuildDockspaceEvent::ID:
			if (auto ev = value->as<BuildDockspaceEvent>())
			{
				Dockspace & d = ev->dockspace;
				d.dockWindow(m_browser.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_profiler.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_resources.getTitle(), d.getNode(d.RightUp));
				d.dockWindow(m_terminal.getTitle(), d.getNode(d.LeftDn));
			}
			break;

			// Key
			/* * * * * * * * * * * * * * * * * * * * */
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Show Terminal | Ctrl+Alt+T
				if (ev->getPress(KeyCode::T, { 0, 1, 1, 0 })) m_terminal.setOpen(true);

				// Show Browser | Ctrl+Alt+E
				if (ev->getPress(KeyCode::E, { 0, 1, 1, 0 })) m_browser.setOpen(true);

				// Show Profiler | Ctrl+Alt+P
				if (ev->getPress(KeyCode::P, { 0, 1, 1, 0 })) m_profiler.setOpen(true);

				// Show Resources | Ctrl+Alt+R
				if (ev->getPress(KeyCode::R, { 0, 1, 1, 0 })) m_resources.setOpen(true);

				/* * * * * * * * * * * * * * * * * * * * */

				// File -> New | Ctrl+N
				if (ev->isNew()) eventSystem().fireEvent(File_New_Event());

				// File -> Open | Ctrl+O
				if (ev->isOpen()) eventSystem().fireEvent(File_Open_Event());

				// File -> Save | Ctrl+S
				if (ev->isSave()) eventSystem().fireEvent(File_Save_Event());

				/* * * * * * * * * * * * * * * * * * * * */

				// Edit -> Undo	| Ctrl+Z
				if (ev->isUndo()) eventSystem().fireEvent(Edit_Undo_Event());

				// Edit -> Redo	| Ctrl+Y / Ctrl+Shift+Z
				if (ev->isRedo()) eventSystem().fireEvent(Edit_Redo_Event());

				// Edit -> Cut	| Ctrl+X / Shift+Insert
				if (ev->isCut()) eventSystem().fireEvent(Edit_Cut_Event());

				// Edit -> Copy | Ctrl+C / Ctrl+Insert
				if (ev->isCopy()) eventSystem().fireEvent(Edit_Copy_Event());

				// Edit -> Paste | Ctrl+V / Shift+Insert
				if (ev->isPaste()) eventSystem().fireEvent(Edit_Paste_Event());

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;


			// File -> New
			/* * * * * * * * * * * * * * * * * * * * */
		case File_New_Event::ID:
			if (auto ev = value->as<File_New_Event>()) {}
			break;

			// File -> Open
			/* * * * * * * * * * * * * * * * * * * * */
		case File_Open_Event::ID:
			if (auto ev = value->as<File_Open_Event>()) {}
			break;

			// File -> Save
			/* * * * * * * * * * * * * * * * * * * * */
		case File_Save_Event::ID:
			if (auto ev = value->as<File_Save_Event>()) {}
			break;

			// File -> Exit
			/* * * * * * * * * * * * * * * * * * * * */
		case File_Quit_Event::ID:
			if (auto ev = value->as<File_Quit_Event>()) 
				eventSystem().fireEvent(WindowKillEvent());
			break;


			// Edit -> Undo
			/* * * * * * * * * * * * * * * * * * * * */
		case Edit_Undo_Event::ID:
			if (auto ev = value->as<Edit_Undo_Event>()) {}
			break;

			// Edit -> Redo
			/* * * * * * * * * * * * * * * * * * * * */
		case Edit_Redo_Event::ID:
			if (auto ev = value->as<Edit_Redo_Event>()) {}
			break;

			// Edit -> Cut
			/* * * * * * * * * * * * * * * * * * * * */
		case Edit_Cut_Event::ID:
			if (auto ev = value->as<Edit_Cut_Event>()) {}
			break;

			// Edit -> Copy
			/* * * * * * * * * * * * * * * * * * * * */
		case Edit_Copy_Event::ID:
			if (auto ev = value->as<Edit_Copy_Event>()) {}
			break;

			// Edit -> Paste
			/* * * * * * * * * * * * * * * * * * * * */
		case Edit_Paste_Event::ID: 
			if (auto ev = value->as<Edit_Paste_Event>()) {}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEnter(const EnterEvent & ev)
	{
		// Initialize
		/* * * * * * * * * * * * * * * * * * * * */
		ML_ImGui_Impl;

		// Setup Style
		/* * * * * * * * * * * * * * * * * * * * */
		const String styleFile = ev.prefs.GetString(
			"Editor", "styleFile", "Classic"
		);
		if (styleFile == "Classic")  ImGui::StyleColorsClassic();
		else if (styleFile == "Dark") ImGui::StyleColorsDark();
		else if (styleFile == "Light") ImGui::StyleColorsLight();
		else if (!ML_ImGui_Impl.LoadStyle(ML_FS.getPathTo(styleFile)))
		{
			Debug::logError("Failed loading ImGui style");
		}

		// Setup Fonts
		/* * * * * * * * * * * * * * * * * * * * */
		String fontFile = ev.prefs.GetString("Editor", "fontFile", "");
		if (fontFile)
		{
			float_t fontSize = ev.prefs.GetFloat("Editor", "fontSize", 12.0f);
			if (fontSize > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(
					fontFile.c_str(),
					fontSize
				);
			}
		}

		// Setup Ini File
		/* * * * * * * * * * * * * * * * * * * * */
		C_String imguiIni = (ev.prefs.GetBool("Editor", "useImguiIni", false)
			? "imgui.ini"
			: nullptr
		);

		// Startup
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_ImGui_Impl.Startup("#version 410", &ev.window, true, imguiIni))
		{
			Debug::fatal("Failed Initializing ImGui");
		}

		// Capture Cout
		m_terminal.redirect(cout);
	}

	void Editor::onBeginGui(const BeginGuiEvent & ev)
	{
		ML_ImGui_Impl.NewFrame();
		ImGui::NewFrame();
	}

	void Editor::onGui(const GuiEvent & ev)
	{
		// ImGui Demo
		/* * * * * * * * * * * * * * * * * * * * */
		static bool show_imgui_demo		= false;
		static bool show_imgui_metrics	= false;
		static bool show_imgui_style	= false;
		static bool show_imgui_about	= false;

		if (show_imgui_demo)	{ ImGui::ShowDemoWindow(&show_imgui_demo); }
		if (show_imgui_metrics) { ImGui::ShowMetricsWindow(&show_imgui_metrics); }
		if (show_imgui_style)	{ ImGui::Begin("Dear ImGui Style Editor", &show_imgui_style); ImGui::ShowStyleEditor(); ImGui::End(); }
		if (show_imgui_about)	{ ImGui::ShowAboutWindow(&show_imgui_about); }

		// Main Menu Bar
		/* * * * * * * * * * * * * * * * * * * * */
		if (ImGui::BeginMainMenuBar())
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

				eventSystem().fireEvent(MainMenuBarEvent(
					(*this),
					MainMenuBarEvent::File
				));

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

				eventSystem().fireEvent(MainMenuBarEvent(
					(*this),
					MainMenuBarEvent::Edit
				));

				ImGui::EndMenu();
			}

			// Menu -> Window
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(m_browser.getTitle(),	"Ctrl+Alt+E", m_browser.openPtr()	);
				ImGui::MenuItem(m_profiler.getTitle(),	"Ctrl+Alt+P", m_profiler.openPtr()	);
				ImGui::MenuItem(m_resources.getTitle(), "Ctrl+Alt+R", m_resources.openPtr()	);
				ImGui::MenuItem(m_terminal.getTitle(),	"Ctrl+Alt+T", m_terminal.openPtr()	);

				eventSystem().fireEvent(MainMenuBarEvent(
					(*this),
					MainMenuBarEvent::Window
				));
				
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
				if (ImGui::BeginMenu("License"))
				{
					static String preview;
					if (!preview && ML_FS.getFileContents(ML_FS.getPathTo("../../../LICENSE.txt"), preview))
					{
						preview.pop_back();
					}
					ImGui::TextUnformatted(&preview[0], &preview[preview.size()]);
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Third Party Software"))
				{
					if (ImGui::MenuItem("FreeType")) OS::execute("open", "https://www.freetype.org/");
					if (ImGui::MenuItem("GCEM")) OS::execute("open", "https://github.com/kthohr/gcem");
					if (ImGui::MenuItem("GLEW")) OS::execute("open", "http://glew.sourceforge.net/");
					if (ImGui::MenuItem("GLFW")) OS::execute("open", "https://www.glfw.org/");
					if (ImGui::MenuItem("GLM")) OS::execute("open", "https://glm.g-truc.net/0.9.9/index.html");
					if (ImGui::MenuItem("ImGui")) OS::execute("open", "https://github.com/ocornut/imgui");
					if (ImGui::MenuItem("ImGuiColorTextEdit")) OS::execute("open", "https://github.com/BalazsJako/ImGuiColorTextEdit");
					if (ImGui::MenuItem("INIReader")) OS::execute("open", "https://github.com/benhoyt/inih");
					if (ImGui::MenuItem("OpenAL")) OS::execute("open", "https://www.openal.org/");
					if (ImGui::MenuItem("RakNet")) OS::execute("open", "http://www.jenkinssoftware.com/");
					if (ImGui::MenuItem("STB")) OS::execute("open", "https://github.com/nothings/stb");

					ImGui::EndMenu();
				}
				ImGui::Separator();

				ImGui::MenuItem("ImGui Demo", "", &show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", "", &show_imgui_metrics);
				ImGui::MenuItem("Style Editor", "", &show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", "", &show_imgui_about);

				eventSystem().fireEvent(MainMenuBarEvent(
					(*this),
					MainMenuBarEvent::Help
				));

				ImGui::EndMenu();
			}

			eventSystem().fireEvent(MainMenuBarEvent(
				(*this),
				MainMenuBarEvent::None
			));

			ImGui::EndMainMenuBar();
		}

		/* Dockspace */ m_dockspace.onGui(ev);
		/* Explorer */	m_explorer.onGui(ev);
		/* Profiler */	m_profiler.onGui(ev);
		/* Resources */ m_resources.onGui(ev);
		/* Browser */	m_browser.onGui(ev);
		/* Terminal */	m_terminal.onGui(ev);
	}

	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		ImGui::Render();
		ML_ImGui_Impl.Render(ImGui::GetDrawData());
	}

	void Editor::onExit(const ExitEvent & ev)
	{
		// Release Cout
		m_terminal.redirect(cout);

		// Shutdown ImGui
		ML_ImGui_Impl.Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}