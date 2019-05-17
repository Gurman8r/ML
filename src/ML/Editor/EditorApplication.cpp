#include <ML/Editor/EditorApplication.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorApplication::EditorApplication()
		: Application()
	{
		ML_EventSystem.addListener(EditorEvent::EV_Gui, this);
		ML_EventSystem.addListener(EditorEvent::EV_File_New, this);
		ML_EventSystem.addListener(EditorEvent::EV_File_Open, this);
		ML_EventSystem.addListener(EditorEvent::EV_File_Save, this);
		ML_EventSystem.addListener(EditorEvent::EV_File_Close, this);
		ML_EventSystem.addListener(EditorEvent::EV_Edit_Undo, this);
		ML_EventSystem.addListener(EditorEvent::EV_Edit_Redo, this);
		ML_EventSystem.addListener(EditorEvent::EV_Edit_Cut, this);
		ML_EventSystem.addListener(EditorEvent::EV_Edit_Copy, this);
		ML_EventSystem.addListener(EditorEvent::EV_Edit_Paste, this);
	}

	EditorApplication::~EditorApplication() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorApplication::onEvent(const IEvent * value)
	{
		Application::onEvent(value);

		switch (*value)
		{
			// Draw Gui
			/* * * * * * * * * * * * * * * * * * * * */
		case EditorEvent::EV_Gui: 
			if (const auto * ev = value->as<GuiEvent>())
			{
				ImGui_ML_NewFrame();
				ImGui::NewFrame();
				{
					this->onGui(ev);
				}
				ImGui::Render();
				ImGui_ML_Render(ImGui::GetDrawData());
			}
			break;

			// File -> Close ...
			/* * * * * * * * * * * * * * * * * * * * */
		case EditorEvent::EV_File_Close:
			if (const auto * ev = value->as<File_Close_Event>())
			{
				this->close();
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}