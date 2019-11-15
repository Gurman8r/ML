/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Plugin.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/Editor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API TestPlugin final : public Plugin
	{
		TestPlugin() : Plugin {}
		{
			ML_EventSystem.addListener<StartEvent>(this);
		}

		~TestPlugin() {}

		void onEvent(const Event & value) override
		{
			switch (*value)
			{
				case StartEvent::ID: if (auto ev{ value.as<StartEvent>() })
				{
					ML_Editor.mainMenuBar().addMenu("Plugins", [&]() 
					{
						ImGui::PushID(ML_ADDRESSOF(this));
						if (ImGui::BeginMenu("Test Plugin"))
						{
							ImGui::Text("Sample text.");
							ImGui::EndMenu();
						}
						ImGui::PopID();
					});
				} break;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

extern "C"
{
	ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main()
	{
		return new ml::TestPlugin{};
	}
}