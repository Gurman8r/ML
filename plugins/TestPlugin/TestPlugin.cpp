/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main() { return new ml::TestPlugin {}; }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin()
		: Plugin {}
	{
		ML_EventSystem.addListener<MainMenuBarEvent>(this);
	}

	void TestPlugin::onEvent(const Event & value)
	{
		switch (*value)
		{
		case MainMenuBarEvent::ID:
			if (auto ev = value.as<MainMenuBarEvent>())
			{
				switch (ev->submenu)
				{
				case MainMenuBarEvent::Plugins:
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu(nameof<>::filter_namespace(get_type_info().name()).c_str()))
					{
						ImGui::Text("Sample text.");
						ImGui::EndMenu();
					}
					ImGui::PopID();
					break;
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}