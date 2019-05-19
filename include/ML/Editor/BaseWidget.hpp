#ifndef _ML_BASE_WIDGET_HPP_
#define _ML_BASE_WIDGET_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	class EventSystem;

	class ML_EDITOR_API BaseWidget
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		BaseWidget(EventSystem & eventSystem, CString title);
		virtual ~BaseWidget();

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool drawGui(const GuiEvent * ev, bool * p_open) = 0;

	protected:
		virtual bool beginDraw(bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	public:
		inline EventSystem &	eventSystem()	const { return m_eventSystem; }
		inline CString			getTitle()		const { return m_title; }
		inline int32_t			getFlags()		const { return m_flags; }
		inline bool *			isOpen()		const { return m_open; }

		uint32_t getID() const;

	protected:
		inline bool	good() const { return (m_good); }
		inline bool goodCheck(const bool value) { return (m_good = value); }

	protected:
		EventSystem & m_eventSystem;

		bool	m_good;
		CString	m_title;
		int32_t m_flags;
		bool *	m_open;
	};
}

#endif // !_ML_BASE_WIDGET_HPP_