#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <ML/Core/IEvent.hpp>
#include <ML/Editor/Export.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Editor;
	class Engine;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = IEvent::EV_EDITOR + 1,

			EV_Gui,

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

		EditorEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API GuiEvent final : public EditorEvent
	{
		Engine		& engine;
		Editor		& editor;
		Resources	& resources;

		GuiEvent(Engine & engine, Editor & editor, Resources & resources)
			: EditorEvent(EV_Gui)
			, engine(engine)
			, editor(editor)
			, resources(resources)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_New_Event final : public EditorEvent
	{
		File_New_Event()
			: EditorEvent(EV_File_New)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_Open_Event final : public EditorEvent
	{
		File_Open_Event()
			: EditorEvent(EV_File_Open)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_Save_Event final : public EditorEvent
	{
		const bool all;
		File_Save_Event(const bool all)
			: EditorEvent(EV_File_Save)
			, all(all)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API File_Close_Event final : public EditorEvent
	{
		File_Close_Event()
			: EditorEvent(EV_File_Close)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Undo_Event final : public EditorEvent
	{
		Edit_Undo_Event()
			: EditorEvent(EV_Edit_Undo)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Redo_Event final : public EditorEvent
	{
		Edit_Redo_Event()
			: EditorEvent(EV_Edit_Redo)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Cut_Event final : public EditorEvent
	{
		Edit_Cut_Event()
			: EditorEvent(EV_Edit_Cut)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Copy_Event final : public EditorEvent
	{
		Edit_Copy_Event()
			: EditorEvent(EV_Edit_Copy)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Edit_Paste_Event final : public EditorEvent
	{
		Edit_Paste_Event()
			: EditorEvent(EV_Edit_Paste)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_