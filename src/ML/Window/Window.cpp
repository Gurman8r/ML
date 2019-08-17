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
	static inline const GLFWimage & MapGLFWimage(const Image & value)
	{
		static HashMap<const uint8_t *, GLFWimage> cache;
		auto it = cache.find(value.data());
		if (it != cache.end())
		{
			return it->second;
		}
		return cache.insert({ 
			value.data(), 
			{ (int32_t)value.width(), (int32_t)value.height(), (uint8_t *)value.data()} 
		}).first->second;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window(EventSystem & eventSystem)
		: I_EventListener	(eventSystem)
		, m_window		(nullptr)
		, m_monitor		(nullptr)
		, m_share		(nullptr)
		, m_title		(String())
		, m_context		(ContextSettings())
		, m_style		(WindowStyle())
		, m_videoMode		(VideoMode())
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

	bool Window::create(const String & title, const VideoMode & video, const WindowStyle & style, const ContextSettings & context)
	{
		m_title		= title;
		m_videoMode		= video;
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
			case ContextSettings::Any:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
				break;
			case ContextSettings::Core:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
				break;
			case ContextSettings::Compat: 
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); 
				break;
			default:
			case ContextSettings::Debug:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_DEBUG_CONTEXT);
				break;
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
				getTitle().c_str(),
				static_cast<GLFWmonitor *>(m_monitor = nullptr),
				static_cast<GLFWwindow *>(m_share = nullptr)))
			)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		if (m_window) (glfwSetWindowShouldClose(
			static_cast<GLFWwindow *>(m_window),
			true
		));
		return (*this);
	}

	Window & Window::destroy()
	{
		if (m_window) (glfwDestroyWindow(
			static_cast<GLFWwindow *>(m_window)
		));
		return (*this);
	}

	Window & Window::iconify()
	{
		if (m_window) (glfwIconifyWindow(
			static_cast<GLFWwindow *>(m_window)
		));
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		if (m_window) (glfwMakeContextCurrent(
			static_cast<GLFWwindow *>(m_window)
		));
		return (*this);
	}
	
	Window & Window::maximize()
	{
		if (m_window) (glfwMaximizeWindow(
			static_cast<GLFWwindow *>(m_window)
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
			static_cast<GLFWwindow *>(m_window)
		));
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		if (m_window) glfwSwapBuffers(
			static_cast<GLFWwindow *>(m_window)
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

	Window & Window::setClipboardString(const String & value)
	{
		if (m_window) (glfwSetClipboardString(
			static_cast<GLFWwindow *>(m_window),
			value.c_str()
		));
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		if (m_window) glfwSetCursor(
			static_cast<GLFWwindow *>(m_window), 
			static_cast<GLFWcursor*>(value)
		);
		return (*this);
	}
	
	Window & Window::setCursorMode(const Cursor::Mode value)
	{
		if (m_window) (glfwSetInputMode(
			static_cast<GLFWwindow *>(m_window),
			GLFW_CURSOR,
			static_cast<int32_t>(value)
		));
		return (*this);
	}

	Window & Window::setCursorPos(const vec2i & value)
	{
		if (m_window) (glfwSetCursorPos(
			static_cast<GLFWwindow *>(m_window),
			value[0],
			value[1]
		));
		return (*this);
	}

	Window & Window::setIcons(const List<Image> & value)
	{
		List<GLFWimage> temp;
		temp.reserve(value.size());
		for (const auto & e : value)
		{
			temp.push_back(MapGLFWimage(e));
		}

		if (m_window) (glfwSetWindowIcon(
			static_cast<GLFWwindow *>(m_window),
			(uint32_t)temp.size(),
			&temp[0]
		));

		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		if (m_window) (glfwSetWindowPos(
			static_cast<GLFWwindow *>(m_window),
			value[0],
			value[1]
		));
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		if (m_window) (glfwSetWindowSize(
			static_cast<GLFWwindow *>(m_window),
			m_videoMode.resolution[0] = value[0],
			m_videoMode.resolution[1] = value[1]
		));
		return (*this);
	}

	Window & Window::setTitle(const String & value)
	{
		if (m_window) (glfwSetWindowTitle(
			static_cast<GLFWwindow *>(m_window),
			(m_title = value).c_str()
		));
		return (*this);
	}
	
	Window & Window::terminate()
	{
		if (m_window) glfwTerminate();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::isFocused() const
	{
		return getAttribute(GLFW_FOCUSED);
	}

	bool Window::isOpen() const
	{
		return m_window && (!glfwWindowShouldClose(
			static_cast<GLFWwindow *>(m_window)
		));
	}
	
	int32_t Window::getAttribute(const int32_t value) const
	{
		return (m_window
			? glfwGetWindowAttrib(
				static_cast<GLFWwindow *>(m_window), value)
			: NULL);
	}

	char Window::getChar() const
	{
		static char temp;
		std::swap(m_char, temp);
		return temp;
	}

	C_String Window::getClipboardString() const
	{
		return (m_window
			? glfwGetClipboardString(
			static_cast<GLFWwindow *>(m_window))
			: nullptr);
	}

	vec2 Window::getCursorPos() const
	{
		static vec2d temp;
		if (m_window) (glfwGetCursorPos(
			static_cast<GLFWwindow *>(m_window),
			&temp[0],
			&temp[1]
		));
		return (vec2)temp;
	}

	vec2i Window::getFrameSize() const
	{
		static vec2i temp;
		if (m_window) (glfwGetFramebufferSize(
			static_cast<GLFWwindow *>(m_window),
			&temp[0],
			&temp[1]
		));
		return temp;
	}

	int32_t	Window::getKey(const int32_t value) const
	{
		return (m_window
			? glfwGetKey(
				static_cast<GLFWwindow *>(m_window),
				value)
			: NULL);
	}

	int32_t Window::getInputMode() const
	{
		return (m_window
			? glfwGetInputMode(
				static_cast<GLFWwindow *>(m_window),
				GLFW_CURSOR)
			: NULL);
	}

	int32_t	Window::getMouseButton(const int32_t value) const
	{
		return (m_window
			? glfwGetMouseButton(
				static_cast<GLFWwindow *>(m_window),
				value)
			: NULL);
	}

	vec2i Window::getPosition() const
	{
		vec2i temp;
		if (m_window) (glfwGetWindowPos(
			static_cast<GLFWwindow *>(m_window),
			&temp[0],
			&temp[1]
		));
		return temp;
	}

	float64_t Window::getTime() const
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
		if (m_window) glfwDestroyCursor(
			static_cast<GLFWcursor *>(value)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::CharFun Window::setCharCallback(CharFun callback)
	{
		return (m_window
			? (glfwSetCharCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcharfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun callback)
	{
		return (m_window
			? (glfwSetCursorEnterCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorenterfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun callback)
	{
		return (m_window
			? (glfwSetCursorPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorposfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::ErrorFun Window::setErrorCallback(ErrorFun callback)
	{
		return (m_window
			? (glfwSetErrorCallback(
				reinterpret_cast<GLFWerrorfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}

	Window::FrameSizeFun Window::setFrameSizeCallback(FrameSizeFun callback)
	{
		return (m_window
			? (glfwSetFramebufferSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun callback)
	{
		return (m_window
			? (glfwSetKeyCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWkeyfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun callback)
	{
		return (m_window
			? (glfwSetMouseButtonCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun callback)
	{
		return (m_window
			? (glfwSetScrollCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWscrollfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun callback)
	{
		return (m_window
			? (glfwSetWindowCloseCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowclosefun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun callback)
	{
		return (m_window
			? (glfwSetWindowFocusCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun callback)
	{
		return (m_window
			? (glfwSetWindowPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun callback)
	{
		return (m_window
			? (glfwSetWindowSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(callback))
				? callback
				: nullptr)
			: nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}