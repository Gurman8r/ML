#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/I_EventListener.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/String.hpp>
#include <ML/Window/ContextSettings.hpp>
#include <ML/Window/Cursor.hpp>
#include <ML/Window/VideoMode.hpp>
#include <ML/Window/WindowStyle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Window
		: public I_Newable
		, public I_Disposable
		, public I_NonCopyable
		, public I_EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Window(EventSystem & eventSystem);
		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(
			const String & title, 
			const VideoMode & videoMode,
			const WindowStyle & style,
			const ContextSettings & context
		);

		virtual bool setup();

		virtual void onEvent(const Event & value) override;

		virtual bool dispose() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & close();
		Window & destroy();
		Window & iconify();
		Window & makeContextCurrent();
		Window & maximize();
		Window & pollEvents();
		Window & restore();
		Window & swapBuffers();
		Window & swapInterval(int32_t value);
		Window & setCentered();
		Window & setClipboardString(const String & value);
		Window & setCursor(void * value);
		Window & setCursorMode(const Cursor::Mode value);
		Window & setCursorPos(const vec2i & value);
		Window & setIcon(uint32_t w, uint32_t h, const uint8_t * pixels);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const String & value);
		Window & terminate();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool		isFocused() const;
		bool		isOpen() const;
		int32_t		getAttribute(const int32_t value) const;
		char		getChar() const;
		C_String	getClipboardString() const;
		vec2		getCursorPos() const;
		vec2i		getFrameSize() const;
		int32_t		getKey(const int32_t value) const;
		int32_t		getInputMode() const;
		int32_t		getMouseButton(const int32_t button) const;
		vec2i		getPosition() const;
		float64_t	getTime() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getContext()	const -> const ContextSettings & { return m_context; }
		inline auto getStyle()		const -> const WindowStyle & { return m_style; }
		inline auto getVideoMode()	const -> const VideoMode & { return m_videoMode; }
		inline auto getTitle()		const -> const String { return m_title; }
		inline auto getSize()		const -> const vec2u & { return getVideoMode().resolution; }
		inline auto getWidth()		const -> const uint32_t { return getSize()[0]; }
		inline auto getHeight()		const -> const uint32_t { return getSize()[1]; }
		inline auto getFrameWidth()	const -> const int32_t { return getFrameSize()[0]; }
		inline auto getFrameHeight()const -> const int32_t { return getFrameSize()[1]; }
		inline auto getAspect()		const -> const float_t { return ML_ASPECT(getWidth(), getHeight()); };
		inline auto getFrameAspect()const -> const float_t { return ML_ASPECT(getFrameWidth(), getFrameHeight()); };
		inline auto getMonitor()	const -> const void * { return m_monitor; }
		inline auto getShare()		const -> const void * { return m_share; }
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void *	createCustomCursor(uint32_t w, uint32_t h, const uint8_t * pixels) const;
		void *	createStandardCursor(Cursor::Shape value) const;
		void	destroyCursor(void * value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		CharFun			setCharCallback			(CharFun		callback);
		CursorEnterFun	setCursorEnterCallback	(CursorEnterFun callback);
		CursorPosFun	setCursorPosCallback	(CursorPosFun	callback);
		ErrorFun		setErrorCallback		(ErrorFun		callback);
		FrameSizeFun	setFrameSizeCallback	(FrameSizeFun	callback);
		KeyFun			setKeyCallback			(KeyFun			callback);
		MouseButtonFun	setMouseButtonCallback	(MouseButtonFun callback);
		ScrollFun		setScrollCallback		(ScrollFun		callback);
		CloseFun		setWindowCloseCallback	(CloseFun		callback);
		FocusFun		setWindowFocusCallback	(FocusFun		callback);
		PositionFun		setWindowPosCallback	(PositionFun	callback);
		SizeFun			setWindowSizeCallback	(SizeFun		callback);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void *			m_window;		// 
		void *			m_monitor;		// 
		void *			m_share;		// 
		ContextSettings	m_context;		// 
		WindowStyle		m_style;		// 
		VideoMode		m_videoMode;	// 
		String			m_title;		// 
		mutable char	m_char;			// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_