#ifndef _ML_EDITOR_WINDOW_HPP_
#define _ML_EDITOR_WINDOW_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/Transform.hpp>
#include <ML/Core/EventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorGui
		: public IObject
		, public INonCopyable
		, public EventListener
	{
	protected:
		explicit EditorGui(EventSystem & eventSystem, CString title);
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
		inline CString	getTitle()	const	{ return m_title;	}
		inline int32_t	getFlags()	const	{ return m_flags;	}
		inline bool		isOpen()	const	{ return m_open;	}
		inline bool &	isOpen()			{ return m_open;	}

	protected:
		bool		m_good;
		bool		m_open;
		CString		m_title;
		int32_t		m_flags;

	public:
		static bool LabelField(CString label);
		static bool Vec2Field(CString label, vec2 & value, float speed = 1.f);
		static bool Vec3Field(CString label, vec3 & value, float speed = 1.f);
		static bool Vec4Field(CString label, vec4 & value, float speed = 1.f);
		static bool QuatField(CString label, quat & value, float speed = 1.f);
		static bool Mat3Field(CString label, mat3 & value, float speed = 1.f);
		static bool Mat4Field(CString label, mat4 & value, float speed = 1.f);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_WINDOW_HPP_