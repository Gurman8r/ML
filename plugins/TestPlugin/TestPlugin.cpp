/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Graphics/RenderWindow.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::TestPlugin { eventSystem };
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin(EventSystem & eventSystem)
		: Plugin { eventSystem }
	{
		eventSystem.addListener(StartEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onEvent(const Event & value)
	{
		switch (*value)
		{
		case StartEvent::ID:
			if (auto ev = value.as<StartEvent>())
			{
				Debug::log("Hello from TestPlugin!");
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}