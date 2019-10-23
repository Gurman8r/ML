#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class	Editor;
	struct	GameTime;
	class	EditorDockspace;
	struct	RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class EditorEvents
	{
		MIN_EDITOR_EVENT = Event::EV_EDITOR,

		EV_BeginGui,
		EV_Gui,
		EV_EndGui,
		EV_MainMenuBar,
		EV_Dockspace,

		MAX_EDITOR_EVENT
	};

	static_assert(
		(int32_t)EditorEvents::MAX_EDITOR_EVENT < 
		(int32_t)EditorEvents::MIN_EDITOR_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <EditorEvents ID> struct EditorEvent : public T_Event<EditorEvents, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginGuiEvent final : public EditorEvent<EditorEvents::EV_BeginGui>
	{
		constexpr BeginGuiEvent() {}
	};

	struct GuiEvent final : public EditorEvent<EditorEvents::EV_Gui>
	{
		constexpr GuiEvent() {}
	};

	struct EndGuiEvent final : public EditorEvent<EditorEvents::EV_EndGui>
	{
		constexpr EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MainMenuBarEvent final : public EditorEvent<EditorEvents::EV_MainMenuBar>
	{
		enum Menu : int32_t
		{ 
			File, Edit, Window, Help, User,
			MAX_MAIN_MENU_BAR_MENU
		};

		const Menu submenu;
		constexpr MainMenuBarEvent(const Menu submenu) 
			: submenu(submenu)
		{
		}
	};

	struct DockspaceEvent final : public EditorEvent<EditorEvents::EV_Dockspace>
	{
		EditorDockspace & dockspace;
		constexpr DockspaceEvent(EditorDockspace & dockspace)
			: dockspace(dockspace)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_