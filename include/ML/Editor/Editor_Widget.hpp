#ifndef _ML_EDITOR_WIDGET_HPP_
#define _ML_EDITOR_WIDGET_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Widget : public NonCopyable, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		Editor_Widget(C_String title, C_String hotkey, bool startOpen);

		virtual ~Editor_Widget() {}

		virtual void onEvent(Event const & value) override;
		virtual bool beginDraw(int32_t flags);
		virtual bool draw() = 0;
		virtual bool endDraw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		bool setFocused(bool value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getTitle()		const	-> C_String	{ return m_title; }
		inline auto getHotkey()		const	-> C_String { return m_hotkey; }
		inline auto getFlags()		const	-> int32_t	{ return m_flags; }
		inline auto isOpen()		const	-> bool		{ return m_open; }
		inline auto openPtr()				-> bool *	{ return &m_open; }
		inline auto setOpen(bool value)		-> bool		{ return (m_open = value); }
		inline auto toggleOpen()			-> bool		{ return setOpen(!isOpen()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		bool		m_good;
		bool		m_open;
		C_String	m_title;
		C_String	m_hotkey;
		int32_t		m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_WIDGET_HPP_