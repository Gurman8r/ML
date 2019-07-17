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
	return new DEMO::TestPlugin(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin(ml::EventSystem & eventSystem)
		: EditorPlugin(eventSystem)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onEvent(const ml::Event * value)
	{
		// Handle base events
		ml::EditorPlugin::onEvent(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onStart(const ml::StartEvent & ev)
	{
		ml::Debug::log("Loaded {0}", (*this));
	}

	void TestPlugin::onUpdate(const ml::UpdateEvent	& ev)
	{
	}

	void TestPlugin::onDraw(const ml::DrawEvent	& ev)
	{
	}

	void TestPlugin::onGui(const ml::GuiEvent & ev)
	{
	}

	void TestPlugin::onExit(const ml::ExitEvent	& ev)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */