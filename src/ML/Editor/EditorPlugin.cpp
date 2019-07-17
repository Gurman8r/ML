#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorPlugin::EditorPlugin(EventSystem & eventSystem)
		: Plugin(eventSystem)
	{
		eventSystem.addListener(GuiEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorPlugin::onEvent(const Event * value)
	{
		// Handle base events
		Plugin::onEvent(value);

		switch (*value)
		{
		// Gui Event
		case GuiEvent::ID: return this->onGui(*value->as<GuiEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}