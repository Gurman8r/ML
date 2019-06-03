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
		: EventListener	(eventSystem)
		, dockspace		(eventSystem)
		, browser		(eventSystem)
		, builder		(eventSystem)
		, inspector		(eventSystem)
		, profiler		(eventSystem)
		, resources		(eventSystem)
		, scene			(eventSystem)
		, terminal		(eventSystem)
	{
		eventSystem.addListener(EnterEvent::ID, this);
		eventSystem.addListener(ExitEvent::ID, this);

		eventSystem.addListener(BeginGuiEvent::ID, this);
		eventSystem.addListener(GuiEvent::ID, this);
		eventSystem.addListener(EndGuiEvent::ID, this);

		eventSystem.addListener(KeyEvent::ID, this);

		eventSystem.addListener(File_New_Event::ID, this);
		eventSystem.addListener(File_Open_Event::ID, this);
		eventSystem.addListener(File_Save_Event::ID, this);
		eventSystem.addListener(File_Close_Event::ID, this);

		eventSystem.addListener(Edit_Undo_Event::ID, this);
		eventSystem.addListener(Edit_Redo_Event::ID, this);
		eventSystem.addListener(Edit_Cut_Event::ID, this);
		eventSystem.addListener(Edit_Copy_Event::ID, this);
		eventSystem.addListener(Edit_Paste_Event::ID, this);
	}

	Editor::~Editor() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const Event * value)
	{
		switch (*value)
		{
		case EnterEvent::ID:	return onEnter		(*value->as<EnterEvent>());
		case ExitEvent::ID:		return onExit		(*value->as<ExitEvent>());
		case BeginGuiEvent::ID: return onBeginGui	(*value->as<BeginGuiEvent>());
		case GuiEvent::ID:		return onGui		(*value->as<GuiEvent>());
		case EndGuiEvent::ID:	return onEndGui		(*value->as<EndGuiEvent>());

			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
				// Show TerminalGui (Ctrl+Alt+T)
				if (ev->getKeyDown(KeyCode::T) && (ev->mod_ctrl && ev->mod_alt))
					this->terminal.isOpen() = true;

				// Show BrowserGui (Ctrl+Alt+E)
				if (ev->getKeyDown(KeyCode::E) && (ev->mod_ctrl))
					this->browser.isOpen() = true;

				// Show BuilderGui (Ctrl+Alt+B)
				if (ev->getKeyDown(KeyCode::B) && (ev->mod_ctrl && ev->mod_alt))
					this->builder.isOpen() = true;

				// Show Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(KeyCode::S) && (ev->mod_ctrl && ev->mod_alt))
					this->scene.isOpen() = true;

				// Show InspectorGui (Ctrl+Alt+I)
				if (ev->getKeyDown(KeyCode::I) && (ev->mod_ctrl && ev->mod_alt))
					this->inspector.isOpen() = true;
			}
			break;

			// File -> Close Event
			/* * * * * * * * * * * * * * * * * * * * */
		case File_Close_Event::ID:
			if (auto ev = value->as<File_Close_Event>())
			{
				eventSystem().fireEvent(WindowKillEvent());
			}
			break;

			// File -> Open Event
			/* * * * * * * * * * * * * * * * * * * * */
		case File_Open_Event::ID:
			if (auto ev = value->as<File_Open_Event>())
			{
				OS::execute("open", this->browser.get_selected_path());
			}
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
		StyleLoader loader;
		if (!loader.loadFromFile(ML_FS.getPathTo("../../../assets/styles/style4.txt")))
		{
			String font = ev.prefs.GetString("Editor", "imguiFont", "");
			float  size = ev.prefs.GetFloat("Editor", "imguiSize", 12.0f);

			if (font && size > 0.0f)
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), size);
			}
		}
		else if (Debug::log("LOADED IMGUI STYLE"))
		{
			ImGui::GetStyle().FrameBorderSize	= 1.0f;
			ImGui::GetStyle().FramePadding		= { 4.0f, 2.0f };
			ImGui::GetStyle().ItemSpacing		= { 8.0f, 2.0f };
			ImGui::GetStyle().WindowBorderSize	= 1.0f;
			ImGui::GetStyle().TabBorderSize		= 1.0f;
			ImGui::GetStyle().WindowRounding	= 1.0f;
			ImGui::GetStyle().ChildRounding		= 1.0f;
			ImGui::GetStyle().FrameRounding		= 1.0f;
			ImGui::GetStyle().ScrollbarRounding = 1.0f;
			ImGui::GetStyle().GrabRounding		= 1.0f;
			ImGui::GetStyle().TabRounding		= 1.0f;
		}

		// Set ImGui INI
		String imguiINI = ev.prefs.GetString("Editor", "imguiINI", "");
		if(imguiINI) imguiINI = ML_FS.getPathTo(imguiINI);

		// Start ImGui
		if (!ImGui_ML_Init("#version 410", &ev.window, true, imguiINI.c_str()))
		{
			Debug::fatal("Failed Initializing ImGui");
		}

		// Capture Cout
		if (!(m_coutBuf = cout.rdbuf(m_coutStr.rdbuf())))
		{
			Debug::fatal("Failed Capturing Cout");
		}
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
		static bool show_imgui_demo		= false;
		static bool show_imgui_metrics	= false;
		static bool show_imgui_style	= false;
		static bool show_imgui_about	= false;

		if (show_imgui_demo)	{ ImGui::ShowDemoWindow(&show_imgui_demo); }
		if (show_imgui_metrics) { ImGui::ShowMetricsWindow(&show_imgui_metrics); }
		if (show_imgui_style)	{ ImGui::Begin("Style Editor", &show_imgui_style); ImGui::ShowStyleEditor(); ImGui::End(); }
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
				ImGui::Separator();
				// File -> Save
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					eventSystem().fireEvent(File_Save_Event());
				}
				// File -> Save All
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false))
				{
					eventSystem().fireEvent(File_Save_Event());
				}
				ImGui::Separator();
				// File -> Quit
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					eventSystem().fireEvent(File_Close_Event());
				}
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
				ImGui::EndMenu();
			}

			// Menu -> Window
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(this->terminal.getTitle(),	"Ctrl+Alt+T", &this->terminal.isOpen());
				ImGui::MenuItem(this->browser.getTitle(),	"Ctrl+Alt+E", &this->browser.isOpen());
				ImGui::MenuItem(this->builder.getTitle(),	"Ctrl+Alt+B", &this->builder.isOpen());
				ImGui::MenuItem(this->scene.getTitle(), "Ctrl+Alt+S", &this->scene.isOpen());
				ImGui::MenuItem(this->inspector.getTitle(), "Ctrl+Alt+I", &this->inspector.isOpen());
				ImGui::MenuItem(this->profiler.getTitle(),	"", &this->profiler.isOpen());
				ImGui::MenuItem(this->resources.getTitle(),	"", &this->resources.isOpen());
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
				ImGui::Separator();

				ImGui::MenuItem("ImGui Demo",		"", &show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics",	"", &show_imgui_metrics);
				ImGui::MenuItem("Style Editor",		"", &show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", "", &show_imgui_about);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		// DockspaceGui
		/* * * * * * * * * * * * * * * * * * * * */
		this->dockspace.onGui(ev);

		// TerminalGui
		/* * * * * * * * * * * * * * * * * * * * */
		this->terminal.printss(m_coutStr);
		this->terminal.onGui(ev);

		// ProfilerGui
		/* * * * * * * * * * * * * * * * * * * * */
		this->profiler.onGui(ev);

		// BrowserGui
		/* * * * * * * * * * * * * * * * * * * * */
		this->browser.onGui(ev);

		// Project
		/* * * * * * * * * * * * * * * * * * * * */
		this->resources.onGui(ev);

		// BuilderGui
		/* * * * * * * * * * * * * * * * * * * * */
		this->builder.onGui(ev);
	}
	
	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		ImGui::Render();
		ImGui_ML_Render(ImGui::GetDrawData());
	}
	
	void Editor::onExit(const ExitEvent & ev)
	{
		// Release Cout
		if (m_coutBuf) { cout.rdbuf(m_coutBuf); }

		// Shutdown ImGui
		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}