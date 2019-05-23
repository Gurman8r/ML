#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/INonCopyable.hpp>
#include <ML/Window/ContextSettings.hpp>
#include <ML/Window/Cursor.hpp>
#include <ML/Window/Icon.hpp>
#include <ML/Window/VideoSettings.hpp>
#include <ML/Window/StyleSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_WINDOW_API Window
		: public ITrackable
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
			const VideoSettings & screen, 
			const StyleSettings & style, 
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
		bool			isFocused() const;
		bool			isOpen() const;
		int32_t			getAttrib(const int32_t value) const;
		char			getChar() const;
		CString			getClipboardString() const;
		vec2			getCursorPos() const;
		vec2i			getFrameSize() const;
		int32_t			getKey(const int32_t value) const;
		int32_t			getInputMode() const;
		int32_t			getMouseButton(const int32_t button) const;
		vec2i			getPosition() const;
		double			getTime() const;


	public: // Inline
		/* * * * * * * * * * * * * * * * * * * * */
		inline const ContextSettings &	getContext()const { return m_context; }
		inline const StyleSettings &	getStyle()	const { return m_style; }
		inline const VideoSettings &	getVideo()	const { return m_video; }
		
		inline const String		getTitle()			const { return m_title; }
		inline const vec2u &	getSize()			const { return getVideo().resolution; }
		inline const uint32_t	getWidth()			const { return getSize()[0]; }
		inline const uint32_t	getHeight()			const { return getSize()[1]; }
		inline const int32_t	getFrameWidth()		const { return getFrameSize()[0]; }
		inline const int32_t	getFrameHeight()	const { return getFrameSize()[1]; }
		inline const float		getAspect()			const { return ML_ASPECT(getWidth(), getHeight()); };
		inline const float		getFrameAspect()	const { return ML_ASPECT(getFrameWidth(), getFrameHeight()); };


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
		StyleSettings	m_style;
		VideoSettings	m_video;
		String			m_title;
		mutable char	m_char;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_