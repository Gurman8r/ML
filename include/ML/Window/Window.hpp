#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/Trackable.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Window/ContextSettings.hpp>
#include <ML/Window/Cursor.hpp>
#include <ML/Window/KeyCode.hpp>
#include <ML/Window/MouseButton.hpp>
#include <ML/Window/VideoMode.hpp>
#include <ML/Window/WindowStyle.hpp>

#define ML_ASPECT(w, h) ((w != 0 && h != 0) ? ((float_t)w / (float_t)(h)) : 0.0f)
#define ML_ASPECT_2(v)	ML_ASPECT(v[0], v[1])

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Window
		: public Trackable
		, public Disposable
		, public NonCopyable
		, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using CharFun			= typename void(*)(voidptr_t , uint32_t);
		using CursorEnterFun	= typename void(*)(voidptr_t , int32_t);
		using CursorPosFun		= typename void(*)(voidptr_t , float64_t, float64_t);
		using ErrorFun			= typename void(*)(int32_t, C_String);
		using FrameSizeFun		= typename void(*)(voidptr_t , int32_t, int32_t);
		using KeyFun			= typename void(*)(voidptr_t , int32_t, int32_t, int32_t, int32_t);
		using MouseFun			= typename void(*)(voidptr_t , int32_t, int32_t, int32_t);
		using ScrollFun			= typename void(*)(voidptr_t , float64_t, float64_t);
		using CloseFun			= typename void(*)(voidptr_t );
		using FocusFun			= typename void(*)(voidptr_t , int32_t);
		using PositionFun		= typename void(*)(voidptr_t , int32_t, int32_t);
		using SizeFun			= typename void(*)(voidptr_t , int32_t, int32_t);
		using ProcFun			= typename voidptr_t(*)(void);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window();

		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool create(
			const String & title, 
			const VideoMode & videoMode,
			const WindowStyle & style,
			const ContextSettings & context
		);

		virtual void installCallbacks();

		virtual bool dispose() override;

		virtual void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & close();
		
		Window & destroy();
		
		Window & iconify();
		
		Window & makeContextCurrent();
		
		Window & maximize();
		
		Window & restore();
		
		Window & swapBuffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & setCentered();
		
		Window & setClipboardString(const String & value);
		
		Window & setCursor(voidptr_t value);
		
		Window & setCursorMode(const Cursor::Mode value);
		
		Window & setCursorPos(const vec2i & value);
		
		Window & setFullscreen(bool value);
		
		Window & setIcon(uint32_t w, uint32_t h, const_ptr_t<byte_t> pixels);
		
		Window & setPosition(const vec2i & value);
		
		Window & setMonitor(voidptr_t value);
		
		Window & setSize(const vec2u & value);
		
		Window & setTitle(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool isFocused() const;
		
		bool isFullscreen() const;
		
		bool isOpen() const;
		
		int32_t	getAttribute(int32_t value) const;
		
		C_String getClipboardString() const;
		
		vec2 getCursorPos() const;
		
		vec2i getFrameSize() const;
		
		voidptr_t getHandle() const;

		int32_t getKey(int32_t value) const;
		
		int32_t	getInputMode() const;
		
		int32_t	getMouseButton(int32_t button) const;
		
		vec2i getPosition() const;
		
		voidptr_t getRawHandle() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getAspect() const -> float_t { return ML_ASPECT_2(getSize()); };
		
		inline auto getContext() const -> const ContextSettings & { return m_context; }
		
		inline auto getFrameAspect() const -> float_t { return ML_ASPECT_2(getFrameSize()); };
		
		inline auto getFrameHeight() const -> int32_t { return getFrameSize()[1]; }
		
		inline auto getFrameWidth()	const -> int32_t { return getFrameSize()[0]; }
		
		inline auto getHeight()	const -> uint32_t { return getSize()[1]; }
		
		inline auto getMonitor() const -> voidptr_t { return m_monitor; }

		inline auto getShare() const -> voidptr_t { return m_share; }
		
		inline auto getSize() const -> const vec2u & { return getVideoMode().size; }
		
		inline auto getStyle() const -> const WindowStyle & { return m_style; }
		
		inline auto getTitle() const -> const String & { return m_title; }
		
		inline auto getVideoMode() const -> const VideoMode & { return m_videoMode; }
		
		inline auto getWidth() const -> uint32_t { return getSize()[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static voidptr_t createCustomCursor(uint32_t w, uint32_t h, const_ptr_t<byte_t> pixels);
		
		static voidptr_t createStandardCursor(Cursor::Shape value);
		
		static bool	destroyCursor(voidptr_t value);

		static int32_t extensionSupported(C_String value);

		static voidptr_t getContextCurrent();

		static const VideoMode & getDesktopMode();
		
		static const List<VideoMode> & getFullscreenModes();

		static ProcFun getProcAddress(C_String value);
		
		static const List<voidptr_t> & getMonitors();

		static float64_t getTime();

		static void makeContextCurrent(voidptr_t value);

		static void pollEvents();
		
		static void swapInterval(int32_t value);

		static void terminate();
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		CharFun			setCharCallback			(CharFun		value);
		CursorEnterFun	setCursorEnterCallback	(CursorEnterFun value);
		CursorPosFun	setCursorPosCallback	(CursorPosFun	value);
		ErrorFun		setErrorCallback		(ErrorFun		value);
		FrameSizeFun	setFrameSizeCallback	(FrameSizeFun	value);
		KeyFun			setKeyCallback			(KeyFun			value);
		MouseFun		setMouseCallback		(MouseFun		value);
		ScrollFun		setScrollCallback		(ScrollFun		value);
		CloseFun		setWindowCloseCallback	(CloseFun		value);
		FocusFun		setWindowFocusCallback	(FocusFun		value);
		PositionFun		setWindowPosCallback	(PositionFun	value);
		SizeFun			setWindowSizeCallback	(SizeFun		value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		voidptr_t 		m_window;		//
		voidptr_t 		m_monitor;		//
		voidptr_t 		m_share;		//
		ContextSettings	m_context;		//
		WindowStyle		m_style;		//
		VideoMode		m_videoMode;	//
		String			m_title;		//

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_