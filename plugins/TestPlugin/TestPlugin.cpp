/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/EngineEvents.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main() { return new ml::TestPlugin {}; }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin()
		: Plugin {}
	{
		ML_EventSystem.addListener<StartEvent>(this);
	}

	void TestPlugin::onEvent(const Event & value)
	{
		switch (*value)
		{
		case StartEvent::ID:
			if (auto ev = value.as<StartEvent>())
			{
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}