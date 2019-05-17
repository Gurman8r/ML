#ifndef _ML_WINDOW_EVENTS_HPP_
#define _ML_WINDOW_EVENTS_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Window/WindowInput.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Vector2.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_WINDOW_EVENT = IEvent::EV_WINDOW + 1,

			EV_Char,
			EV_CursorEnter,
			EV_CursorPos,
			EV_FrameSize,
			EV_Key,
			EV_MouseButton,
			EV_Scroll,
			EV_WindowError,
			EV_WindowClose,
			EV_WindowFocus,
			EV_WindowSize,
			Ev_WindowPos,

			MAX_WINDOW_EVENT
		};

		WindowEvent(int32_t eventID)
			: IEvent(eventID)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CharEvent final : public WindowEvent
	{
		uint32_t value;

		CharEvent(uint32_t value)
			: WindowEvent(EV_Char)
			, value(value)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << (char)value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorEnterEvent final : public WindowEvent
	{
		int32_t entered;

		CursorEnterEvent(int32_t entered)
			: WindowEvent(EV_CursorEnter)
			, entered(entered)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << entered;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorPosEvent final : public WindowEvent
	{
		double x;
		double y;

		CursorPosEvent(double x, double y)
			: WindowEvent(EV_CursorPos)
			, x(x)
			, y(y)
		{
		}
		
		void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << x << " " << y;
		}

		inline vec2d position() const { return { x, y }; }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API KeyEvent final : public WindowEvent
	{
		using Mods = typename std::array<bool, 4>;

		int32_t button, scan, action;

		bool mod_shift, mod_ctrl, mod_alt, mod_super;

		KeyEvent(
			int32_t button, int32_t scan, int32_t action, 
			bool mod_shift, bool mod_ctrl, bool mod_alt, bool mod_super)
			: WindowEvent(EV_Key)
			, button	(button)
			, scan		(scan)
			, action	(action)
			, mod_shift	(mod_shift)
			, mod_ctrl	(mod_ctrl)
			, mod_alt	(mod_alt)
			, mod_super	(mod_super)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | "
				<< button << " " << scan << " " << action << " "
				<< mod_shift << " " << mod_ctrl << " " << mod_alt << " " << mod_super;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool getAction(int32_t a)	const { return (action == a); }
		inline bool getKey(int32_t b)		const { return (button == b); }
		inline bool getKeyDown(int32_t b)	const { return getKey(b) && getAction(ML_KEY_PRESS); }
		inline bool getKeyRepeat(int32_t b) const { return getKey(b) && getAction(ML_KEY_REPEAT); }
		inline bool getKeyUp(int32_t b)		const { return getKey(b) && getAction(ML_KEY_RELEASE); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API MouseButtonEvent final : public WindowEvent
	{
		int32_t button, action, mods;

		MouseButtonEvent(int32_t button, int32_t action, int32_t mods)
			: WindowEvent(EV_MouseButton)
			, button(button)
			, action(action)
			, mods(mods)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " 
				<< button << " " << action << " " << mods;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API ScrollEvent final : public WindowEvent
	{
		double x;
		double y;

		ScrollEvent(double x, double y)
			: WindowEvent(EV_Scroll)
			, x(x)
			, y(y)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << x << " " << y;
		}

		inline vec2d position() const { return { x, y }; }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API FrameSizeEvent final : public WindowEvent
	{
		int32_t width;
		int32_t height;

		FrameSizeEvent(int32_t width, int32_t height)
			: WindowEvent(EV_FrameSize)
			, width(width)
			, height(height)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << width << " " << height;
		}

		inline vec2i size() const { return { width, height }; }

		inline float aspect() const { return ((width && height) ? (float)(width) / (float)(height) : FLT_MIN); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowCloseEvent final : public WindowEvent
	{
		WindowCloseEvent()
			: WindowEvent(EV_WindowClose)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowErrorEvent final : public WindowEvent
	{
		int32_t code;
		CString desc;

		WindowErrorEvent(int32_t code, CString desc)
			: WindowEvent(EV_WindowError)
			, code(code)
			, desc(desc)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << code << " " << desc;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFocusEvent final : public WindowEvent
	{
		int32_t focused;

		WindowFocusEvent(int32_t entered)
			: WindowEvent(EV_WindowFocus)
			, focused(entered)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << focused;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowPosEvent final : public WindowEvent
	{
		int32_t x;
		int32_t y;

		WindowPosEvent(int32_t x, int32_t y)
			: WindowEvent(Ev_WindowPos)
			, x(x)
			, y(y)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << x << " " << y;
		}

		inline vec2i position() const { return { x, y }; }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowSizeEvent final : public WindowEvent
	{
		int32_t width;
		int32_t height;

		WindowSizeEvent(int32_t width, int32_t height)
			: WindowEvent(EV_WindowSize)
			, width(width)
			, height(height)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << width << " " << height;
		}

		inline vec2i size() const { return { width, height }; }

		inline float aspect() const
		{
			return (width && height)
				? (float)(width) / (float)(height)
				: 0.0f;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_