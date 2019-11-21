#ifndef _ML_WINDOW_EVENTS_HPP_
#define _ML_WINDOW_EVENTS_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Event.hpp>
#include <ML/Core/BitMask.hpp>
#include <ML/Window/KeyCode.hpp>
#include <ML/Window/MouseButton.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Input States
#define ML_KEY_RELEASE	0 // High	-> Low
#define ML_KEY_PRESS	1 // Low	-> High
#define ML_KEY_REPEAT	2 // High	-> High

// Keyboard Input Modifiers
#define ML_MOD_SHIFT	(0 << 1)
#define ML_MOD_CTRL		(1 << 1)
#define ML_MOD_ALT		(1 << 2)
#define ML_MOD_SUPER	(1 << 3)
#define ML_MOD_CAPSLOCK	(1 << 4)
#define ML_MOD_NUMLOCK	(1 << 5)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Window;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class WindowEventType
	{
		MIN_WINDOW_EVENT = Event::EV_WINDOW,

		EV_Char,				// Keyboard text input
		EV_CursorEnter,			// Cursor enter/exit window
		EV_CursorPos,			// Cursor position changed
		EV_FrameSize,			// Window frame size changed
		EV_Key,					// Keyboard state changed
		EV_Mouse,				// Mouse button state changed
		EV_Scroll,				// Mouse scrollwheel state changed
		EV_WindowClose,			// Fired when window is closed
		EV_WindowError,			// Window error callback
		EV_WindowFocus,			// Window focused/unfocused
		EV_WindowKill,			// Tell window to close
		EV_WindowSize,			// Window size changed
		EV_WindowPos,			// Window position changed
		EV_WindowFullscreen,	// Change window fullscreen state

		MAX_WINDOW_EVENT
	};

	static_assert(
		(int32_t)WindowEventType::MAX_WINDOW_EVENT < 
		(int32_t)WindowEventType::MIN_WINDOW_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <WindowEventType ID> struct WindowEvent : public T_Event<WindowEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CharEvent final : public WindowEvent<WindowEventType::EV_Char>
	{
		const uint32_t value;
		constexpr CharEvent(uint32_t value) noexcept
			: value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CursorEnterEvent final : public WindowEvent<WindowEventType::EV_CursorEnter>
	{
		const int32_t entered;
		constexpr CursorEnterEvent(int32_t entered) noexcept
			: entered{ entered }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CursorPosEvent final : public WindowEvent<WindowEventType::EV_CursorPos>
	{
		const float64_t x, y;
		constexpr CursorPosEvent(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct KeyEvent final : public WindowEvent<WindowEventType::EV_Key>
	{
		using Mods = typename mask8_t;
		const int32_t key, scan, action;
		const Mods mods;

		constexpr KeyEvent(int32_t key, int32_t scan, int32_t action, Mods const & mods) noexcept
			: key{ key }
			, scan{ scan }
			, action{ action }
			, mods{ mods }
		{
		}

		constexpr bool getKeyAction(int32_t k, int32_t a) const
		{
			return (key == k) && (action == a);
		}

		constexpr bool getPress	(int32_t k)	const { return getKeyAction(k, ML_KEY_PRESS); }
		constexpr bool getDown	(int32_t k) const { return getKeyAction(k, ML_KEY_REPEAT); }
		constexpr bool getUp	(int32_t k)	const { return getKeyAction(k, ML_KEY_RELEASE); }

		constexpr bool getPress	(int32_t k, Mods const & m)	const { return getPress(k) && (mods == m); }
		constexpr bool getDown	(int32_t k, Mods const & m) const { return getDown(k) && (mods == m); }
		constexpr bool getUp	(int32_t k, Mods const & m)	const { return getUp(k) && (mods == m); }

		constexpr bool isShift	(int32_t k)	const { return getPress(k, { { 1, 0, 0, 0 } }); }
		constexpr bool isCtrl	(int32_t k)	const { return getPress(k, { { 0, 1, 0, 0 } }); }
		constexpr bool isAlt	(int32_t k)	const { return getPress(k, { { 0, 0, 1, 0 } }); }
		constexpr bool isSuper	(int32_t k)	const { return getPress(k, { { 0, 0, 0, 1 } }); }

		constexpr bool isNew	() const { return isCtrl(KeyCode::N); }
		constexpr bool isOpen	() const { return isCtrl(KeyCode::O); }
		constexpr bool isSave	() const { return isCtrl(KeyCode::S) || getPress(KeyCode::S, { { 1, 1, 0, 0 } }); }
		constexpr bool isUndo	() const { return isCtrl(KeyCode::Z); }
		constexpr bool isRedo	() const { return isCtrl(KeyCode::Y) || getPress(KeyCode::Z, { { 1, 1, 0, 0 } }); }
		constexpr bool isCut	() const { return isCtrl(KeyCode::X) || isShift(KeyCode::Delete); }
		constexpr bool isCopy	() const { return isCtrl(KeyCode::C) || isCtrl(KeyCode::Insert); }
		constexpr bool isPaste	() const { return isCtrl(KeyCode::V) || isShift(KeyCode::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MouseEvent final : public WindowEvent<WindowEventType::EV_Mouse>
	{
		const int32_t key, action, mods;
		constexpr MouseEvent(int32_t key, int32_t action, int32_t mods) noexcept
			: key{ key }
			, action{ action }
			, mods{ mods }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ScrollEvent final : public WindowEvent<WindowEventType::EV_Scroll>
	{
		const float64_t x, y;
		constexpr ScrollEvent(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FrameSizeEvent final : public WindowEvent<WindowEventType::EV_FrameSize>
	{
		const int32_t width, height;
		constexpr FrameSizeEvent(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowCloseEvent final : public WindowEvent<WindowEventType::EV_WindowClose>
	{
		WindowCloseEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowErrorEvent final : public WindowEvent<WindowEventType::EV_WindowError>
	{
		const int32_t code;
		const C_String desc;
		constexpr WindowErrorEvent(int32_t code, C_String desc) noexcept
			: code{ code }
			, desc{ desc }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowFocusEvent final : public WindowEvent<WindowEventType::EV_WindowFocus>
	{
		const int32_t focused;
		constexpr WindowFocusEvent(int32_t focused) noexcept
			: focused{ focused }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowKillEvent final : public WindowEvent<WindowEventType::EV_WindowKill>
	{
		constexpr WindowKillEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowPosEvent final : public WindowEvent<WindowEventType::EV_WindowPos>
	{
		const int32_t x, y;
		constexpr WindowPosEvent(int32_t x, int32_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowSizeEvent final : public WindowEvent<WindowEventType::EV_WindowSize>
	{
		const int32_t width, height;
		constexpr WindowSizeEvent(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowFullscreenEvent final : public WindowEvent<WindowEventType::EV_WindowFullscreen>
	{
		const int32_t value;
		constexpr WindowFullscreenEvent(int32_t value) noexcept
			: value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_