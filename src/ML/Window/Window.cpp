#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>

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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static EventSystem * s_EventSystem = NULL;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window(EventSystem & eventSystem)
		: EventListener(eventSystem)
		, m_window		(NULL)
		, m_monitor		(NULL)
		, m_share		(NULL)
		, m_title		(String())
		, m_context		(ContextSettings())
		, m_style		(StyleSettings())
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
		s_EventSystem = &eventSystem;

		eventSystem.addListener(CharEvent::ID, this);
		eventSystem.addListener(CursorEnterEvent::ID, this);
		eventSystem.addListener(CursorPosEvent::ID, this);
		eventSystem.addListener(FrameSizeEvent::ID, this);
		eventSystem.addListener(KeyEvent::ID, this);
		eventSystem.addListener(MouseButtonEvent::ID, this);
		eventSystem.addListener(ScrollEvent::ID, this);
		eventSystem.addListener(WindowCloseEvent::ID, this);
		eventSystem.addListener(WindowErrorEvent::ID, this);
		eventSystem.addListener(WindowFocusEvent::ID, this);
		eventSystem.addListener(WindowKillEvent::ID, this);
		eventSystem.addListener(WindowSizeEvent::ID, this);
		eventSystem.addListener(WindowPosEvent::ID, this);
	}
	
	Window::~Window() 
	{
		this->destroy();

		this->terminate();

		s_EventSystem = NULL;

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

	bool Window::create(const String & title, const VideoSettings & screen, const StyleSettings & style, const ContextSettings & context)
	{
		m_title		= title;
		m_video	= screen;
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
			case 1	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	break;
			case 2	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	break;
			default	: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);		break;
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
				static_cast<GLFWmonitor *>(m_monitor = NULL),
				static_cast<GLFWwindow *>(m_share = NULL))))
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
		if (s_EventSystem)
		{
			setCharCallback([](void * window, uint32_t c)
			{
				s_EventSystem->fireEvent(CharEvent(c));
			});

			setCursorEnterCallback([](void * window, int32_t entered)
			{
				s_EventSystem->fireEvent(CursorEnterEvent(entered));
			});

			setCursorPosCallback([](void * window, double x, double y)
			{
				s_EventSystem->fireEvent(CursorPosEvent(x, y));
			});

			setErrorCallback([](int32_t code, CString desc)
			{
				s_EventSystem->fireEvent(WindowErrorEvent(code, desc));
			});

			setFrameSizeCallback([](void * window, int32_t w, int32_t h)
			{
				s_EventSystem->fireEvent(FrameSizeEvent(w, h));
			});

			setKeyCallback([](void * window, int32_t button, int32_t scan, int32_t action, int32_t mods)
			{
				s_EventSystem->fireEvent(KeyEvent(button, scan, action,
					(bool)(mods & ML_MOD_SHIFT),
					(bool)(mods & ML_MOD_CTRL),
					(bool)(mods & ML_MOD_ALT),
					(bool)(mods & ML_MOD_SUPER)
				));
			});

			setMouseButtonCallback([](void * window, int32_t button, int32_t action, int32_t mods)
			{
				s_EventSystem->fireEvent(MouseButtonEvent(button, action, mods));
			});
		
			setScrollCallback([](void * window, double x, double y)
			{
				s_EventSystem->fireEvent(ScrollEvent(x, y));
			});

			setWindowCloseCallback([](void * window)
			{
				s_EventSystem->fireEvent(WindowCloseEvent());
			});

			setWindowFocusCallback([](void * window, int32_t focused)
			{
				s_EventSystem->fireEvent(WindowFocusEvent(focused));
			});
		
			setWindowPosCallback([](void * window, int32_t x, int32_t y)
			{
				s_EventSystem->fireEvent(WindowPosEvent(x, y));
			});

			setWindowSizeCallback([](void * window, int32_t width, int32_t height)
			{
				s_EventSystem->fireEvent(WindowSizeEvent(width, height));
			});

			return true;
		}
		return false;
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
				Debug::logError("{0}", (*ev));
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
			value
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

	Window & Window::setIcons(const List<Icon> & value)
	{
		const uint32_t count = (uint32_t)value.size();

		std::vector<GLFWimage> temp(count);

		for (size_t i = 0; i < count; i++)
		{
			temp[i] = GLFWimage {
				value[i].width,
				value[i].height,
				value[i].pixels,
			};
		}

		glfwSetWindowIcon(
			static_cast<GLFWwindow *>(m_window),
			count,
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
		return getAttrib(GLFW_FOCUSED);
	}

	bool	Window::isOpen() const
	{
		return !glfwWindowShouldClose(
			static_cast<GLFWwindow *>(m_window)
		);
	}
	
	int32_t Window::getAttrib(const int32_t value) const
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
		return static_cast<Cursor::Mode>(glfwGetInputMode(
			static_cast<GLFWwindow *>(m_window), 
			GLFW_CURSOR
		));
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

	void * Window::createCursor(uint32_t value) const
	{
		return glfwCreateStandardCursor(value);
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
				: (NULL);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun callback)
	{
		return glfwSetCursorEnterCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWcursorenterfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun callback)
	{
		return glfwSetCursorPosCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWcursorposfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::ErrorFun Window::setErrorCallback(ErrorFun callback)
	{
		return glfwSetErrorCallback(
			reinterpret_cast<GLFWerrorfun>(callback))
				? (callback)
				: (NULL);
	}

	Window::FrameSizeFun Window::setFrameSizeCallback(FrameSizeFun callback)
	{
		return glfwSetFramebufferSizeCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWframebuffersizefun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun callback)
	{
		return glfwSetKeyCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWkeyfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun callback)
	{
		return glfwSetMouseButtonCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWmousebuttonfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun callback)
	{
		return glfwSetScrollCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWscrollfun>(callback))
				? (callback) 
				: (NULL);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun callback)
	{
		return glfwSetWindowCloseCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowclosefun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun callback)
	{
		return glfwSetWindowFocusCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowfocusfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun callback)
	{
		return glfwSetWindowPosCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowposfun>(callback))
				? (callback)
				: (NULL);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun callback)
	{
		return glfwSetWindowSizeCallback(
			static_cast<GLFWwindow *>(m_window), 
			reinterpret_cast<GLFWwindowposfun>(callback))
				? (callback)
				: (NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}