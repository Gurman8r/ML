#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class	Editor;
	struct	GameTime;
	class	Editor_Dockspace;
	struct	RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class EditorEventType
	{
		MIN_EDITOR_EVENT = Event::EV_EDITOR,

		EV_BeginGui,
		EV_Gui,
		EV_EndGui,
		EV_Dockspace,

		MAX_EDITOR_EVENT
	};

	static_assert(
		(int32_t)EditorEventType::MAX_EDITOR_EVENT < 
		(int32_t)EditorEventType::MIN_EDITOR_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <EditorEventType ID> struct EditorEvent : public T_Event<EditorEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginGuiEvent final : public EditorEvent<EditorEventType::EV_BeginGui>
	{
		constexpr BeginGuiEvent() {}
	};

	struct GuiEvent final : public EditorEvent<EditorEventType::EV_Gui>
	{
		constexpr GuiEvent() {}
	};

	struct EndGuiEvent final : public EditorEvent<EditorEventType::EV_EndGui>
	{
		constexpr EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct DockspaceEvent final : public EditorEvent<EditorEventType::EV_Dockspace>
	{
		Editor_Dockspace & dockspace;
		constexpr DockspaceEvent(Editor_Dockspace & dockspace)
			: dockspace(dockspace)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_