#include <ML/Editor/MainMenuBar.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/CoreEvents.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar()
		: BaseWidget("Main Menu Bar")
	{
		ML_OS; // initialize OS singleton
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void MainMenuBar::onEvent(const IEvent * value)
	{
	}

	bool MainMenuBar::drawGui(const GuiEvent * ev, bool * p_open)
	{
		if (beginDraw(p_open, 0))
		{
			// File Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					ML_EventSystem.fireEvent(File_New_Event());
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					ML_EventSystem.fireEvent(File_Open_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					ML_EventSystem.fireEvent(File_Save_Event(false));
				}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false))
				{
					ML_EventSystem.fireEvent(File_Save_Event(true));
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					ML_EventSystem.fireEvent(File_Close_Event());
				}
				ImGui::EndMenu();
			}

			// Edit Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
					ML_EventSystem.fireEvent(Edit_Undo_Event());
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
					ML_EventSystem.fireEvent(Edit_Redo_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X"))
				{
					ML_EventSystem.fireEvent(Edit_Cut_Event());
				}
				if (ImGui::MenuItem("Copy", "Ctrl+C"))
				{
					ML_EventSystem.fireEvent(Edit_Copy_Event());
				}
				if (ImGui::MenuItem("Paste", "Ctrl+V"))
				{
					ML_EventSystem.fireEvent(Edit_Paste_Event());
				}
				ImGui::EndMenu();
			}

			// Window Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(ML_Editor.terminal.title(), "Ctrl+Alt+T", &ML_Editor.show_terminal);
				ImGui::MenuItem(ML_Editor.browser.title(), "Ctrl+Alt+E", &ML_Editor.show_browser);
				ImGui::MenuItem(ML_Editor.builder.title(), "Ctrl+Alt+B", &ML_Editor.show_builder);
				ImGui::MenuItem(ML_Editor.sceneView.title(), "Ctrl+Alt+S", &ML_Editor.show_sceneView);
				ImGui::MenuItem(ML_Editor.inspector.title(), "Ctrl+Alt+I", &ML_Editor.show_inspector);
				ImGui::MenuItem(ML_Editor.profiler.title(), NULL, &ML_Editor.show_profiler);
				ImGui::MenuItem(ML_Editor.textEditor.title(), NULL, &ML_Editor.show_textEditor);
				ImGui::MenuItem(ML_Editor.resourceView.title(), NULL, &ML_Editor.show_resourceView);
				ImGui::MenuItem(ML_Editor.networkHUD.title(), NULL, &ML_Editor.show_network);
				ImGui::EndMenu();
			}

			// Help Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_PROJECT_URL));
				}
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &ML_Editor.show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &ML_Editor.show_imgui_metrics);
				ImGui::MenuItem("ImGui Style Editor", NULL, &ML_Editor.show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &ML_Editor.show_imgui_about);
				ImGui::EndMenu();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::beginDraw(bool * p_open, int32_t flags)
	{
		return goodCheck(
			((!p_open) || (*p_open)) &&
			//((p_open) && (*p_open)) &&
			ImGui::BeginMainMenuBar()
		);
	}

	bool MainMenuBar::endDraw()
	{
		ImGui::EndMainMenuBar();
		return good();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}