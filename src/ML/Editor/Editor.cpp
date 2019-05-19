#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/GUI.hpp>
#include <ML/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Editor::Editor(EventSystem & eventSystem)
		: browser		(eventSystem)
		, builder		(eventSystem)
		, dockspace		(eventSystem)
		, inspector		(eventSystem)
		, mainMenuBar	(eventSystem)
		, networkHUD	(eventSystem)
		, profiler		(eventSystem)
		, resourceView	(eventSystem)
		, sceneView		(eventSystem)
		, terminal		(eventSystem)
		, textEditor	(eventSystem)
	{
	}

	Editor::~Editor()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}