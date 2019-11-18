#include <ML/Editor/Editor_Explorer.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/SizeOf.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Explorer::Editor_Explorer()
		: Editor_Base { "Explorer", "Ctrl+Alt+E", ML_Engine.prefs().get_bool("Editor", "show_explorer", false) }
		, m_browser	{ }
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Explorer::onEvent(const Event & value)
	{
		Editor_Base::onEvent(value);

		switch (*value)
		{
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Editor.mainMenuBar().addMenu("View", [&]()
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::MenuItem(getTitle(), getHotkey(), openPtr());
				ImGui::PopID();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			m_browser.update();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (Editor_Dockspace & d{ ev.dockspace }; d.isOpen())
			{
				d.dockWindow(getTitle(), d.getNode(d.RightUp));
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ev.getPress(KeyCode::E, { { 0, 1, 1, 0 } })) { toggleOpen(); }

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	bool Editor_Explorer::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);

		if (beginDraw(ImGuiWindowFlags_AlwaysAutoResize))
		{
			m_browser.render(
				(getTitle() + "##FileBrowser"_s).c_str(), 
				{ 0, 0 }, 
				true, 
				ImGuiWindowFlags_MenuBar
			);
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}