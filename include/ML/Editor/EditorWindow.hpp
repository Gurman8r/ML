#ifndef _ML_EDITOR_FORM_HPP_
#define _ML_EDITOR_FORM_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorWindow : public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit EditorWindow(Editor & editor, C_String title, C_String hotkey, bool startOpen);

		virtual ~EditorWindow() {}

		virtual void onUpdate(const UpdateEvent & ev) = 0;
		virtual bool beginDraw(int32_t flags);
		virtual bool onGui(const GuiEvent & ev) = 0;
		virtual bool endDraw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		bool Focus(bool value);
		bool MenuItem(bool showHotkey = true);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto editor()		const	-> Editor &	{ return m_editor; }
		inline auto getTitle()		const	-> C_String	{ return m_title; }
		inline auto getHotkey()		const	-> C_String { return m_hotkey; }
		inline auto getFlags()		const	-> int32_t	{ return m_flags; }
		inline auto isOpen()		const	-> bool		{ return m_open; }
		inline auto openPtr()				-> bool *	{ return &m_open; }
		inline auto setOpen(bool value)		-> bool		{ return (m_open = value); }
		inline auto toggleOpen()			-> bool		{ return setOpen(!isOpen()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		Editor &	m_editor;
		bool		m_good;
		bool		m_open;
		C_String	m_title;
		C_String	m_hotkey;
		int32_t		m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_FORM_HPP_