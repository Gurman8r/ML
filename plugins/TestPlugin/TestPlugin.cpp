/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Resources.hpp>
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

	TestPlugin::~TestPlugin() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onEvent(const ml::Event * value)
	{
		// Handle base events
		ml::EditorPlugin::onEvent(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TestPlugin::onEnter(const ml::EnterEvent & ev)
	{
		// Hello!
		ml::Debug::log("Hello from {0}!", (*this));
	}

	void TestPlugin::onStart(const ml::StartEvent & ev)
	{
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