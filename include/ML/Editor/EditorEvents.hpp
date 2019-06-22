#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/Event.hpp>
#include <ML/Editor/Export.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class DockspaceGui;
	class Editor;
	class GameTime;

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

	struct ML_EDITOR_API BeginGuiEvent final : public IEvent<EditorEvent::EV_BeginGui>
	{
		constexpr BeginGuiEvent() {}
	};

	struct ML_EDITOR_API GuiEvent final : public IEvent<EditorEvent::EV_Gui>
	{
		const GameTime	& time;
		Editor			& editor;
		constexpr GuiEvent(const GameTime & time, Editor & editor)
			: time	(time)
			, editor(editor)
		{
		}
	};

	struct ML_EDITOR_API EndGuiEvent final : public IEvent<EditorEvent::EV_EndGui>
	{
		constexpr EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API MainMenuBarEvent final : public IEvent<EditorEvent::EV_MainMenuBar>
	{
		enum Menu : int32_t
		{ 
			None,
			File, Edit, Window, Help,
			MAX_MAIN_MENU_BAR_MENU
		};

		Editor & editor;
		const Menu menu;
		constexpr MainMenuBarEvent(Editor & editor, const Menu menu)
			: editor(editor)
			, menu	(menu)
		{
		}
	};

	struct ML_EDITOR_API BuildDockspaceEvent final : public IEvent<EditorEvent::EV_BuildDockspace>
	{
		Editor			& editor;
		DockspaceGui	& dockspace;
		constexpr BuildDockspaceEvent(Editor & editor, DockspaceGui & dockspace)
			: editor	(editor)
			, dockspace	(dockspace)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_New_Event final : public IEvent<EditorEvent::EV_File_New>
	{
		constexpr File_New_Event() {}
	};

	struct ML_EDITOR_API File_Open_Event final : public IEvent<EditorEvent::EV_File_Open>
	{
		constexpr File_Open_Event() {}
	};

	struct ML_EDITOR_API File_Save_Event final : public IEvent<EditorEvent::EV_File_Save>
	{
		constexpr File_Save_Event() {}
	};

	struct ML_EDITOR_API File_Quit_Event final : public IEvent<EditorEvent::EV_File_Quit>
	{
		constexpr File_Quit_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Undo_Event final : public IEvent<EditorEvent::EV_Edit_Undo>
	{
		constexpr Edit_Undo_Event() {}
	};

	struct ML_EDITOR_API Edit_Redo_Event final : public IEvent<EditorEvent::EV_Edit_Redo>
	{
		constexpr Edit_Redo_Event() {}
	};

	struct ML_EDITOR_API Edit_Cut_Event final : public IEvent<EditorEvent::EV_Edit_Cut>
	{
		constexpr Edit_Cut_Event() {}
	};

	struct ML_EDITOR_API Edit_Copy_Event final : public IEvent<EditorEvent::EV_Edit_Copy>
	{
		constexpr Edit_Copy_Event() {}
	};

	struct ML_EDITOR_API Edit_Paste_Event final : public IEvent<EditorEvent::EV_Edit_Paste>
	{
		constexpr Edit_Paste_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_