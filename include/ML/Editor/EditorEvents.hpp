#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class	Editor;
	struct	GameTime;
	class	Dockspace;
	struct	RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EditorEvent final
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = Event::EV_EDITOR,

			EV_BeginGui,
			EV_Gui,
			EV_EndGui,

			EV_MainMenuBar,
			EV_BuildDockspace,

			EV_File_New,
			EV_File_Open,
			EV_File_Save,
			EV_File_Quit,

			EV_Edit_Undo,
			EV_Edit_Redo,
			EV_Edit_Cut,
			EV_Edit_Copy,
			EV_Edit_Paste,

			MAX_EDITOR_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MainMenuBarEvent final : public I_Event<EditorEvent::EV_MainMenuBar>
	{
		enum Menu : int32_t
		{ 
			File, Edit, Window, Help,
			User,
			MAX_MAIN_MENU_BAR_MENU
		};

		const Menu menu;
		constexpr MainMenuBarEvent(const Menu menu)
			: menu(menu)
		{
		}
	};

	struct BuildDockspaceEvent final : public I_Event<EditorEvent::EV_BuildDockspace>
	{
		Dockspace & dockspace;
		constexpr BuildDockspaceEvent(Dockspace & dockspace)
			: dockspace(dockspace)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct File_New_Event final : public I_Event<EditorEvent::EV_File_New>
	{
		constexpr File_New_Event() {}
	};

	struct File_Open_Event final : public I_Event<EditorEvent::EV_File_Open>
	{
		constexpr File_Open_Event() {}
	};

	struct File_Save_Event final : public I_Event<EditorEvent::EV_File_Save>
	{
		constexpr File_Save_Event() {}
	};

	struct File_Quit_Event final : public I_Event<EditorEvent::EV_File_Quit>
	{
		constexpr File_Quit_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Edit_Undo_Event final : public I_Event<EditorEvent::EV_Edit_Undo>
	{
		constexpr Edit_Undo_Event() {}
	};

	struct Edit_Redo_Event final : public I_Event<EditorEvent::EV_Edit_Redo>
	{
		constexpr Edit_Redo_Event() {}
	};

	struct Edit_Cut_Event final : public I_Event<EditorEvent::EV_Edit_Cut>
	{
		constexpr Edit_Cut_Event() {}
	};

	struct Edit_Copy_Event final : public I_Event<EditorEvent::EV_Edit_Copy>
	{
		constexpr Edit_Copy_Event() {}
	};

	struct Edit_Paste_Event final : public I_Event<EditorEvent::EV_Edit_Paste>
	{
		constexpr Edit_Paste_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_