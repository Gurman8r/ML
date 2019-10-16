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
	static const GLFWimage & map_image(uint32_t w, uint32_t h, const uint8_t * pixels)
	{
		static HashMap<const uint8_t *, GLFWimage> cache {};
		auto it = cache.find(pixels);
		if (it == cache.end())
		{
			it = cache.insert({
				pixels, GLFWimage { (int32_t)w, (int32_t)h, (uint8_t *)pixels }
			}).first;
		}
		return it->second;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window()
		: m_window		{ nullptr }
		, m_monitor		{ nullptr }
		, m_share		{ nullptr }
		, m_title		{ String() }
		, m_context		{ ContextSettings() }
		, m_style		{ WindowStyle() }
		, m_videoMode	{ VideoMode() }
		, m_char		{ 0 }
		, m_cursorPos	{ 0 }
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
		ML_EventSystem.addListener(CharEvent::ID, this);
		ML_EventSystem.addListener(CursorEnterEvent::ID, this);
		ML_EventSystem.addListener(CursorPosEvent::ID, this);
		ML_EventSystem.addListener(FrameSizeEvent::ID, this);
		ML_EventSystem.addListener(KeyEvent::ID, this);
		ML_EventSystem.addListener(MouseButtonEvent::ID, this);
		ML_EventSystem.addListener(ScrollEvent::ID, this);
		ML_EventSystem.addListener(WindowCloseEvent::ID, this);
		ML_EventSystem.addListener(WindowErrorEvent::ID, this);
		ML_EventSystem.addListener(WindowFocusEvent::ID, this);
		ML_EventSystem.addListener(WindowKillEvent::ID, this);
		ML_EventSystem.addListener(WindowSizeEvent::ID, this);
		ML_EventSystem.addListener(WindowPosEvent::ID, this);
		ML_EventSystem.addListener(WindowFullscreenEvent::ID, this);
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
		static constexpr Array<int32_t, 4> profiles 
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
			this->getWidth(),
			this->getHeight(),
			this->getTitle().c_str(),
			ML_MONITOR(m_monitor = nullptr),
			ML_WINDOW(m_share = nullptr)
		)))
		{
			this->makeContextCurrent();

			if (this->setup())
			{
				this->setCursorMode(Cursor::Mode::Normal);

				if (this->getStyle().maximized)
				{
					this->maximize(); // Maximized
				}
				else
				{
					this->setCentered(); // Centered
				}
				
				return true;
			}
		}
		return false;
	}

	bool Window::setup()
	{
		setCharCallback([](void *, uint32_t c)
		{
			ML_EventSystem.fireEvent<CharEvent>(c);
		});

		setCursorEnterCallback([](void *, int32_t entered)
		{
			ML_EventSystem.fireEvent<CursorEnterEvent>(entered);
		});

		setCursorPosCallback([](void *, float64_t x, float64_t y)
		{
			ML_EventSystem.fireEvent<CursorPosEvent>(x, y);
		});

		setErrorCallback([](int32_t code, C_String desc)
		{
#if ML_DEBUG == 1
			cerr << "GLFW Error " << code << ": \'" << desc << "\'" << endl;
#endif
			ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
		});

		setFrameSizeCallback([](void *, int32_t w, int32_t h)
		{
			ML_EventSystem.fireEvent<FrameSizeEvent>(w, h);
		});

		setKeyCallback([](void *, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent<KeyEvent>(button, scan, action, bitmask_8 { {
				(mods & ML_MOD_SHIFT),
				(mods & ML_MOD_CTRL),
				(mods & ML_MOD_ALT),
				(mods & ML_MOD_SUPER),
				(mods & ML_MOD_CAPSLOCK),
				(mods & ML_MOD_NUMLOCK)
			} });
		});

		setMouseButtonCallback([](void *, int32_t button, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent<MouseButtonEvent>(button, action, mods);
		});
		
		setScrollCallback([](void *, float64_t x, float64_t y)
		{
			ML_EventSystem.fireEvent<ScrollEvent>(x, y);
		});

		setWindowCloseCallback([](void *)
		{
			ML_EventSystem.fireEvent<WindowCloseEvent>();
		});

		setWindowFocusCallback([](void *, int32_t focused)
		{
			ML_EventSystem.fireEvent<WindowFocusEvent>(focused);
		});
		
		setWindowPosCallback([](void *, int32_t x, int32_t y)
		{
			ML_EventSystem.fireEvent<WindowPosEvent>(x, y);
		});

		setWindowSizeCallback([](void *, int32_t width, int32_t height)
		{
			ML_EventSystem.fireEvent<WindowSizeEvent>(width, height);
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
				m_videoMode.size = { (uint32_t)ev->width, (uint32_t)ev->height };
			}
			break;
		case WindowFullscreenEvent::ID:
			if (auto ev = value.as<WindowFullscreenEvent>())
			{
				switch (ev->value)
				{
				case  0:
				case  1: this->setFullscreen(ev->value); break;
				case -1: this->setFullscreen(!this->is_fullscreen()); break;
				}
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
		if (m_window)
		{
			glfwSetWindowShouldClose(ML_WINDOW(m_window), true);
		}
		return (*this);
	}

	Window & Window::destroy()
	{
		if (m_window)
		{
			glfwDestroyWindow(ML_WINDOW(m_window));
		}
		return (*this);
	}

	Window & Window::iconify()
	{
		if (m_window) 
		{
			glfwIconifyWindow(ML_WINDOW(m_window));
		}
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		if (m_window)
		{
			glfwMakeContextCurrent(ML_WINDOW(m_window));
		}
		return (*this);
	}
	
	Window & Window::maximize()
	{
		if (m_window)
		{
			glfwMaximizeWindow(ML_WINDOW(m_window));
		}
		return (*this);
	}

	Window & Window::pollEvents()
	{
		if (m_window)
		{
			glfwPollEvents();
		}
		return (*this);
	}

	Window & Window::restore()
	{
		if (m_window)
		{
			glfwRestoreWindow(ML_WINDOW(m_window));
		}
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(ML_WINDOW(m_window));
		}
		return (*this);
	}

	Window & Window::swapInterval(const int32_t value)
	{
		if (m_window)
		{
			glfwSwapInterval(value);
		}
		return (*this);
	}

	Window & Window::terminate()
	{
		if (m_window)
		{
			glfwTerminate();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::setCentered()
	{
		return setPosition((vec2i)(Window::getDesktopMode().size - this->getSize()) / 2);
	}

	Window & Window::setClipboardString(const String & value)
	{
		if (m_window)
		{
			glfwSetClipboardString(ML_WINDOW(m_window), value.c_str());
		}
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		if (m_window && value)
		{
			glfwSetCursor(ML_WINDOW(m_window), ML_CURSOR(value));
		}
		return (*this);
	}
	
	Window & Window::setCursorMode(const Cursor::Mode value)
	{
		if (m_window)
		{
			glfwSetInputMode(ML_WINDOW(m_window), GLFW_CURSOR, static_cast<int32_t>(value));
		}
		return (*this);
	}

	Window & Window::setCursorPos(const vec2i & value)
	{
		if (m_window)
		{
			glfwSetCursorPos(ML_WINDOW(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::setFullscreen(bool value)
	{
		return setMonitor(value ? glfwGetPrimaryMonitor() : nullptr);
	}

	Window & Window::setIcon(uint32_t w, uint32_t h, const uint8_t * pixels)
	{
		if (m_window)
		{
			glfwSetWindowIcon(ML_WINDOW(m_window), 1, &map_image(w, h, pixels));
		}
		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		if (m_window)
		{
			glfwSetWindowPos(ML_WINDOW(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::setMonitor(void * value)
	{
		if (m_window && (m_monitor != value))
		{
			if (m_monitor = value)
			{
				if (auto v = glfwGetVideoMode(ML_MONITOR(m_monitor)))
				{
					glfwSetWindowMonitor(
						ML_WINDOW(m_window),
						ML_MONITOR(m_monitor),
						0, 
						0,
						v->width,
						v->height,
						GLFW_DONT_CARE
					);
				}
			}
			else
			{
				glfwSetWindowMonitor(
					ML_WINDOW(m_window),
					ML_MONITOR(m_monitor),
					0,
					0,
					getWidth(),
					getHeight(),
					GLFW_DONT_CARE
				);
			}
		}
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		m_videoMode.size = value;
		if (m_window)
		{
			glfwSetWindowSize(ML_WINDOW(m_window), getWidth(), getHeight());
		}
		return (*this);
	}

	Window & Window::setTitle(const String & value)
	{
		m_title = value;
		if (m_window)
		{
			glfwSetWindowTitle(ML_WINDOW(m_window), m_title.c_str());
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::is_focused() const
	{
		return getAttribute(GLFW_FOCUSED);
	}

	bool Window::is_fullscreen() const
	{
		return m_window && m_monitor && (m_monitor == glfwGetPrimaryMonitor());
	}

	bool Window::is_open() const
	{
		return m_window && !glfwWindowShouldClose(ML_WINDOW(m_window));
	}
	
	int32_t Window::getAttribute(int32_t value) const
	{
		return (m_window ? glfwGetWindowAttrib(ML_WINDOW(m_window), value) : NULL);
	}

	const char & Window::getChar() const
	{
		char temp { m_char };
		m_char = '\0';
		return m_char;
	}

	C_String Window::getClipboardString() const
	{
		return (m_window ? glfwGetClipboardString(ML_WINDOW(m_window)) : nullptr);
	}

	const vec2 & Window::getCursorPos() const
	{
		vec2d temp { NULL };
		if (m_window)
		{
			glfwGetCursorPos(ML_WINDOW(m_window), &temp[0], &temp[1]);
		}
		return (m_cursorPos = (vec2)temp);
	}

	vec2i Window::getFrameSize() const
	{
		vec2i temp { NULL };
		if (m_window)
		{
			glfwGetFramebufferSize(ML_WINDOW(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	int32_t	Window::getKey(const int32_t value) const
	{
		return (m_window ? glfwGetKey(ML_WINDOW(m_window), value) : NULL);
	}

	int32_t Window::getInputMode() const
	{
		return (m_window ? glfwGetInputMode(ML_WINDOW(m_window), GLFW_CURSOR) : NULL);
	}

	int32_t	Window::getMouseButton(const int32_t value) const
	{
		return (m_window ? glfwGetMouseButton(ML_WINDOW(m_window), value) : NULL);
	}

	vec2i Window::getPosition() const
	{
		vec2i temp { NULL };
		if (m_window)
		{
			glfwGetWindowPos(ML_WINDOW(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	float64_t Window::getTime() const
	{
		return (m_window ? glfwGetTime() : 0.0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Window::createCustomCursor(uint32_t w, uint32_t h, const uint8_t * pixels) const
	{
		return (m_window ? glfwCreateCursor(&map_image(w, h, pixels), w, h) : nullptr);
	}

	void * Window::createStandardCursor(Cursor::Shape value) const
	{
		return (m_window ? glfwCreateStandardCursor((int32_t)value) : nullptr);
	}

	bool Window::destroyCursor(void * value) const
	{
		return ((m_window && value) ? ML_TRUE_EXPR(glfwDestroyCursor(ML_CURSOR(value))) : false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const VideoMode & Window::getDesktopMode()
	{
		static VideoMode temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_SYSTEM_WINDOWS
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp = VideoMode { dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel };
#else
			// do the thing
#endif
		}
		return temp;
	}

	const List<VideoMode>& Window::getFullscreenModes()
	{
		static List<VideoMode> temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_SYSTEM_WINDOWS
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				VideoMode vm { dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel };

				if (std::find(temp.begin(), temp.end(), vm) == temp.end())
				{
					temp.push_back(vm);
				}
			}
#else
			// do the thing
#endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::CharFun Window::setCharCallback(CharFun value)
	{
		return (m_window
			? glfwSetCharCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcharfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun value)
	{
		return (m_window
			? glfwSetCursorEnterCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcursorenterfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun value)
	{
		return (m_window
			? glfwSetCursorPosCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWcursorposfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::ErrorFun Window::setErrorCallback(ErrorFun value)
	{
		return (m_window
			? glfwSetErrorCallback(
				reinterpret_cast<GLFWerrorfun>(value)) ? value : nullptr
			: nullptr
		);
	}

	Window::FrameSizeFun Window::setFrameSizeCallback(FrameSizeFun value)
	{
		return (m_window
			? glfwSetFramebufferSizeCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun value)
	{
		return (m_window
			? glfwSetKeyCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWkeyfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun value)
	{
		return (m_window
			? glfwSetMouseButtonCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun value)
	{
		return (m_window
			? glfwSetScrollCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWscrollfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun value)
	{
		return (m_window
			? glfwSetWindowCloseCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowclosefun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun value)
	{
		return (m_window
			? glfwSetWindowFocusCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun value)
	{
		return (m_window
			? glfwSetWindowPosCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun value)
	{
		return (m_window
			? glfwSetWindowSizeCallback(
				ML_WINDOW(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}