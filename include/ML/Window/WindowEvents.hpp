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
			MIN_WINDOW_EVENT = Event::EV_WINDOW,

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
		const uint32_t value;
		CharEvent(uint32_t value)
			: value(value)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorEnterEvent final : public IEvent<WindowEvent::EV_CursorEnter>
	{
		const int32_t entered;
		CursorEnterEvent(int32_t entered)
			: entered(entered)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API CursorPosEvent final : public IEvent<WindowEvent::EV_CursorPos>
	{
		const double x, y;
		CursorPosEvent(double x, double y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API KeyEvent final : public IEvent<WindowEvent::EV_Key>
	{
		const int32_t key, scan, act;
		
		const struct Mods
		{ 
			bool shift	= false;
			bool ctrl	= false;
			bool alt	= false;
			bool super	= false;

			inline friend bool operator==(const Mods & lhs, const Mods & rhs)
			{
				return
					lhs.shift == rhs.shift &&
					lhs.ctrl  == rhs.ctrl  &&
					lhs.alt	  == rhs.alt   &&
					lhs.super == rhs.super ;
			}

		} mod;

		KeyEvent(int32_t key, int32_t scan, int32_t act, const Mods & mod)
			: key	(key)
			, scan	(scan)
			, act	(act)
			, mod	{ mod.shift, mod.ctrl, mod.alt, mod.super }
		{
		}

		inline bool getPress(int32_t k)	const { return (key == k && act == ML_KEY_PRESS); }
		inline bool getDown	(int32_t k) const { return (key == k && act == ML_KEY_REPEAT); }
		inline bool getUp	(int32_t k)	const { return (key == k && act == ML_KEY_RELEASE); }

		inline bool getPress(int32_t k, const Mods & m)	const { return getPress(k) && (mod == m); }
		inline bool getDown	(int32_t k, const Mods & m) const { return getDown(k) && (mod == m); }
		inline bool getUp	(int32_t k, const Mods & m)	const { return getUp(k) && (mod == m); }

		inline bool isAlt	(int32_t k)	const { return getPress(k, Mods{ 0, 0, 1, 0 }); }
		inline bool isCtrl	(int32_t k)	const { return getPress(k, Mods{ 0, 1, 0, 0 }); }
		inline bool isShift	(int32_t k)	const { return getPress(k, Mods{ 1, 0, 0, 0 }); }
		inline bool isSuper	(int32_t k)	const { return getPress(k, Mods{ 0, 0, 0, 1 }); }

		inline bool isNew	() const { return isCtrl(KeyCode::N); }
		inline bool isOpen	() const { return isCtrl(KeyCode::O); }
		inline bool isSave	() const { return isCtrl(KeyCode::S) || getPress(KeyCode::S, { 1, 1, 0, 0 }); }
		inline bool isUndo	() const { return isCtrl(KeyCode::Z); }
		inline bool isRedo	() const { return isCtrl(KeyCode::Y) || getPress(KeyCode::Z, { 1, 1, 0, 0 }); }
		inline bool isCut	() const { return isCtrl(KeyCode::X) || isShift(KeyCode::Delete); }
		inline bool isCopy	() const { return isCtrl(KeyCode::C) || isCtrl(KeyCode::Insert); }
		inline bool isPaste	() const { return isCtrl(KeyCode::V) || isShift(KeyCode::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API MouseButtonEvent final : public IEvent<WindowEvent::EV_MouseButton>
	{
		const int32_t key, act, mod;
		MouseButtonEvent(int32_t key, int32_t act, int32_t mod)
			: key(key)
			, act(act)
			, mod(mod)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API ScrollEvent final : public IEvent<WindowEvent::EV_Scroll>
	{
		const double x, y;
		ScrollEvent(double x, double y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API FrameSizeEvent final : public IEvent<WindowEvent::EV_FrameSize>
	{
		const int32_t width, height;
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
		const int32_t code;
		const CString desc;
		WindowErrorEvent(int32_t code, CString desc)
			: code(code)
			, desc(desc)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFocusEvent final : public IEvent<WindowEvent::EV_WindowFocus>
	{
		const int32_t focused;
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
		const int32_t x, y;
		WindowPosEvent(int32_t x, int32_t y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowSizeEvent final : public IEvent<WindowEvent::EV_WindowSize>
	{
		const int32_t width, height;
		WindowSizeEvent(int32_t width, int32_t height)
			: width(width)
			, height(height)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_