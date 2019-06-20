#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/INonCopyable.hpp>
#include <ML/Core/INewable.hpp>
#include <ML/Window/ContextSettings.hpp>
#include <ML/Window/Cursor.hpp>
#include <ML/Window/Icon.hpp>
#include <ML/Window/VideoSettings.hpp>
#include <ML/Window/WindowStyle.hpp>

#define ML_ASPECT(w, h) ((h != 0) \
	? (static_cast<float>(w) / static_cast<float>(h)) \
	: (0.0f))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_WINDOW_API Window
		: public INewable
		, public INonCopyable
		, public EventListener
	{
	public: // Callback Types
		/* * * * * * * * * * * * * * * * * * * * */
		using CharFun			= typename void(*)(void *, uint32_t);
		using CursorEnterFun	= typename void(*)(void *, int32_t);
		using CursorPosFun		= typename void(*)(void *, double, double);
		using ErrorFun			= typename void(*)(int32_t, CString);
		using FrameSizeFun		= typename void(*)(void *, int32_t, int32_t);
		using KeyFun			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseButtonFun	= typename void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= typename void(*)(void *, double, double);
		using CloseFun			= typename void(*)(void *);
		using FocusFun			= typename void(*)(void *, int32_t);
		using PositionFun		= typename void(*)(void *, int32_t, int32_t);
		using SizeFun			= typename void(*)(void *, int32_t, int32_t);


	public: // Constructors / Destructor
		/* * * * * * * * * * * * * * * * * * * * */
		explicit Window(EventSystem & eventSystem);
		virtual ~Window();


	public: // Core
		/* * * * * * * * * * * * * * * * * * * * */
		bool create(
			const String & title, 
			const VideoSettings & video, 
			const WindowStyle & style,
			const ContextSettings & context
		);

		virtual bool setup();

		virtual void onEvent(const Event * ev) override;


	public: // Modifiers
		/* * * * * * * * * * * * * * * * * * * * */
		Window & close();
		Window & destroy();
		Window & iconify();
		Window & makeContextCurrent();
		Window & maximize();
		Window & pollEvents();
		Window & restore();
		Window & swapBuffers();
		Window & swapInterval(const int32_t value);
		Window & setClipboardString(const String & value);
		Window & setCursor(void * value);
		Window & seCursorMode(const Cursor::Mode value);
		Window & setCursorPos(const vec2i & value);
		Window & setIcons(const List<Icon> & value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const String & value);
		Window & terminate();


	public: // Accessors
		/* * * * * * * * * * * * * * * * * * * * */
		bool		isFocused() const;
		bool		isOpen() const;
		int32_t		getAttrib(const int32_t value) const;
		char		getChar() const;
		CString		getClipboardString() const;
		vec2		getCursorPos() const;
		vec2i		getFrameSize() const;
		int32_t		getKey(const int32_t value) const;
		int32_t		getInputMode() const;
		int32_t		getMouseButton(const int32_t button) const;
		vec2i		getPosition() const;
		double		getTime() const;


	public: // Inline
		/* * * * * * * * * * * * * * * * * * * * */
		inline auto getContext()	const -> const ContextSettings & { return m_context; }
		inline auto getStyle()		const -> const WindowStyle & { return m_style; }
		inline auto getVideo()		const -> const VideoSettings & { return m_video; }
		inline auto getTitle()		const -> const String { return m_title; }
		inline auto getSize()		const -> const vec2u & { return getVideo().resolution; }
		inline auto getWidth()		const -> const uint32_t { return getSize()[0]; }
		inline auto getHeight()		const -> const uint32_t { return getSize()[1]; }
		inline auto getFrameWidth()	const -> const int32_t { return getFrameSize()[0]; }
		inline auto getFrameHeight()const -> const int32_t { return getFrameSize()[1]; }
		inline auto getAspect()		const -> const float { return ML_ASPECT(getWidth(), getHeight()); };
		inline auto getFrameAspect()const -> const float { return ML_ASPECT(getFrameWidth(), getFrameHeight()); };


	public: // Cursors
		/* * * * * * * * * * * * * * * * * * * * */
		void *	createCursor(uint32_t value) const;
		void	destroyCursor(void * value) const;


	public: // Set Callbacks
		/* * * * * * * * * * * * * * * * * * * * */
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


	protected: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		void *			m_window;
		void *			m_monitor;
		void *			m_share;
		ContextSettings	m_context;
		WindowStyle	m_style;
		VideoSettings	m_video;
		String			m_title;
		mutable char	m_char;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_