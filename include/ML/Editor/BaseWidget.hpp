#ifndef _ML_BASE_WIDGET_HPP_
#define _ML_BASE_WIDGET_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	class ML_EDITOR_API BaseWidget
		: public ITrackable
		, public IEventListener
	{
	public:
		BaseWidget(CString title);
		virtual ~BaseWidget();

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool drawGui(const GuiEvent * ev, bool * p_open) = 0;

	protected:
		virtual bool beginDraw(bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	public:
		inline CString	title() const { return m_title; }
		inline bool *	open()	const { return m_open; }
		inline int32_t	flags() const { return m_flags; }

		uint32_t getID() const;

	protected:
		inline bool	good() const { return (m_good); }
		inline bool goodCheck(const bool value) { return (m_good = value); }

	protected:
		bool	m_good;
		CString	m_title;
		bool *	m_open;
		int32_t m_flags;
	};
}

#endif // !_ML_BASE_WIDGET_HPP_