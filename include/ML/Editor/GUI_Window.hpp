#ifndef _ML_GUI_WINDOW_HPP_
#define _ML_GUI_WINDOW_HPP_

#include <ML/Editor/GUI_Base.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	// A GUI drawer for Window
	class ML_EDITOR_API GUI_Window
		: public GUI_Base
		, public IEventListener
	{
	public:
		GUI_Window(CString title);
		virtual ~GUI_Window();

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool drawGui(bool * p_open) = 0;

	protected:
		virtual bool beginDraw(bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	public:
		inline CString	title() const { return m_title; }
		inline bool *	open()	const { return m_open; }
		inline int32_t	flags() const { return m_flags; }

		uint32_t getID() const;

	protected:
		CString	m_title;
		bool *	m_open;
		int32_t m_flags;
	};
}

#endif // !_ML_GUI_WINDOW_HPP_