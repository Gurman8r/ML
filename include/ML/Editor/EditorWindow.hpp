#ifndef _ML_EDITOR_WINDOW_HPP_
#define _ML_EDITOR_WINDOW_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorWindow : public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit EditorWindow(Editor & editor, C_String title, bool open);

		virtual ~EditorWindow() {}

		virtual bool beginDraw(int32_t flags);
		virtual bool onGui(const GuiEvent & ev) = 0;
		virtual bool endDraw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto editor()		const	-> Editor &	{ return m_editor; }
		inline auto getTitle()		const	-> C_String	{ return m_title; }
		inline auto getFlags()		const	-> int32_t	{ return m_flags; }
		inline auto isOpen()		const	-> bool		{ return m_open; }
		inline auto openPtr()				-> bool *	{ return &m_open; }
		inline auto setOpen(bool v)			-> bool		{ return m_open = v; }
		inline auto toggleOpen()			-> bool		{ return setOpen(!isOpen()); }

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

#endif // !_ML_EDITOR_WINDOW_HPP_