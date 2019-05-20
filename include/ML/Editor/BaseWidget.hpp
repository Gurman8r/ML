#ifndef _ML_BASE_WIDGET_HPP_
#define _ML_BASE_WIDGET_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Editor;
	class Engine;
	class EventSystem;
	class Preferences;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API BaseWidget
		: public ITrackable
		, public INonCopyable
	{
	public:
		BaseWidget(CString title, Editor & editor, bool open);
		virtual ~BaseWidget();

		bool show = false;

	public:
		virtual bool drawGui(const GuiEvent & ev) = 0;

	public:
		uint32_t getID() const;

		Editor		& editor()		const;
		Engine		& engine()		const;
		EventSystem	& eventSystem() const;
		Preferences & prefs()		const;
		Resources	& resources()	const;

	public:
		inline CString	getTitle()	const	{ return m_title; }
		inline int32_t	getFlags()	const	{ return m_flags; }
		inline bool		isOpen()	const	{ return m_open; }
		inline bool *	openPtr()			{ return &m_open; }

	protected:
		virtual bool beginDraw(bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	protected:
		inline bool	good() const { return (m_good); }
		inline bool goodCheck(const bool value) { return (m_good = value); }

	protected:
		Editor &	m_editor;
		bool		m_good;
		CString		m_title;
		int32_t		m_flags;
		bool		m_open;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BASE_WIDGET_HPP_