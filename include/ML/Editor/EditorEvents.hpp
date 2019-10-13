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

	struct EditorEvent final
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = Event::EV_EDITOR,

			EV_BeginGui,
			EV_Gui,
			EV_EndGui,
			EV_MainMenuBar,
			EV_Dockspace,

			MAX_EDITOR_EVENT
		};

		static_assert(MAX_EDITOR_EVENT < (MIN_EDITOR_EVENT + Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginGuiEvent final : public I_Event<EditorEvent::EV_BeginGui>
	{
		constexpr BeginGuiEvent() {}
	};

	struct GuiEvent final : public I_Event<EditorEvent::EV_Gui>
	{
		constexpr GuiEvent() {}
	};

	struct EndGuiEvent final : public I_Event<EditorEvent::EV_EndGui>
	{
		constexpr EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MainMenuBarEvent final : public I_Event<EditorEvent::EV_MainMenuBar>
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

	struct DockspaceEvent final : public I_Event<EditorEvent::EV_Dockspace>
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