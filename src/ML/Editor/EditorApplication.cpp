#include <ML/Editor/EditorApplication.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorApplication::EditorApplication(EventSystem & eventSystem)
		: Application(eventSystem)
	{
		eventSystem.addListener(EditorEvent::EV_DrawGui, this);
	}

	EditorApplication::~EditorApplication() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorApplication::onEvent(const IEvent * value)
	{
		Application::onEvent(value);

		switch (*value)
		{
		case EditorEvent::EV_DrawGui:
			this->onGui(*value->as<DrawGuiEvent>());
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}