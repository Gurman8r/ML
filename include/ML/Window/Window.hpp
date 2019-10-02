#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/I_EventListener.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/Matrix.hpp>
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
		: public I_Newable
		, public I_Disposable
		, public I_NonCopyable
		, public I_EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using CharFun			= typename void(*)(void *, uint32_t);
		using CursorEnterFun	= typename void(*)(void *, int32_t);
		using CursorPosFun		= typename void(*)(void *, float64_t, float64_t);
		using ErrorFun			= typename void(*)(int32_t, C_String);
		using FrameSizeFun		= typename void(*)(void *, int32_t, int32_t);
		using KeyFun			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseButtonFun	= typename void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= typename void(*)(void *, float64_t, float64_t);
		using CloseFun			= typename void(*)(void *);
		using FocusFun			= typename void(*)(void *, int32_t);
		using PositionFun		= typename void(*)(void *, int32_t, int32_t);
		using SizeFun			= typename void(*)(void *, int32_t, int32_t);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window();
		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(
			const String & title, 
			const VideoMode & videoMode,
			const WindowStyle & style,
			const ContextSettings & context
		);

		virtual bool setup();

		virtual void onEvent(const Event & value) override;

		virtual bool dispose() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & close();
		Window & destroy();
		Window & iconify();
		Window & makeContextCurrent();
		Window & maximize();
		Window & pollEvents();
		Window & restore();
		Window & swapBuffers();
		Window & swapInterval(int32_t value);
		Window & terminate();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & setCentered();
		Window & setClipboardString(const String & value);
		Window & setCursor(void * value);
		Window & setCursorMode(const Cursor::Mode value);
		Window & setCursorPos(const vec2i & value);
		Window & setFullscreen(bool value);
		Window & setIcon(uint32_t w, uint32_t h, const uint8_t * pixels);
		Window & setPosition(const vec2i & value);
		Window & setMonitor(void * value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool		is_focused() const;
		bool		is_fullscreen()	const;
		bool		is_open() const;
		int32_t		getAttribute(int32_t value) const;
		char		getChar() const;
		C_String	getClipboardString() const;
		vec2		getCursorPos() const;
		vec2i		getFrameSize() const;
		int32_t		getKey(int32_t value) const;
		int32_t		getInputMode() const;
		int32_t		getMouseButton(int32_t button) const;
		vec2i		getPosition() const;
		float64_t	getTime() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto aspect()		const -> const float_t			{ return ML_ASPECT_2(size()); };
		inline auto context()		const -> const ContextSettings &{ return m_context; }
		inline auto frame_aspect()	const -> float_t				{ return ML_ASPECT_2(getFrameSize()); };
		inline auto frame_height()	const -> int32_t				{ return getFrameSize()[1]; }
		inline auto frame_width()	const -> int32_t				{ return getFrameSize()[0]; }
		inline auto height()		const -> const uint32_t	&		{ return size()[1]; }
		inline auto monitor()		const -> const void *			{ return m_monitor; }
		inline auto share()			const -> const void *			{ return m_share; }
		inline auto size()			const -> const vec2u &			{ return video_mode().size; }
		inline auto style()			const -> const WindowStyle &	{ return m_style; }
		inline auto title()			const -> const String &			{ return m_title; }
		inline auto video_mode()	const -> const VideoMode &		{ return m_videoMode; }
		inline auto width()			const -> const uint32_t	&		{ return size()[0]; }
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void *	createCustomCursor(uint32_t w, uint32_t h, const uint8_t * pixels) const;
		void *	createStandardCursor(Cursor::Shape value) const;
		bool	destroyCursor(void * value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const VideoMode &		getDesktopMode();
		static const List<VideoMode> &	getFullscreenModes();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		CharFun			setCharCallback			(CharFun		value);
		CursorEnterFun	setCursorEnterCallback	(CursorEnterFun value);
		CursorPosFun	setCursorPosCallback	(CursorPosFun	value);
		ErrorFun		setErrorCallback		(ErrorFun		value);
		FrameSizeFun	setFrameSizeCallback	(FrameSizeFun	value);
		KeyFun			setKeyCallback			(KeyFun			value);
		MouseButtonFun	setMouseButtonCallback	(MouseButtonFun value);
		ScrollFun		setScrollCallback		(ScrollFun		value);
		CloseFun		setWindowCloseCallback	(CloseFun		value);
		FocusFun		setWindowFocusCallback	(FocusFun		value);
		PositionFun		setWindowPosCallback	(PositionFun	value);
		SizeFun			setWindowSizeCallback	(SizeFun		value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void *			m_window;		// 
		void *			m_monitor;		// 
		void *			m_share;		// 
		ContextSettings	m_context;		// 
		WindowStyle		m_style;		// 
		VideoMode		m_videoMode;	// 
		String			m_title;		// 
		mutable char	m_char;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_