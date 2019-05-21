#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/StyleLoader.hpp>
#include <ML/Editor/ImGui_Style.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Engine.hpp>
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
		: m_eventSystem	(eventSystem)
		, dockspace		(true)
		, browser		(true)
		, builder		(true)
		, inspector		(true)
		, profiler		(true)
		, project		(true)
		, sceneView		(true)
		, terminal		(true, eventSystem)
	{
		eventSystem.addListener(WindowEvent::EV_Key,		this);
		eventSystem.addListener(EngineEvent::EV_Enter,		this);
		eventSystem.addListener(EngineEvent::EV_Exit,		this);
		
		eventSystem.addListener(EditorEvent::EV_BeginGui,	this);
		eventSystem.addListener(EditorEvent::EV_Gui,		this);
		eventSystem.addListener(EditorEvent::EV_EndGui,		this);

		eventSystem.addListener(EditorEvent::EV_File_New,	this);
		eventSystem.addListener(EditorEvent::EV_File_Open,	this);
		eventSystem.addListener(EditorEvent::EV_File_Save,	this);
		eventSystem.addListener(EditorEvent::EV_File_Close,	this);

		eventSystem.addListener(EditorEvent::EV_Edit_Undo,	this);
		eventSystem.addListener(EditorEvent::EV_Edit_Redo,	this);
		eventSystem.addListener(EditorEvent::EV_Edit_Cut,	this);
		eventSystem.addListener(EditorEvent::EV_Edit_Copy,	this);
		eventSystem.addListener(EditorEvent::EV_Edit_Paste,	this);
	}

	Editor::~Editor() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const IEvent * value)
	{
		switch (*value)
		{
			// Enter Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Enter:
			if (auto ev = value->as<EnterEvent>())
			{
				if (IMGUI_CHECKVERSION())
				{
					// Create ImGui Context
					ImGui::CreateContext();

					// Load ImGui Style
					StyleLoader loader;
					if (!loader.loadFromFile(ML_FS.getPathTo("../../../assets/styles/style4.txt")))
					{
						ImGui::StyleHelper::Style4();

						String font = ev->prefs.GetString("Editor", "imguiFont", "");
						float  size = ev->prefs.GetFloat("Editor", "imguiSize", 12.0f);

						if (font && size > 0.0f)
						{
							ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), size);
						}
					}

					// Set ImGui INI
					String imguiINI = ev->prefs.GetString("Editor", "imguiINI", "");
					imguiINI = imguiINI ? ML_FS.getPathTo(imguiINI) : String();

					// Initialize ImGui
					if (!ImGui_ML_Init("#version 410", &ev->window, true, imguiINI.c_str()))
					{
						Debug::fatal("Failed Initializing ImGui");
					}

					// Capture Cout
					if (!(m_coutBuf = cout.rdbuf(m_coutStr.rdbuf())))
					{
						Debug::fatal("Failed Capturing Cout");
					}
				}
			}
			break;

			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EngineEvent::EV_Exit:
			if (auto ev = value->as<ExitEvent>())
			{
				// Release Cout
				if (m_coutBuf) { cout.rdbuf(m_coutBuf); }

				// Shutdown ImGui
				ImGui_ML_Shutdown();
			}
			break;


			// Gui Events
			/* * * * * * * * * * * * * * * * * * * * */
		case EditorEvent::EV_BeginGui:
			ImGui_ML_NewFrame();
			ImGui::NewFrame();
			break;

		case EditorEvent::EV_Gui:
			this->onGui(*value->as<GuiEvent>());
			break;

		case EditorEvent::EV_EndGui:
			ImGui::Render();
			ImGui_ML_Render(ImGui::GetDrawData());
			break;

			// File -> Close Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EditorEvent::EV_File_Close:
			if (auto ev = value->as<File_Close_Event>())
			{
				eventSystem().fireEvent(ShutdownEvent());
			}
			break;

			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case WindowEvent::EV_Key:
			if (auto ev = value->as<KeyEvent>())
			{
				// Show Terminal (Ctrl+Alt+T)
				if (ev->getKeyDown(KeyCode::T) && (ev->mod_ctrl && ev->mod_alt))
					this->terminal.isOpen() = true;

				// Show Browser (Ctrl+Alt+E)
				if (ev->getKeyDown(KeyCode::E) && (ev->mod_ctrl))
					this->browser.isOpen() = true;

				// Show Builder (Ctrl+Alt+B)
				if (ev->getKeyDown(KeyCode::B) && (ev->mod_ctrl && ev->mod_alt))
					this->builder.isOpen() = true;

				// Show Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(KeyCode::S) && (ev->mod_ctrl && ev->mod_alt))
					this->sceneView.isOpen() = true;

				// Show Inspector (Ctrl+Alt+I)
				if (ev->getKeyDown(KeyCode::I) && (ev->mod_ctrl && ev->mod_alt))
					this->inspector.isOpen() = true;
			}
			break;

			/* * * * * * * * * * * * * * * * * * * * */
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onGui(const GuiEvent & ev)
	{
		// Demo Toggles
		static bool show_imgui_demo		= false;
		static bool show_imgui_metrics	= false;
		static bool show_imgui_style	= false;
		static bool show_imgui_about	= false;

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
				ImGui::MenuItem(this->sceneView.getTitle(), "Ctrl+Alt+S", &this->sceneView.isOpen());
				ImGui::MenuItem(this->inspector.getTitle(), "Ctrl+Alt+I", &this->inspector.isOpen());
				ImGui::MenuItem(this->profiler.getTitle(),	"", &this->profiler.isOpen());
				ImGui::MenuItem(this->project.getTitle(),	"", &this->project.isOpen());
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

		// ImGui Demo
		/* * * * * * * * * * * * * * * * * * * * */
		if (show_imgui_demo)	{ ImGui_Builtin::showDemo	(&show_imgui_demo);		}
		if (show_imgui_metrics) { ImGui_Builtin::showMetrics(&show_imgui_metrics);	}
		if (show_imgui_style)	{ ImGui_Builtin::showStyle	(&show_imgui_style);	}
		if (show_imgui_about)	{ ImGui_Builtin::showAbout	(&show_imgui_about);	}

		// Dockspace
		this->dockspace.onGui(ev);

		// Terminal
		this->terminal.printss(m_coutStr);
		this->terminal.onGui(ev);

		// Profiler
		this->profiler.onGui(ev);

		// Browser
		this->browser.onGui(ev);

		// Project
		this->project.onGui(ev);

		// Builder
		this->builder.onGui(ev);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventSystem	& Editor::eventSystem() const { return m_eventSystem; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}