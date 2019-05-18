#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <ML/Core/IEventListener.hpp>
#include <ML/Core/INonCopyable.hpp>
#include <ML/Window/Cursor.hpp>
#include <ML/Window/Context.hpp>
#include <ML/Window/Icon.hpp>
#include <ML/Window/Screen.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_WINDOW_API Window
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public: // Callback Types
		/* * * * * * * * * * * * * * * * * * * * */
		using CharFun			= void(*)(void *, uint32_t);
		using CursorEnterFun	= void(*)(void *, int32_t);
		using CursorPosFun		= void(*)(void *, double, double);
		using ErrorFun			= void(*)(int32_t, CString);
		using FrameSizeFun		= void(*)(void *, int32_t, int32_t);
		using KeyFun			= void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseButtonFun	= void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= void(*)(void *, double, double);
		using CloseFun			= void(*)(void *);
		using FocusFun			= void(*)(void *, int32_t);
		using PositionFun		= void(*)(void *, int32_t, int32_t);
		using SizeFun			= void(*)(void *, int32_t, int32_t);

	public: // Style
		/* * * * * * * * * * * * * * * * * * * * */
		enum Style : uint32_t
		{
			None		= (0 << 0),
			Resizable	= (1 << 0),
			Visible		= (1 << 1),
			Decorated	= (1 << 2),
			Focused		= (1 << 3),
			AutoIconify	= (1 << 4),
			Floating	= (1 << 5),
			Maximized	= (1 << 6),

			// Resizable | Visible | Decorated | Focused | AutoIconify | Maximized
			Default	= Resizable | Decorated | Focused | AutoIconify | Maximized,
		};

	public: // C/D-tor
		/* * * * * * * * * * * * * * * * * * * * */
		Window();
		virtual ~Window();

	public: // Core
		/* * * * * * * * * * * * * * * * * * * * */
		bool create(const String & title, const Screen & screen, const uint32_t style, const Context & context);

		virtual bool setup();

		virtual void onEvent(const IEvent * ev) override;

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
		bool	isFocused() const;
		bool	isOpen() const;
		int32_t	getAttrib(const int32_t value) const;
		char	getChar() const;
		CString	getClipboardString() const;
		vec2	getCursorPos() const;
		vec2i	getFrameSize() const;
		int32_t	getKey(const int32_t value) const;
		int32_t	getInputMode() const;
		int32_t	getMouseButton(const int32_t button) const;
		double	getTime() const;

		/* * * * * * * * * * * * * * * * * * * * */
		
		inline const Context &	getContext()	const { return m_context; }
		inline const uint32_t &	getStyle()		const { return m_style; }
		inline const vec2i &	getPosition()	const { return m_position; }
		inline const Screen &	getScreen()		const { return m_screen; }
		inline const vec2u &	getSize()		const { return getScreen().resolution; }
		inline const String &	getTitle()		const { return m_title; }
		inline const uint32_t	getWidth()		const { return getSize()[0]; }
		inline const uint32_t	getHeight()		const { return getSize()[1]; }
		inline const int32_t	getFrameWidth()	const { return getFrameSize()[0]; }
		inline const int32_t	getFrameHeight()const { return getFrameSize()[1]; }
		inline const float		getAspect()		const { return ML_ASPECT(getWidth(), getHeight()); };
		inline const float		getFrameAspect()const { return ML_ASPECT(getFrameWidth(), getFrameHeight()); };


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
		Context			m_context;
		Screen			m_screen;
		uint32_t		m_style;
		vec2i			m_position;
		String			m_title;
		mutable char	m_char;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_WINDOW_HPP_