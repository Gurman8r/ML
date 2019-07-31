#ifndef _ML_WINDOW_EVENTS_HPP_
#define _ML_WINDOW_EVENTS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/I_Event.hpp>
#include <ML/Core/C_String.hpp>
#include <ML/Window/KeyCode.hpp>
#include <ML/Window/MouseButton.hpp>

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

			EV_Char,		// Keyboard text input
			EV_CursorEnter,	// Cursor enter/exit window
			EV_CursorPos,	// Cursor position changed
			EV_FrameSize,	// Window frame size changed
			EV_Key,			// Keyboard state changed
			EV_MouseButton,	// Mouse button state changed
			EV_Scroll,		// Mouse scrollwheel state changed
			EV_WindowClose,	// Fired when window is closed
			EV_WindowError,	// Window error callback
			EV_WindowFocus,	// Window focused/unfocused
			EV_WindowKill,	// Tell window to close
			EV_WindowSize,	// Window size changed
			Ev_WindowPos,	// Window position changed

			MAX_WINDOW_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct CharEvent final : public I_Event<WindowEvent::EV_Char>
	{
		const uint32_t value;
		constexpr CharEvent(uint32_t value)
			: value(value)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct CursorEnterEvent final : public I_Event<WindowEvent::EV_CursorEnter>
	{
		const int32_t entered;
		constexpr CursorEnterEvent(int32_t entered)
			: entered(entered)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct CursorPosEvent final : public I_Event<WindowEvent::EV_CursorPos>
	{
		const float64_t x, y;
		constexpr CursorPosEvent(float64_t x, float64_t y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct KeyEvent final : public I_Event<WindowEvent::EV_Key>
	{
		const int32_t key, scan, act;
		
		const struct Mods
		{ 
			bool shift	= false;
			bool ctrl	= false;
			bool alt	= false;
			bool super	= false;

			constexpr friend bool operator==(const Mods & lhs, const Mods & rhs)
			{
				return
					lhs.shift == rhs.shift &&
					lhs.ctrl  == rhs.ctrl  &&
					lhs.alt	  == rhs.alt   &&
					lhs.super == rhs.super ;
			}

		} mods;

		constexpr KeyEvent(int32_t key, int32_t scan, int32_t act, const Mods & mods)
			: key	(key)
			, scan	(scan)
			, act	(act)
			, mods	{ mods.shift, mods.ctrl, mods.alt, mods.super }
		{
		}

		constexpr bool getPress	(int32_t k)	const { return (key == k && act == ML_KEY_PRESS); }
		constexpr bool getDown	(int32_t k) const { return (key == k && act == ML_KEY_REPEAT); }
		constexpr bool getUp	(int32_t k)	const { return (key == k && act == ML_KEY_RELEASE); }

		constexpr bool getPress	(int32_t k, const Mods & m)	const { return getPress(k) && (mods == m); }
		constexpr bool getDown	(int32_t k, const Mods & m) const { return getDown(k) && (mods == m); }
		constexpr bool getUp	(int32_t k, const Mods & m)	const { return getUp(k) && (mods == m); }

		constexpr bool isShift	(int32_t k)	const { return getPress(k, { 1, 0, 0, 0 }); }
		constexpr bool isCtrl	(int32_t k)	const { return getPress(k, { 0, 1, 0, 0 }); }
		constexpr bool isAlt	(int32_t k)	const { return getPress(k, { 0, 0, 1, 0 }); }
		constexpr bool isSuper	(int32_t k)	const { return getPress(k, { 0, 0, 0, 1 }); }

		constexpr bool isNew	() const { return isCtrl(KeyCode::N); }
		constexpr bool isOpen	() const { return isCtrl(KeyCode::O); }
		constexpr bool isSave	() const { return isCtrl(KeyCode::S) || getPress(KeyCode::S, { 1, 1, 0, 0 }); }
		constexpr bool isUndo	() const { return isCtrl(KeyCode::Z); }
		constexpr bool isRedo	() const { return isCtrl(KeyCode::Y) || getPress(KeyCode::Z, { 1, 1, 0, 0 }); }
		constexpr bool isCut	() const { return isCtrl(KeyCode::X) || isShift(KeyCode::Delete); }
		constexpr bool isCopy	() const { return isCtrl(KeyCode::C) || isCtrl(KeyCode::Insert); }
		constexpr bool isPaste	() const { return isCtrl(KeyCode::V) || isShift(KeyCode::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct MouseButtonEvent final : public I_Event<WindowEvent::EV_MouseButton>
	{
		const int32_t key, act, mod;
		constexpr MouseButtonEvent(int32_t key, int32_t act, int32_t mod)
			: key(key)
			, act(act)
			, mod(mod)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ScrollEvent final : public I_Event<WindowEvent::EV_Scroll>
	{
		const float64_t x, y;
		constexpr ScrollEvent(float64_t x, float64_t y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct FrameSizeEvent final : public I_Event<WindowEvent::EV_FrameSize>
	{
		const int32_t width, height;
		constexpr FrameSizeEvent(int32_t width, int32_t height)
			: width(width)
			, height(height)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowCloseEvent final : public I_Event<WindowEvent::EV_WindowClose>
	{
		WindowCloseEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowErrorEvent final : public I_Event<WindowEvent::EV_WindowError>
	{
		const int32_t code;
		const C_String desc;
		constexpr WindowErrorEvent(int32_t code, C_String desc)
			: code(code)
			, desc(desc)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowFocusEvent final : public I_Event<WindowEvent::EV_WindowFocus>
	{
		const int32_t focused;
		constexpr WindowFocusEvent(int32_t entered)
			: focused(entered)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowKillEvent final : public I_Event<WindowEvent::EV_WindowKill>
	{
		constexpr WindowKillEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowPosEvent final : public I_Event<WindowEvent::Ev_WindowPos>
	{
		const int32_t x, y;
		constexpr WindowPosEvent(int32_t x, int32_t y)
			: x(x)
			, y(y)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowSizeEvent final : public I_Event<WindowEvent::EV_WindowSize>
	{
		const int32_t width, height;
		constexpr WindowSizeEvent(int32_t width, int32_t height)
			: width(width)
			, height(height)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_