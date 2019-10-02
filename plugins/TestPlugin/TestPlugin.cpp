/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Graphics/RenderWindow.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::TestPlugin {};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin()
		: Plugin {}
	{
		ML_EventSystem.addListener(StartEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}