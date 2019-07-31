/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::TestPlugin { eventSystem };
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin(EventSystem & eventSystem)
		: EditorPlugin { eventSystem }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onEvent(const Event & value)
	{
		// Handle base events
		EditorPlugin::onEvent(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onStart(const StartEvent & ev)
	{
		Debug::log("Hello from \'{0}\'", (*this));
	}

	void TestPlugin::onUpdate(const UpdateEvent	& ev)
	{
	}

	void TestPlugin::onDraw(const DrawEvent	& ev)
	{
	}

	void TestPlugin::onGui(const GuiEvent & ev)
	{
	}

	void TestPlugin::onExit(const ExitEvent	& ev)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */