#ifndef _ML_WINDOW_EVENTS_HPP_
#define _ML_WINDOW_EVENTS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Window/Export.hpp>
#include <ML/Window/KeyCode.hpp>
#include <ML/Window/MouseButton.hpp>
#include <ML/Core/Event.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Vector2.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_KEY_RELEASE	0
#define ML_KEY_PRESS	1
#define ML_KEY_REPEAT	2

#define ML_MOD_SHIFT	(0 << 1)
#define ML_MOD_CTRL		(1 << 1)
#define ML_MOD_ALT		(1 << 2)
#define ML_MOD_SUPER	(1 << 3)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowEvent final
	{
		enum : int32_t
		{
			MIN_WINDOW_EVENT = Event::EV_WINDOW + 1,

			EV_Char,
			EV_CursorEnter,
			EV_CursorPos,
			EV_FrameSize,
			EV_Key,
			EV_MouseButton,
			EV_Scroll,
			EV_WindowClose,
			EV_WindowError,
			EV_WindowFocus,
			EV_WindowKill,
			EV_WindowSize,
			Ev_WindowPos,

			MAX_WINDOW_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CharEvent final : public IEvent<WindowEvent::EV_Char>
	{
		uint32_t value;
		CharEvent(uint32_t value)
			: value(value)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorEnterEvent final : public IEvent<WindowEvent::EV_CursorEnter>
	{
		int32_t entered;
		CursorEnterEvent(int32_t entered)
			: entered(entered)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorPosEvent final : public IEvent<WindowEvent::EV_CursorPos>
	{
		double x;
		double y;
		CursorPosEvent(double x, double y)
			: x(x)
			, y(y)
		{
		}

		inline vec2d position() const { return { x, y }; }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API KeyEvent final : public IEvent<WindowEvent::EV_Key>
	{
		int32_t button, scan, action;
		bool	mod_shift, mod_ctrl, mod_alt, mod_super;
		KeyEvent(
			int32_t button, int32_t scan, int32_t action, 
			bool mod_shift, bool mod_ctrl, bool mod_alt, bool mod_super)
			: button	(button)
			, scan		(scan)
			, action	(action)
			, mod_shift	(mod_shift)
			, mod_ctrl	(mod_ctrl)
			, mod_alt	(mod_alt)
			, mod_super	(mod_super)
		{
		}
		inline bool getAction(int32_t a)	const { return (action == a); }
		inline bool getKey(int32_t b)		const { return (button == b); }
		inline bool getKeyDown(int32_t b)	const { return getKey(b) && getAction(ML_KEY_PRESS); }
		inline bool getKeyRepeat(int32_t b) const { return getKey(b) && getAction(ML_KEY_REPEAT); }
		inline bool getKeyUp(int32_t b)		const { return getKey(b) && getAction(ML_KEY_RELEASE); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API MouseButtonEvent final : public IEvent<WindowEvent::EV_MouseButton>
	{
		int32_t button, action, mods;
		MouseButtonEvent(int32_t button, int32_t action, int32_t mods)
			: button(button)
			, action(action)
			, mods(mods)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API ScrollEvent final : public IEvent<WindowEvent::EV_Scroll>
	{
		double x;
		double y;
		ScrollEvent(double x, double y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API FrameSizeEvent final : public IEvent<WindowEvent::EV_FrameSize>
	{
		int32_t width;
		int32_t height;

		FrameSizeEvent(int32_t width, int32_t height)
			: width(width)
			, height(height)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowCloseEvent final : public IEvent<WindowEvent::EV_WindowClose>
	{
		WindowCloseEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowErrorEvent final : public IEvent<WindowEvent::EV_WindowError>
	{
		int32_t code;
		CString desc;
		WindowErrorEvent(int32_t code, CString desc)
			: code(code)
			, desc(desc)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFocusEvent final : public IEvent<WindowEvent::EV_WindowFocus>
	{
		int32_t focused;
		WindowFocusEvent(int32_t entered)
			: focused(entered)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowKillEvent final : public IEvent<WindowEvent::EV_WindowKill>
	{
		WindowKillEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowPosEvent final : public IEvent<WindowEvent::Ev_WindowPos>
	{
		int32_t x;
		int32_t y;
		WindowPosEvent(int32_t x, int32_t y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowSizeEvent final : public IEvent<WindowEvent::EV_WindowSize>
	{
		int32_t width;
		int32_t height;
		WindowSizeEvent(int32_t width, int32_t height)
			: width(width)
			, height(height)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_