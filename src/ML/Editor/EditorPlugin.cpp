#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorPlugin::EditorPlugin(EventSystem & eventSystem)
		: Plugin(eventSystem)
	{
		eventSystem.addListener(EditorEvent::EV_Gui, this);
	}

	EditorPlugin::~EditorPlugin() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorPlugin::onEvent(const Event * value)
	{
		// Handle base events
		Plugin::onEvent(value);

		switch (*value)
		{
			// Gui Event
		case EditorEvent::EV_Gui:
			return this->onGui(*value->as<GuiEvent>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}