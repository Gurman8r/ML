#ifndef _ML_EDITOR_WINDOW_HPP_
#define _ML_EDITOR_WINDOW_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Editor;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorGui
		: public I_Newable
		, public I_NonCopyable
		, public EventListener
	{
		friend class Editor;

	protected:
		explicit EditorGui(EventSystem & eventSystem, C_String title);
		virtual ~EditorGui() {}

	protected:
		virtual void onEvent(const Event * value) override {}
		
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
		inline C_String	getTitle()	const	{ return m_title;		}
		inline int32_t	getFlags()	const	{ return m_flags;		}
		inline bool		isOpen	()	const	{ return m_open;		}
		inline bool *	openPtr	()			{ return &m_open;		}
		inline bool		setOpen	(bool v)	{ return m_open = v;	}

	protected:
		bool		m_good;
		bool		m_open;
		C_String	m_title;
		int32_t		m_flags;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_WINDOW_HPP_