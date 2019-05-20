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

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(Engine & engine)
		: m_engine	(engine)
		, browser	(*this, true)
		, builder	(*this, true)
		, dockspace	(*this, true)
		, inspector	(*this, true)
		, mainMenu	(*this, true)
		, network	(*this, false)
		, profiler	(*this, true)
		, project	(*this, true)
		, sceneView	(*this, true)
		, terminal	(*this, true)
		, textEdit	(*this, false)
	{

		eventSystem().addListener(WindowEvent::EV_Key,			this);
		eventSystem().addListener(EngineEvent::EV_Load,			this);
		eventSystem().addListener(EngineEvent::EV_Unload,		this);
		
		eventSystem().addListener(EditorEvent::EV_BeginGui,		this);
		eventSystem().addListener(EditorEvent::EV_DrawGui,		this);
		eventSystem().addListener(EditorEvent::EV_EndGui,		this);

		eventSystem().addListener(EditorEvent::EV_File_New,		this);
		eventSystem().addListener(EditorEvent::EV_File_Open,	this);
		eventSystem().addListener(EditorEvent::EV_File_Save,	this);
		eventSystem().addListener(EditorEvent::EV_File_Close,	this);
		eventSystem().addListener(EditorEvent::EV_Edit_Undo,	this);
		eventSystem().addListener(EditorEvent::EV_Edit_Redo,	this);
		eventSystem().addListener(EditorEvent::EV_Edit_Cut,		this);
		eventSystem().addListener(EditorEvent::EV_Edit_Copy,	this);
		eventSystem().addListener(EditorEvent::EV_Edit_Paste,	this);
	}

	Editor::~Editor() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const IEvent * value)
	{
		switch (*value)
		{
			/* * * * * * * * * * * * * * * * * * * * */

		case EditorEvent::EV_BeginGui:
			this->onBeginGui(*value->as<BeginGuiEvent>());
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case EditorEvent::EV_DrawGui:
			this->onDrawGui(*value->as<DrawGuiEvent>()); 
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case EditorEvent::EV_EndGui: 
			this->onEndGui(*value->as<EndGuiEvent>());
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case EngineEvent::EV_Load:
			if (auto ev = value->as<LoadEvent>())
			{
				if (IMGUI_CHECKVERSION())
				{
					// Create ImGui Context
					ImGui::CreateContext();

					// Load ImGui Custom Fonts
					{
						StyleLoader loader;
						if (!loader.loadFromFile(ML_FS.getPathTo("../../../assets/styles/style4.txt")))
						{
							ImGui::StyleHelper::Style4();
						}

						String imguiFont = ev->engine.prefs().GetString(
							"Editor", "imguiFont", ""
						);

						const float imguiSize = ev->engine.prefs().GetFloat(
							"Editor", "imguiSize", 12.0f
						);

						if (imguiFont && imguiSize > 0.0f)
						{
							ImGui::GetIO().Fonts->AddFontFromFileTTF(imguiFont.c_str(), imguiSize);
						}
					}

					// Set ImGui INI
					String imguiINI = ev->engine.prefs().GetString("Editor", "imguiINI", "");
					imguiINI = imguiINI ? ML_FS.getPathTo(imguiINI) : String();

					// Initialize ImGui
					if (!ImGui_ML_Init("#version 410", &ev->engine.window(), true, imguiINI.c_str()))
					{
						return Debug::fatal("Failed Initializing ImGui");
					}
				}
			}
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case EngineEvent::EV_Unload:
			if (auto ev = value->as<UnloadEvent>())
			{
				// Shutdown ImGui
				ImGui_ML_Shutdown();
			}
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case EditorEvent::EV_File_Close:
			if (auto ev = value->as<File_Close_Event>())
			{
				eventSystem().fireEvent(ShutdownEvent());
			}
			break;

			/* * * * * * * * * * * * * * * * * * * * */

		case WindowEvent::EV_Key:
			if (auto ev = value->as<KeyEvent>())
			{
				// Show Terminal (Ctrl+Alt+T)
				if (ev->getKeyDown(KeyCode::T) && (ev->mod_ctrl && ev->mod_alt))
					terminal.isOpen() = true;
				
				// Show Browser (Ctrl+Alt+E)
				if (ev->getKeyDown(KeyCode::E) && (ev->mod_ctrl))
					browser.isOpen() = true;
				
				// Show Builder (Ctrl+Alt+B)
				if (ev->getKeyDown(KeyCode::B) && (ev->mod_ctrl && ev->mod_alt))
					builder.isOpen() = true;
				
				// Show Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(KeyCode::S) && (ev->mod_ctrl && ev->mod_alt))
					sceneView.isOpen() = true;

				// Show Inspector (Ctrl+Alt+I)
				if (ev->getKeyDown(KeyCode::I) && (ev->mod_ctrl && ev->mod_alt))
					inspector.isOpen() = true;

				// Show ImGui Demo (Ctrl+H)
				if (ev->getKeyDown(KeyCode::H) && (ev->mod_ctrl))
					show_imgui_demo = true;
			}
			break;

			/* * * * * * * * * * * * * * * * * * * * */
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::onBeginGui(const BeginGuiEvent & ev)
	{
		ImGui_ML_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::onDrawGui(const DrawGuiEvent & ev)
	{
		/*	Main Menu	*/ ev.editor.mainMenu.onGui(ev);
		/*	Dockspace	*/ ev.editor.dockspace.onGui(ev);
		/*	Network		*/ ev.editor.network.onGui(ev);
		/*	Profiler	*/ ev.editor.profiler.onGui(ev);
		/*	Browser		*/ ev.editor.browser.onGui(ev);
		/*	Terminal	*/ ev.editor.terminal.onGui(ev);
		/*	Text Editor	*/ ev.editor.textEdit.onGui(ev);
		/*	Project		*/ ev.editor.project.onGui(ev);
		/*	Builder		*/ ev.editor.builder.onGui(ev);

		// ImGui Builtin
		/* * * * * * * * * * * * * * * * * * * * */
		if (show_imgui_demo)	{ ImGui_Builtin::showDemo(&show_imgui_demo); }
		if (show_imgui_metrics) { ImGui_Builtin::showMetrics(&show_imgui_metrics); }
		if (show_imgui_style)	{ ImGui_Builtin::showStyle(&show_imgui_style); }
		if (show_imgui_about)	{ ImGui_Builtin::showAbout(&show_imgui_about); }
	}

	void Editor::onEndGui(const EndGuiEvent & ev)
	{
		ImGui::Render();

		ImGui_ML_Render(ImGui::GetDrawData());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine			& Editor::engine()		const { return m_engine; }
	EventSystem		& Editor::eventSystem() const { return engine().eventSystem(); }
	Preferences		& Editor::prefs()		const { return engine().prefs(); }
	Resources		& Editor::resources()	const { return engine().resources(); }
	RenderWindow	& Editor::window()		const { return engine().window(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}