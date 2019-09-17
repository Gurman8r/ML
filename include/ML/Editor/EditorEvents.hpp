#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/I_Event.hpp>

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
		const GameTime & time;
		RenderWindow & window;
		Editor & editor;
		constexpr BeginGuiEvent(const GameTime & time, RenderWindow & window, Editor & editor)
			: time	(time)
			, window(window)
			, editor(editor)
		{
		}
	};

	struct GuiEvent final : public I_Event<EditorEvent::EV_Gui>
	{
		const GameTime & time;
		RenderWindow & window;
		Editor & editor;
		constexpr GuiEvent(const GameTime & time, RenderWindow & window, Editor & editor)
			: time(time)
			, window(window)
			, editor(editor)
		{
		}
	};

	struct EndGuiEvent final : public I_Event<EditorEvent::EV_EndGui>
	{
		const GameTime & time;
		RenderWindow & window;
		Editor & editor;
		constexpr EndGuiEvent(const GameTime & time, RenderWindow & window, Editor & editor)
			: time(time)
			, window(window)
			, editor(editor)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MainMenuBarEvent final : public I_Event<EditorEvent::EV_MainMenuBar>
	{
		enum Menu : int32_t
		{ 
			File, Edit, Window, Help,
			User,
			MAX_MAIN_MENU_BAR_MENU
		};

		Editor & editor;
		const Menu submenu;
		constexpr MainMenuBarEvent(Editor & editor, const Menu submenu)
			: editor(editor)
			, submenu(submenu)
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