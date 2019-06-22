#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Cache.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLFW/glfw3.h>

# if defined(APIENTRY)
# 	undef APIENTRY
# endif

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	inline static const GLFWimage & MapGLFWimage(const Image & value)
	{
		static HashCache<const uint8_t *, GLFWimage> cache;
		return cache(
			&value.pixels()[0], 
			GLFWimage {
				(int32_t)value.width(),
				(int32_t)value.height(),
				(uint8_t *)(value.pixelsPtr())
			});
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window(EventSystem & eventSystem)
		: EventListener	(eventSystem)
		, m_window		(nullptr)
		, m_monitor		(nullptr)
		, m_share		(nullptr)
		, m_title		(String())
		, m_context		(ContextSettings())
		, m_style		(WindowStyle())
		, m_video		(VideoSettings())
	{
#ifdef ML_SYSTEM_WINDOWS
		// Disable CMD Close Button
		if (HWND window = GetConsoleWindow())
		{
			if (HMENU menu = GetSystemMenu(window, false))
			{
				EnableMenuItem(menu, SC_CLOSE, MF_GRAYED);
			}
		}
#endif
		eventSystem.addListener(CharEvent::ID,			this);
		eventSystem.addListener(CursorEnterEvent::ID,	this);
		eventSystem.addListener(CursorPosEvent::ID,		this);
		eventSystem.addListener(FrameSizeEvent::ID,		this);
		eventSystem.addListener(KeyEvent::ID,			this);
		eventSystem.addListener(MouseButtonEvent::ID,	this);
		eventSystem.addListener(ScrollEvent::ID,		this);
		eventSystem.addListener(WindowCloseEvent::ID,	this);
		eventSystem.addListener(WindowErrorEvent::ID,	this);
		eventSystem.addListener(WindowFocusEvent::ID,	this);
		eventSystem.addListener(WindowKillEvent::ID,	this);
		eventSystem.addListener(WindowSizeEvent::ID,	this);
		eventSystem.addListener(WindowPosEvent::ID,		this);
	}
	
	Window::~Window() 
	{
		this->destroy();

		this->terminate();

#ifdef ML_SYSTEM_WINDOWS
		// Re-Enable CMD Close Button
		if (HWND window = GetConsoleWindow())
		{
			if (HMENU menu = GetSystemMenu(window, false))
			{
				EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			}
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::create(const String & title, const VideoSettings & video, const WindowStyle & style, const ContextSettings & context)
	{
		m_title		= title;
		m_video		= video;
		m_context	= context;
		m_style		= style;

		if (glfwInit() == GLFW_TRUE)
		{
			// Context Settings
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_context.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_context.minorVersion);

			glfwWindowHint(GLFW_DEPTH_BITS,		m_context.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	m_context.stencilBits);
			glfwWindowHint(GLFW_SRGB_CAPABLE,	m_context.srgbCapable);
			
			switch (m_context.profile)
			{
			case 1	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); break;
			case 2	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); break;
			default	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);	break;
			}

			// Style Settings
			glfwWindowHint(GLFW_RESIZABLE,		m_style.resizable);
			glfwWindowHint(GLFW_VISIBLE,		m_style.visible);
			glfwWindowHint(GLFW_DECORATED,		m_style.decorated);
			glfwWindowHint(GLFW_FOCUSED,		m_style.focused);
			glfwWindowHint(GLFW_AUTO_ICONIFY,	m_style.autoIconify);
			glfwWindowHint(GLFW_FLOATING,		m_style.floating);
			glfwWindowHint(GLFW_MAXIMIZED,		m_style.maximized);

			// Create Window
			if (m_window = static_cast<GLFWwindow *>(glfwCreateWindow(
				getWidth(), 
				getHeight(), 
				m_title.c_str(),
				static_cast<GLFWmonitor *>(m_monitor = nullptr),
				static_cast<GLFWwindow *>(m_share = nullptr))))
			{
				makeContextCurrent();

				return setup();
			}
			else
			{
				return Debug::logError("Failed to Create GLFW Window");
			}
		}
		else
		{
			return Debug::logError("Failed to Initialize GLFW");
		}
	}

	bool Window::setup()
	{
		static EventSystem * evs { &this->eventSystem() };
		if (!evs) { return false; }

		setCharCallback([](void * window, uint32_t c)
		{
			evs->fireEvent(CharEvent(c));
		});

		setCursorEnterCallback([](void * window, int32_t entered)
		{
			evs->fireEvent(CursorEnterEvent(entered));
		});

		setCursorPosCallback([](void * window, double x, double y)
		{
			evs->fireEvent(CursorPosEvent(x, y));
		});

		setErrorCallback([](int32_t code, CString desc)
		{
			evs->fireEvent(WindowErrorEvent(code, desc));
		});

		setFrameSizeCallback([](void * window, int32_t w, int32_t h)
		{
			evs->fireEvent(FrameSizeEvent(w, h));
		});

		setKeyCallback([](void * window, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			evs->fireEvent(KeyEvent(button, scan, action, {
				(bool)(mods & ML_MOD_SHIFT),
				(bool)(mods & ML_MOD_CTRL),
				(bool)(mods & ML_MOD_ALT),
				(bool)(mods & ML_MOD_SUPER)
				}));
		});

		setMouseButtonCallback([](void * window, int32_t button, int32_t action, int32_t mods)
		{
			evs->fireEvent(MouseButtonEvent(button, action, mods));
		});
		
		setScrollCallback([](void * window, double x, double y)
		{
			evs->fireEvent(ScrollEvent(x, y));
		});

		setWindowCloseCallback([](void * window)
		{
			evs->fireEvent(WindowCloseEvent());
		});

		setWindowFocusCallback([](void * window, int32_t focused)
		{
			evs->fireEvent(WindowFocusEvent(focused));
		});
		
		setWindowPosCallback([](void * window, int32_t x, int32_t y)
		{
			evs->fireEvent(WindowPosEvent(x, y));
		});

		setWindowSizeCallback([](void * window, int32_t width, int32_t height)
		{
			evs->fireEvent(WindowSizeEvent(width, height));
		});

		return true;
	}

	void Window::onEvent(const Event * value)
	{
		switch (*value)
		{
		case CharEvent::ID:
			if (auto ev = value->as<CharEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case CursorEnterEvent::ID:
			if (auto ev = value->as<CursorEnterEvent>())
			{
			}
			break;
		case CursorPosEvent::ID:
			if (auto ev = value->as<CursorPosEvent>())
			{
			}
			break;
		case FrameSizeEvent::ID:
			if (auto ev = value->as<FrameSizeEvent>())
			{
			}
			break;
		case KeyEvent::ID:
			if (auto ev = value->as<KeyEvent>())
			{
			}
			break;
		case MouseButtonEvent::ID:
			if (auto ev = value->as<MouseButtonEvent>())
			{
			}
			break;
		case ScrollEvent::ID:
			if (auto ev = value->as<ScrollEvent>())
			{
			}
			break;
		case WindowCloseEvent::ID:
			if (auto ev = value->as<WindowCloseEvent>())
			{
			}
			break;
		case WindowErrorEvent::ID:
			if (auto ev = value->as<WindowErrorEvent>())
			{
				Debug::logError("GLFW : {0} : {1}", ev->code, ev->desc);
			}
			break;
		case WindowFocusEvent::ID:
			if (auto ev = value->as<WindowFocusEvent>())
			{
			}
			break;
		case WindowKillEvent::ID:
			if (auto ev = value->as<WindowKillEvent>())
			{
				this->close();
			}
			break;
		case WindowPosEvent::ID:
			if (auto ev = value->as<WindowPosEvent>())
			{
			}
			break;
		case WindowSizeEvent::ID:
			if (auto ev = value->as<WindowSizeEvent>())
			{
				m_video.resolution = { (uint32_t)ev->width, (uint32_t)ev->height };
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		glfwSetWindowShouldClose(
			static_cast<GLFWwindow *>(m_window), 
			GLFW_TRUE
		);
		return (*this);
	}

	Window & Window::destroy()
	{
		glfwDestroyWindow(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}

	Window & Window::iconify()
	{
		glfwIconifyWindow(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		glfwMakeContextCurrent(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}
	
	Window & Window::maximize()
	{
		glfwMaximizeWindow(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}

	Window & Window::pollEvents()
	{
		glfwPollEvents();
		return (*this);
	}

	Window & Window::restore()
	{
		glfwRestoreWindow(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(
			static_cast<GLFWwindow *>(m_window)
		);
		return (*this);
	}

	Window & Window::swapInterval(const int32_t value)
	{
		glfwSwapInterval(value);
		return (*this);
	}

	Window & Window::setClipboardString(const String & value)
	{
		glfwSetClipboardString(
			static_cast<GLFWwindow *>(m_window), 
			value.c_str()
		);
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		glfwSetCursor(
			static_cast<GLFWwindow *>(m_window), 
			static_cast<GLFWcursor*>(value)
		);
		return (*this);
	}
	
	Window & Window::seCursorMode(const Cursor::Mode value)
	{
		glfwSetInputMode(
			static_cast<GLFWwindow *>(m_window),
			GLFW_CURSOR, 
			static_cast<int32_t>(value)
		);
		return (*this);
	}

	Window & Window::setCursorPos(const vec2i & value)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow *>(m_window), 
			value[0], 
			value[1]
		);
		return (*this);
	}

	Window & Window::setIcons(const List<Image> & value)
	{
		List<GLFWimage> temp;

		for (const auto & e : value)
		{
			temp.push_back(MapGLFWimage(e));
		}

		glfwSetWindowIcon(
			static_cast<GLFWwindow *>(m_window),
			(uint32_t)temp.size(),
			&temp[0]
		);

		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		glfwSetWindowPos(
			static_cast<GLFWwindow *>(m_window),
			value[0],
			value[1]
		);
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		m_video.resolution = value;
		glfwSetWindowSize(
			static_cast<GLFWwindow *>(m_window), 
			value[0], 
			value[1]
		);
		return (*this);
	}

	Window & Window::setTitle(const String & value)
	{
		m_title = value;
		glfwSetWindowTitle(
			static_cast<GLFWwindow *>(m_window),
			value.c_str()
		);
		return (*this);
	}
	
	Window & Window::terminate()
	{
		glfwTerminate();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool	Window::isFocused() const
	{
		return getAttribute(GLFW_FOCUSED);
	}

	bool	Window::isOpen() const
	{
		return !glfwWindowShouldClose(
			static_cast<GLFWwindow *>(m_window)
		);
	}
	
	int32_t Window::getAttribute(const int32_t value) const
	{
		return glfwGetWindowAttrib(
			static_cast<GLFWwindow *>(m_window), value
		);
	}

	char	Window::getChar() const
	{
		static char temp;
		std::swap(m_char, temp);
		return temp;
	}

	CString Window::getClipboardString() const
	{
		return (glfwGetClipboardString(
			static_cast<GLFWwindow *>(m_window)
		));
	}

	vec2	Window::getCursorPos() const
	{
		static vec2d temp;
		glfwGetCursorPos(
			static_cast<GLFWwindow *>(m_window), 
			&temp[0],
			&temp[1]
		);
		return (vec2)temp;
	}

	vec2i	Window::getFrameSize() const
	{
		static vec2i temp;
		glfwGetFramebufferSize(
			static_cast<GLFWwindow *>(m_window), 
			&temp[0], 
			&temp[1]
		);
		return temp;
	}

	int32_t	Window::getKey(const int32_t value) const
	{
		return glfwGetKey(
			static_cast<GLFWwindow *>(m_window),
			value
		);
	}

	int32_t Window::getInputMode() const
	{
		return glfwGetInputMode(
			static_cast<GLFWwindow *>(m_window),
			GLFW_CURSOR
		);
	}

	int32_t	Window::getMouseButton(const int32_t value) const
	{
		return glfwGetMouseButton(
			static_cast<GLFWwindow *>(m_window),
			value
		);
	}

	vec2i	Window::getPosition() const
	{
		int32_t x, y;
		glfwGetWindowPos(static_cast<GLFWwindow *>(m_window), &x, &y);
		return vec2i(x, y);
	}

	double	Window::getTime() const
	{
		return glfwGetTime();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Window::createCustomCursor(const Image & image) const
	{
		return glfwCreateCursor(
			&MapGLFWimage(image), 
			(int32_t)image.width(),
			(int32_t)image.height()
		);
	}

	void * Window::createStandardCursor(Cursor::Shape value) const
	{
		return glfwCreateStandardCursor(static_cast<int32_t>(value));
	}

	void Window::destroyCursor(void * value) const
	{
		glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::CharFun Window::setCharCallback(CharFun callback)
	{
		return glfwSetCharCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWcharfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun callback)
	{
		return glfwSetCursorEnterCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWcursorenterfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun callback)
	{
		return glfwSetCursorPosCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWcursorposfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::ErrorFun Window::setErrorCallback(ErrorFun callback)
	{
		return glfwSetErrorCallback(
			reinterpret_cast<GLFWerrorfun>(callback))
				? (callback)
				: (nullptr);
	}

	Window::FrameSizeFun Window::setFrameSizeCallback(FrameSizeFun callback)
	{
		return glfwSetFramebufferSizeCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWframebuffersizefun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun callback)
	{
		return glfwSetKeyCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWkeyfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun callback)
	{
		return glfwSetMouseButtonCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWmousebuttonfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun callback)
	{
		return glfwSetScrollCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWscrollfun>(callback))
				? (callback) 
				: (nullptr);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun callback)
	{
		return glfwSetWindowCloseCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowclosefun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun callback)
	{
		return glfwSetWindowFocusCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowfocusfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun callback)
	{
		return glfwSetWindowPosCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowposfun>(callback))
				? (callback)
				: (nullptr);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun callback)
	{
		return glfwSetWindowSizeCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowposfun>(callback))
				? (callback)
				: (nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}