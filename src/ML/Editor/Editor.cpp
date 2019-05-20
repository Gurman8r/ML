#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(Engine & engine)
		: m_engine	(engine)
		, browser	(*this, true)
		, builder	(*this, true)
		, dockspace	(*this, true)
		, inspector	(*this, false)
		, mainMenu	(*this, true)
		, network	(*this, false)
		, profiler	(*this, true)
		, project	(*this, true)
		, sceneView	(*this, true)
		, terminal	(*this, true)
		, textEditor(*this, false)
	{
		engine.eventSystem().addListener(EditorEvent::EV_Gui, this);
	}

	Editor::~Editor() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Editor::onEvent(const IEvent * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Engine		& Editor::engine()		const { return m_engine; }
	EventSystem & Editor::eventSystem() const { return engine().eventSystem(); }
	Preferences & Editor::prefs()		const { return engine().prefs(); }
	Resources	& Editor::resources()	const { return engine().resources(); }

	/* * * * * * * * * * * * * * * * * * * * */
}