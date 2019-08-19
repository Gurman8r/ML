#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLFW/glfw3.h>

# ifdef APIENTRY
# 	undef APIENTRY
# endif

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif

#define ML_WINDOW(ptr)	static_cast<GLFWwindow *>(ptr)
#define ML_MONITOR(ptr) static_cast<GLFWmonitor *>(ptr)
#define ML_CURSOR(ptr)	static_cast<GLFWcursor *>(ptr)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	static const GLFWimage & cache_GLFWimage(uint32_t w, uint32_t h, const uint8_t * pixels)
	{
		static HashMap<const uint8_t *, GLFWimage> cache;
		auto it = cache.find(pixels);
		if (it == cache.end())
		{
			it = cache.insert({
				pixels,
				GLFWimage { (int32_t)w, (int32_t)h, (uint8_t *)pixels }
			}).first;
		}
		return it->second;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window(EventSystem & eventSystem)
		: I_EventListener	{ eventSystem }
		, m_window			{ nullptr }
		, m_monitor			{ nullptr }
		, m_share			{ nullptr }
		, m_title			{}
		, m_context			{}
		, m_style			{}
		, m_videoMode		{}
	{
#ifdef ML_SYSTEM_WINDOWS
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
		this->dispose();

#ifdef ML_SYSTEM_WINDOWS
		if (HWND window = GetConsoleWindow())
		{
			if (HMENU menu = GetSystemMenu(window, false))
			{
				EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			}
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::create(const String & title, const VideoMode & videoMode, const WindowStyle & style, const ContextSettings & context)
	{
		if (m_window)
		{
			return Debug::logError("Window already initialized");
		}

		// Initialize
		if (!glfwInit())
		{
			return Debug::logError("Failed initializing GLFW");
		}

		// Set Data
		m_title		= title;
		m_videoMode = videoMode;
		m_context	= context;
		m_style		= style;

		// Context Settings
		glfwWindowHint(GLFW_CLIENT_API,				GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	m_context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	m_context.minor);
		glfwWindowHint(GLFW_DEPTH_BITS,				m_context.depthBits);
		glfwWindowHint(GLFW_STENCIL_BITS,			m_context.stencilBits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,			m_context.srgbCapable);

		// Profile
		static Array<int32_t, 4> profiles 
		{
			GLFW_OPENGL_ANY_PROFILE,
			GLFW_OPENGL_CORE_PROFILE,
			GLFW_OPENGL_COMPAT_PROFILE,
			GLFW_OPENGL_DEBUG_CONTEXT
		};
		glfwWindowHint(GLFW_OPENGL_PROFILE, profiles[m_context.profile]);

		// Style Settings
		glfwWindowHint(GLFW_RESIZABLE,		m_style.resizable);
		glfwWindowHint(GLFW_VISIBLE,		m_style.visible);
		glfwWindowHint(GLFW_DECORATED,		m_style.decorated);
		glfwWindowHint(GLFW_FOCUSED,		m_style.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY,	m_style.autoIconify);
		glfwWindowHint(GLFW_FLOATING,		m_style.floating);
		glfwWindowHint(GLFW_MAXIMIZED,		m_style.maximized);

		// Create Window
		if (m_window = ML_WINDOW(glfwCreateWindow(
			getWidth(), 
			getHeight(), 
			getTitle().c_str(),
			ML_MONITOR(m_monitor = m_style.fullscreen ? glfwGetPrimaryMonitor() : nullptr),
			ML_WINDOW(m_share = nullptr)
		)))
		{
			makeContextCurrent();

			if (setup())
			{
				setCursorMode(Cursor::Mode::Normal);

				if (!getStyle().fullscreen)
				{
					if (getStyle().maximized)
					{
						maximize(); // Maximized
					}
					else
					{
						setCentered(); // Centered
					}
				}
				
				return true;
			}
		}
		return false;
	}

	bool Window::setup()
	{
		static EventSystem * evSys = nullptr;
		evSys = &this->eventSystem();

		setCharCallback([](void *, uint32_t c)
		{
			evSys->fireEvent(CharEvent(c));
		});

		setCursorEnterCallback([](void *, int32_t entered)
		{
			evSys->fireEvent(CursorEnterEvent(entered));
		});

		setCursorPosCallback([](void *, float64_t x, float64_t y)
		{
			evSys->fireEvent(CursorPosEvent(x, y));
		});

		setErrorCallback([](int32_t code, C_String desc)
		{
#if ML_DEBUG
			cerr << "GLFW Error " << code << ": \'" << desc << "\'" << endl;
#endif
			evSys->fireEvent(WindowErrorEvent(code, desc));
		});

		setFrameSizeCallback([](void *, int32_t w, int32_t h)
		{
			evSys->fireEvent(FrameSizeEvent(w, h));
		});

		setKeyCallback([](void *, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			evSys->fireEvent(KeyEvent(button, scan, action, {
				(bool)(mods & ML_MOD_SHIFT),
				(bool)(mods & ML_MOD_CTRL),
				(bool)(mods & ML_MOD_ALT),
				(bool)(mods & ML_MOD_SUPER)
				}));
		});

		setMouseButtonCallback([](void *, int32_t button, int32_t action, int32_t mods)
		{
			evSys->fireEvent(MouseButtonEvent(button, action, mods));
		});
		
		setScrollCallback([](void *, float64_t x, float64_t y)
		{
			evSys->fireEvent(ScrollEvent(x, y));
		});

		setWindowCloseCallback([](void *)
		{
			evSys->fireEvent(WindowCloseEvent());
		});

		setWindowFocusCallback([](void *, int32_t focused)
		{
			evSys->fireEvent(WindowFocusEvent(focused));
		});
		
		setWindowPosCallback([](void *, int32_t x, int32_t y)
		{
			evSys->fireEvent(WindowPosEvent(x, y));
		});

		setWindowSizeCallback([](void *, int32_t width, int32_t height)
		{
			evSys->fireEvent(WindowSizeEvent(width, height));
		});

		return true;
	}

	void Window::onEvent(const Event & value)
	{
		switch (*value)
		{
		case CharEvent::ID:
			if (auto ev = value.as<CharEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case CursorEnterEvent::ID:
			if (auto ev = value.as<CursorEnterEvent>()) {}
			break;
		case CursorPosEvent::ID:
			if (auto ev = value.as<CursorPosEvent>()) {}
			break;
		case FrameSizeEvent::ID:
			if (auto ev = value.as<FrameSizeEvent>()) {}
			break;
		case KeyEvent::ID:
			if (auto ev = value.as<KeyEvent>()) {}
			break;
		case MouseButtonEvent::ID:
			if (auto ev = value.as<MouseButtonEvent>()) {}
			break;
		case ScrollEvent::ID:
			if (auto ev = value.as<ScrollEvent>()) {}
			break;
		case WindowCloseEvent::ID:
			if (auto ev = value.as<WindowCloseEvent>()) {}
			break;
		case WindowErrorEvent::ID:
			if (auto ev = value.as<WindowErrorEvent>())
			{
				Debug::logError("GLFW Error {0}: \'{1}\'", ev->code, ev->desc);
			}
			break;
		case WindowFocusEvent::ID:
			if (auto ev = value.as<WindowFocusEvent>()) {}
			break;
		case WindowKillEvent::ID:
			if (auto ev = value.as<WindowKillEvent>()) 
			{
				this->close();
			}
			break;
		case WindowPosEvent::ID:
			if (auto ev = value.as<WindowPosEvent>()) {}
			break;
		case WindowSizeEvent::ID:
			if (auto ev = value.as<WindowSizeEvent>())
			{
				m_videoMode.resolution = { (uint32_t)ev->width, (uint32_t)ev->height };
			}
			break;
		}
	}

	bool Window::dispose()
	{
		this->destroy();
		this->terminate();

		m_window	= nullptr;
		m_monitor	= nullptr;
		m_share		= nullptr;

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		if (m_window) (glfwSetWindowShouldClose(
			ML_WINDOW(m_window),
			true
		));
		return (*this);
	}

	Window & Window::destroy()
	{
		if (m_window) (glfwDestroyWindow(
			ML_WINDOW(m_window)
		));
		return (*this);
	}

	Window & Window::iconify()
	{
		if (m_window) (glfwIconifyWindow(
			ML_WINDOW(m_window)
		));
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		if (m_window) (glfwMakeContextCurrent(
			ML_WINDOW(m_window)
		));
		return (*this);
	}
	
	Window & Window::maximize()
	{
		if (m_window) (glfwMaximizeWindow(
			ML_WINDOW(m_window)
		));
		return (*this);
	}

	Window & Window::pollEvents()
	{
		if (m_window) (glfwPollEvents(
		));
		return (*this);
	}

	Window & Window::restore()
	{
		if (m_window) (glfwRestoreWindow(
			ML_WINDOW(m_window)
		));
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		if (m_window) glfwSwapBuffers(
			ML_WINDOW(m_window)
		);
		return (*this);
	}

	Window & Window::swapInterval(const int32_t value)
	{
		if (m_window) (glfwSwapInterval(
			value
		));
		return (*this);
	}

	Window & Window::setCentered()
	{
		return setPosition(
			(vec2i)(VideoMode::get_desktop_mode().resolution - getSize()) / 2
		);
	}

	Window & Window::setClipboardString(const String & value)
	{
		if (m_window) (glfwSetClipboardString(
			ML_WINDOW(m_window),
			value.c_str()
		));
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		if (m_window) glfwSetCursor(
			ML_WINDOW(m_window), 
			static_cast<GLFWcursor*>(value)
		);
		return (*this);
	}
	
	Window & Window::setCursorMode(const Cursor::Mode value)
	{
		if (m_window) (glfwSetInputMode(
			ML_WINDOW(m_window),
			GLFW_CURSOR,
			static_cast<int32_t>(value)
		));
		return (*this);
	}

	Window & Window::setCursorPos(const vec2i & value)
	{
		if (m_window) (glfwSetCursorPos(
			ML_WINDOW(m_window),
			value[0],
			value[1]
		));
		return (*this);
	}

	Window & Window::setIcon(uint32_t w, uint32_t h, const uint8_t * pixels)
	{
		if (m_window)
		{
			glfwSetWindowIcon(
				ML_WINDOW(m_window),
				1,
				&cache_GLFWimage(w, h, pixels)
			);
		}
		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		if (m_window) (glfwSetWindowPos(
			ML_WINDOW(m_window),
			value[0],
			value[1]
		));
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		if (m_window) (glfwSetWindowSize(
			ML_WINDOW(m_window),
			m_videoMode.resolution[0] = value[0],
			m_videoMode.resolution[1] = value[1]
		));
		return (*this);
	}

	Window & Window::setTitle(const String & value)
	{
		if (m_window) (glfwSetWindowTitle(
			ML_WINDOW(m_window),
			(m_title = value).c_str()
		));
		return (*this);
	}
	
	Window & Window::terminate()
	{
		if (m_window) glfwTerminate();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::isFocused() const
	{
		return getAttribute(GLFW_FOCUSED);
	}

	bool Window::isOpen() const
	{
		return m_window && (!glfwWindowShouldClose(
			ML_WINDOW(m_window)
		));
	}
	
	int32_t Window::getAttribute(int32_t value) const
	{
		return (m_window
			? glfwGetWindowAttrib(
				ML_WINDOW(m_window), value)
			: NULL);
	}

	char Window::getChar() const
	{
		char temp = m_char;
		m_char = '\0';
		return m_char;
	}

	C_String Window::getClipboardString() const
	{
		return (m_window
			? glfwGetClipboardString(
			ML_WINDOW(m_window))
			: nullptr);
	}

	vec2 Window::getCursorPos() const
	{
		static vec2d temp;
		if (m_window) (glfwGetCursorPos(
			ML_WINDOW(m_window),
			&temp[0],
			&temp[1]
		));
		return (vec2)temp;
	}

	vec2i Window::getFrameSize() const
	{
		static vec2i temp;
		if (m_window) (glfwGetFramebufferSize(
			ML_WINDOW(m_window),
			&temp[0],
			&temp[1]
		));
		return temp;
	}

	int32_t	Window::getKey(const int32_t value) const
	{
		return (m_window
			? glfwGetKey(
				ML_WINDOW(m_window),
				value)
			: NULL
		);
	}

	int32_t Window::getInputMode() const
	{
		return (m_window
			? glfwGetInputMode(
				ML_WINDOW(m_window),
				GLFW_CURSOR)
			: NULL
		);
	}

	int32_t	Window::getMouseButton(const int32_t value) const
	{
		return (m_window
			? glfwGetMouseButton(
				ML_WINDOW(m_window),
				value)
			: NULL
		);
	}

	vec2i Window::getPosition() const
	{
		vec2i temp;
		if (m_window)
		{
			glfwGetWindowPos(ML_WINDOW(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	float64_t Window::getTime() const
	{
		return (m_window 
			? glfwGetTime()
			: 0.0
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Window::createCustomCursor(uint32_t w, uint32_t h, const uint8_t * pixels) const
	{
		return (m_window
			? glfwCreateCursor(&cache_GLFWimage(w, h, pixels), w, h)
			: nullptr
		);
	}

	void * Window::createStandardCursor(Cursor::Shape value) const
	{
		return (m_window
			? glfwCreateStandardCursor(static_cast<int32_t>(value))
			: nullptr
		);
	}

	void Window::destroyCursor(void * value) const
	{
		return (m_window
			? glfwDestroyCursor(ML_CURSOR(value))
			: void()
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::CharFun Window::setCharCallback(CharFun callback)
	{
		return (m_window
			? glfwSetCharCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcharfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun callback)
	{
		return (m_window
			? glfwSetCursorEnterCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcursorenterfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun callback)
	{
		return (m_window
			? glfwSetCursorPosCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcursorposfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::ErrorFun Window::setErrorCallback(ErrorFun callback)
	{
		return (m_window
			? glfwSetErrorCallback(
				reinterpret_cast<GLFWerrorfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}

	Window::FrameSizeFun Window::setFrameSizeCallback(FrameSizeFun callback)
	{
		return (m_window
			? glfwSetFramebufferSizeCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun callback)
	{
		return (m_window
			? glfwSetKeyCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWkeyfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun callback)
	{
		return (m_window
			? glfwSetMouseButtonCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun callback)
	{
		return (m_window
			? glfwSetScrollCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWscrollfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun callback)
	{
		return (m_window
			? glfwSetWindowCloseCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowclosefun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun callback)
	{
		return (m_window
			? glfwSetWindowFocusCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun callback)
	{
		return (m_window
			? glfwSetWindowPosCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowposfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun callback)
	{
		return (m_window
			? glfwSetWindowSizeCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowposfun>(callback)) ? callback : nullptr
			: nullptr
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}