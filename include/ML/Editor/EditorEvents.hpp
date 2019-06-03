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
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EditorEvent final
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = Event::EV_EDITOR,

			EV_BeginGui,
			EV_Gui,
			EV_EndGui,

			EV_File_New,
			EV_File_Open,
			EV_File_Save,
			EV_File_Close,

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
		BeginGuiEvent() {}
	};

	struct ML_EDITOR_API GuiEvent final : public IEvent<EditorEvent::EV_Gui>
	{
		const GameTime	& time;
		Resources		& resources;
		Editor			& editor;
		GuiEvent(const GameTime & time, Resources & resources, Editor & editor)
			: time		(time)
			, resources	(resources)
			, editor	(editor)
		{
		}
	};

	struct ML_EDITOR_API EndGuiEvent final : public IEvent<EditorEvent::EV_EndGui>
	{
		EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_New_Event final : public IEvent<EditorEvent::EV_File_New>
	{
		File_New_Event() {}
	};

	struct ML_EDITOR_API File_Open_Event final : public IEvent<EditorEvent::EV_File_Open>
	{
		File_Open_Event() {}
	};

	struct ML_EDITOR_API File_Save_Event final : public IEvent<EditorEvent::EV_File_Save>
	{
		File_Save_Event() {}
	};

	struct ML_EDITOR_API File_Close_Event final : public IEvent<EditorEvent::EV_File_Close>
	{
		File_Close_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Undo_Event final : public IEvent<EditorEvent::EV_Edit_Undo>
	{
		Edit_Undo_Event() {}
	};

	struct ML_EDITOR_API Edit_Redo_Event final : public IEvent<EditorEvent::EV_Edit_Redo>
	{
		Edit_Redo_Event() {}
	};

	struct ML_EDITOR_API Edit_Cut_Event final : public IEvent<EditorEvent::EV_Edit_Cut>
	{
		Edit_Cut_Event() {}
	};

	struct ML_EDITOR_API Edit_Copy_Event final : public IEvent<EditorEvent::EV_Edit_Copy>
	{
		Edit_Copy_Event() {}
	};

	struct ML_EDITOR_API Edit_Paste_Event final : public IEvent<EditorEvent::EV_Edit_Paste>
	{
		Edit_Paste_Event() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_