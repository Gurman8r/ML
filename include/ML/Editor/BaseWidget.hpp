#ifndef _ML_BASE_WIDGET_HPP_
#define _ML_BASE_WIDGET_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/ITrackable.hpp>

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
	protected:
		BaseWidget(CString title, Editor & editor, bool open);
		virtual ~BaseWidget();

	public:
		inline bool onGui(const GuiEvent & ev)
		{
			if (this->isOpen())
			{
				this->drawGui(ev);
				return true;
			}
			return false;
		}

	protected:
		virtual bool drawGui(const GuiEvent & ev) = 0;
		virtual bool beginDraw(int32_t flags = 0);
		virtual bool endDraw();

	public:
		uint32_t getID() const;

	public:
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
		Editor &	m_editor;
		bool		m_good;
		bool		m_open;
		CString		m_title;
		int32_t		m_flags;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BASE_WIDGET_HPP_