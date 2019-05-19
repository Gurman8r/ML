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
		: m_engine		(engine)
		, browser		(*this)
		, builder		(*this)
		, dockspace		(*this)
		, inspector		(*this)
		, mainMenuBar	(*this)
		, networkHUD	(*this)
		, profiler		(*this)
		, resourceView	(*this)
		, sceneView		(*this)
		, terminal		(*this)
		, textEditor	(*this)
	{
	}

	Editor::~Editor()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Engine & Editor::engine() const
	{
		return m_engine;
	}

	EventSystem & Editor::eventSystem() const
	{
		return engine().eventSystem();
	}
	
	Preferences & Editor::prefs() const
	{
		return engine().prefs();
	}

	Resources & Editor::resources() const
	{
		return engine().resources();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}