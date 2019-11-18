#include <ML/Editor/Editor_Manual.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Manual::Editor_Manual()
		: Editor_Base { "Manual", "Ctrl+Alt+M", ML_Engine.prefs().get_bool("Editor", "show_manual", false) }
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Manual::onEvent(const Event & value)
	{
		Editor_Base::onEvent(value);

		switch (*value)
		{
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			//ML_Editor.mainMenuBar().addMenu("View", [&]()
			//{
			//	ImGui::PushID(ML_ADDRESSOF(this));
			//	ImGui::MenuItem(getTitle(), getHotkey(), openPtr());
			//	ImGui::PopID();
			//});

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev{ value.as<KeyEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ev.getPress(KeyCode::M, { { 0, 1, 1, 0 } })) { toggleOpen(); }

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	bool Editor_Manual::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

		if (beginDraw(0))
		{
			ImGui::Columns(2);
			ImGui::Text("Categories"); ImGui::NextColumn();
			ImGui::Text("Content");
			ImGui::Columns(1);
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}