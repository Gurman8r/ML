#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/StyleLoader.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(EventSystem & eventSystem)
		: EventListener(eventSystem)
		, m_gui()
	{
		create<DockspaceGui>(String(), eventSystem)->setOpen(true);
		create<BrowserGui>	(String(), eventSystem)->setOpen(false);
		create<BuilderGui>	(String(), eventSystem)->setOpen(false);
		create<ProfilerGui>	(String(), eventSystem)->setOpen(false);
		create<ResourceGui>	(String(), eventSystem)->setOpen(false);
		create<TerminalGui>	(String(), eventSystem)->setOpen(true);

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

	Editor::~Editor() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor::dispose()
	{
		for (auto & type_pair : m_gui)
		{
			for (auto & gui_pair : type_pair.second)
			{
				delete gui_pair.second;
				gui_pair.second = nullptr;
			}
			type_pair.second.clear();
		}
		m_gui.clear();
		return m_gui.empty();
	}

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
				DockspaceGui & d = ev->dockspace;
				d.dockWindow(get<ProfilerGui>	()->getTitle(), d.getNode(d.RightUp));
				d.dockWindow(get<ResourceGui>	()->getTitle(), d.getNode(d.RightUp));
				d.dockWindow(get<BuilderGui>	()->getTitle(), d.getNode(d.RightUp));
				d.dockWindow(get<BrowserGui>	()->getTitle(), d.getNode(d.RightUp));
				d.dockWindow(get<TerminalGui>	()->getTitle(), d.getNode(d.RightDn));
			}
			break;

			// Key
			/* * * * * * * * * * * * * * * * * * * * */
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Show Terminal | Ctrl+Alt+T
				if (ev->getPress(KeyCode::T, { 0, 1, 1, 0 })) get<TerminalGui>()->setOpen(true);

				// Show Browser | Ctrl+Alt+E
				if (ev->getPress(KeyCode::E, { 0, 1, 1, 0 })) get<BrowserGui>()->setOpen(true);

				// Show Builder | Ctrl+Alt+B)
				if (ev->getPress(KeyCode::B, { 0, 1, 1, 0 })) get<BuilderGui>()->setOpen(true);

				// Show Profiler | Ctrl+Alt+P
				if (ev->getPress(KeyCode::P, { 0, 1, 1, 0 })) get<ProfilerGui>()->setOpen(true);

				// Show Resources | Ctrl+Alt+R
				if (ev->getPress(KeyCode::R, { 0, 1, 1, 0 })) get<ResourceGui>()->setOpen(true);

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
			if (auto ev = value->as<File_Open_Event>())
				ML_OS.execute("open", get<BrowserGui>()->get_selected_path());
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
		// Initialize ImGui
		IMGUI_CHECKVERSION();

		// Create ImGui Context
		ImGui::CreateContext();

		// Load ImGui Style
		const String styleConf = ev.prefs.GetString(
			"Editor",
			"styleConfig",
			"Classic"
		);

		if (styleConf == "Classic")  ImGui::StyleColorsClassic();
		else if (styleConf == "Dark") ImGui::StyleColorsDark();
		else if (styleConf == "Light") ImGui::StyleColorsLight();
		else if (StyleLoader loader = StyleLoader(ML_FS.getPathTo(styleConf)))
		{
			// should be data driven
			ImGui::GetStyle().FrameBorderSize = 1.0f;
			ImGui::GetStyle().FramePadding = { 4.0f, 4.0f };
			ImGui::GetStyle().ItemSpacing = { 8.0f, 2.0f };
			ImGui::GetStyle().WindowBorderSize = 1.0f;
			ImGui::GetStyle().TabBorderSize = 1.0f;
			ImGui::GetStyle().WindowRounding = 1.0f;
			ImGui::GetStyle().ChildRounding = 1.0f;
			ImGui::GetStyle().FrameRounding = 1.0f;
			ImGui::GetStyle().ScrollbarRounding = 1.0f;
			ImGui::GetStyle().GrabRounding = 1.0f;
			ImGui::GetStyle().TabRounding = 1.0f;
		}

		// Custom Font
		String font = ev.prefs.GetString("Editor", "imguiFont", "");
		float  size = ev.prefs.GetFloat("Editor", "imguiSize", 12.0f);
		if (font && size > 0.0f)
		{
			ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), size);
		}

		// Set ImGui INI
		String imguiINI = ev.prefs.GetString("Editor", "imguiINI", "");

		// Start ImGui
		if (!ImGui_ML_Init("#version 410", &ev.window, true, nullptr))
		{
			Debug::fatal("Failed Initializing ImGui");
		}

		// Capture Cout
		get<TerminalGui>()->redirect(cout);
	}

	void Editor::onBeginGui(const BeginGuiEvent & ev)
	{
		ImGui_ML_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::onGui(const GuiEvent & ev)
	{
		// ImGui Demo
		/* * * * * * * * * * * * * * * * * * * * */
		static bool show_imgui_demo = false;
		static bool show_imgui_metrics = false;
		static bool show_imgui_style = false;
		static bool show_imgui_about = false;

		if (show_imgui_demo) { ImGui::ShowDemoWindow(&show_imgui_demo); }
		if (show_imgui_metrics) { ImGui::ShowMetricsWindow(&show_imgui_metrics); }
		if (show_imgui_style) { ImGui::Begin("Style Editor", &show_imgui_style); ImGui::ShowStyleEditor(); ImGui::End(); }
		if (show_imgui_about) { ImGui::ShowAboutWindow(&show_imgui_about); }


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
				ImGui::MenuItem(get<TerminalGui>()->getTitle(), "Ctrl+Alt+T", get<TerminalGui>()->openPtr());
				ImGui::MenuItem(get<BrowserGui> ()->getTitle(), "Ctrl+Alt+E", get<BrowserGui> ()->openPtr());
				ImGui::MenuItem(get<BuilderGui> ()->getTitle(), "Ctrl+Alt+B", get<BuilderGui> ()->openPtr());
				ImGui::MenuItem(get<ProfilerGui>()->getTitle(), "Ctrl+Alt+P", get<ProfilerGui>()->openPtr());
				ImGui::MenuItem(get<ResourceGui>()->getTitle(), "Ctrl+Alt+R", get<ResourceGui>()->openPtr());

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
					ML_OS.execute("open", ML_PROJECT_URL);
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

		// DockspaceGui
		/* * * * * * * * * * * * * * * * * * * * */
		get<DockspaceGui>()->onGui(ev);

		// ProfilerGui
		/* * * * * * * * * * * * * * * * * * * * */
		get<ProfilerGui>()->onGui(ev);

		// Project
		/* * * * * * * * * * * * * * * * * * * * */
		get<ResourceGui>()->onGui(ev);

		// BuilderGui
		/* * * * * * * * * * * * * * * * * * * * */
		get<BuilderGui>()->onGui(ev);

		// BrowserGui
		/* * * * * * * * * * * * * * * * * * * * */
		get<BrowserGui>()->onGui(ev);

		// TerminalGui
		/* * * * * * * * * * * * * * * * * * * * */
		get<TerminalGui>()->onGui(ev);
	}

	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		ImGui::Render();
		ImGui_ML_Render(ImGui::GetDrawData());
	}

	void Editor::onExit(const ExitEvent & ev)
	{
		// Release Cout
		get<TerminalGui>()->redirect(cout);

		// Shutdown ImGui
		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}