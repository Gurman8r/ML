#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorPlugin::EditorPlugin(EventSystem & eventSystem)
		: Plugin { eventSystem }
	{
		eventSystem.addListener(GuiEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorPlugin::onEvent(const Event & value)
	{
		// Handle base events
		Plugin::onEvent(value);

		switch (*value)
		{
		case GuiEvent::ID: 
			if (auto ev = value.as<GuiEvent>()) return this->onGui(*ev);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}