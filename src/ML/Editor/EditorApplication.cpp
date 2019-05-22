#include <ML/Editor/EditorApplication.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorApplication::EditorApplication(EventSystem & eventSystem)
		: Application(eventSystem)
	{
		eventSystem.addListener(EditorEvent::EV_Gui, this);
	}

	EditorApplication::~EditorApplication() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorApplication::onEvent(const Event * value)
	{
		// Handle base events
		Application::onEvent(value);

		switch (*value)
		{
			// Gui Event
		case EditorEvent::EV_Gui:
			return this->onGui(*value->as<GuiEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}