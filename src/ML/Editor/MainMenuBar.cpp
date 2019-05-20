#include <ML/Editor/MainMenuBar.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar(Editor & editor, bool open)
		: BaseWidget("Main Menu Bar", editor, open)
	{
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::drawGui(const DrawGuiEvent & ev)
	{
		if (beginDraw())
		{
			// File Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					eventSystem().fireEvent(File_New_Event());
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					eventSystem().fireEvent(File_Open_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					eventSystem().fireEvent(File_Save_Event(false));
				}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false))
				{
					eventSystem().fireEvent(File_Save_Event(true));
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					eventSystem().fireEvent(File_Close_Event());
				}
				ImGui::EndMenu();
			}

			// Edit Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
					eventSystem().fireEvent(Edit_Undo_Event());
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
					eventSystem().fireEvent(Edit_Redo_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X"))
				{
					eventSystem().fireEvent(Edit_Cut_Event());
				}
				if (ImGui::MenuItem("Copy", "Ctrl+C"))
				{
					eventSystem().fireEvent(Edit_Copy_Event());
				}
				if (ImGui::MenuItem("Paste", "Ctrl+V"))
				{
					eventSystem().fireEvent(Edit_Paste_Event());
				}
				ImGui::EndMenu();
			}

			// Window Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(ev.editor.terminal.getTitle(),	"Ctrl+Alt+T", &ev.editor.terminal.isOpen()	);
				ImGui::MenuItem(ev.editor.browser.getTitle(),	"Ctrl+Alt+E", &ev.editor.browser.isOpen()	);
				ImGui::MenuItem(ev.editor.builder.getTitle(),	"Ctrl+Alt+B", &ev.editor.builder.isOpen()	);
				ImGui::MenuItem(ev.editor.sceneView.getTitle(),	"Ctrl+Alt+S", &ev.editor.sceneView.isOpen()	);
				ImGui::MenuItem(ev.editor.inspector.getTitle(),	"Ctrl+Alt+I", &ev.editor.inspector.isOpen()	);
				ImGui::MenuItem(ev.editor.profiler.getTitle(),	"", &ev.editor.profiler.isOpen()	);
				ImGui::MenuItem(ev.editor.textEdit.getTitle(),	"", &ev.editor.textEdit.isOpen()	);
				ImGui::MenuItem(ev.editor.project.getTitle(),	"", &ev.editor.project.isOpen()	);
				ImGui::MenuItem(ev.editor.network.getTitle(),	"", &ev.editor.network.isOpen()	);
				ImGui::EndMenu();
			}

			// Help Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					OS::execute("open", ML_PROJECT_URL);
				}
				ImGui::Separator();

				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &ev.editor.show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", "", &ev.editor.show_imgui_metrics);
				ImGui::MenuItem("ImGui Style Editor", "", &ev.editor.show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", "", &ev.editor.show_imgui_about);
				ImGui::EndMenu();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::beginDraw(int32_t flags)
	{
		//if (!isOpen()) return false;

		return m_good = ImGui::BeginMainMenuBar();
	}

	bool MainMenuBar::endDraw()
	{
		//if (!isOpen()) return false;

		ImGui::EndMainMenuBar();

		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}