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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		TestPlugin() : Plugin {}
		{
			ML_EventSystem.addListener<LoadEvent>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override
		{
			switch (*value)
			{
			case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ML_Editor.mainMenuBar()
					.addMenu("Plugins", [&]()
					{
						ImGui::PushID(ML_ADDRESSOF(this));
						if (ImGui::BeginMenu("Test Plugin"))
						{
							ImGui::Text("Sample text.");
							ImGui::EndMenu();
						}
						ImGui::PopID();
					});

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::TestPlugin{};
}