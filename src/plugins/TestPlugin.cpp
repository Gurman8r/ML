/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Plugin.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API TestPlugin final : public Plugin
	{
		TestPlugin() : Plugin {}
		{
			ML_EventSystem.addListener<MainMenuBarEvent>(this);
		}

		~TestPlugin() {}

		void onEvent(const Event & value) override
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
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

extern "C" ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main() 
{ 
	return new ml::TestPlugin {}; 
}