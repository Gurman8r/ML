#ifndef _ML_EDITOR_GUI_HPP_
#define _ML_EDITOR_GUI_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Represents an integral Editor window
	class ML_EDITOR_API EditorGui : public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit EditorGui(Editor & editor, C_String title, bool open);

		virtual ~EditorGui() {}

		virtual bool beginDraw(int32_t flags);
		virtual bool drawGui(const GuiEvent & ev) = 0;
		virtual bool endDraw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto editor	()	const -> Editor &	{ return m_editor; }
		inline auto getTitle()	const -> C_String	{ return m_title; }
		inline auto getFlags()	const -> int32_t	{ return m_flags; }
		inline auto isOpen	()	const -> bool		{ return m_open; }
		inline auto openPtr	()		  -> bool *		{ return &m_open; }
		inline auto setOpen	(bool v)  -> bool		{ return m_open = v; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		Editor &	m_editor;
		bool		m_good;
		bool		m_open;
		C_String	m_title;
		int32_t		m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_GUI_HPP_