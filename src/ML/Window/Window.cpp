#ifdef ML_IMPL_PLATFORM_GLFW

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# include <glfw/glfw3.h>
# ifdef ML_SYSTEM_WINDOWS
#	undef APIENTRY
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#	include <Windows.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GLFW_HAS_WINDOW_TOPMOST		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN				(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface
#define GLFW_HAS_FOCUS_WINDOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwFocusWindow
#define GLFW_HAS_FOCUS_ON_SHOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_FOCUS_ON_SHOW
#define GLFW_HAS_MONITOR_WORK_AREA	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorWorkarea

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static GLFWimage const & map_glfw_image(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		static HashMap<byte_t const *, GLFWimage> cache {};
		auto it { cache.find(pixels) };
		if (it == cache.end())
		{
			it = cache.insert({
				pixels, GLFWimage { (int32_t)w, (int32_t)h, (uint8_t *)pixels }
			}).first;
		}
		return it->second;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window()
		: m_window		{ nullptr }
		, m_monitor		{ nullptr }
		, m_share		{ nullptr }
		, m_title		{}
		, m_context		{}
		, m_style		{}
		, m_videoMode	{}
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HWND window { GetConsoleWindow() })
		{
			if (HMENU menu { GetSystemMenu(window, false) })
			{
				EnableMenuItem(menu, SC_CLOSE, MF_GRAYED);
			}
		}
#endif
		ML_EventSystem.addListener(WindowErrorEvent::ID, this);
		ML_EventSystem.addListener(WindowKillEvent::ID, this);
		ML_EventSystem.addListener(WindowSizeEvent::ID, this);
		ML_EventSystem.addListener(WindowFullscreenEvent::ID, this);
	}
	
	Window::~Window()
	{
		this->dispose();

#ifdef ML_SYSTEM_WINDOWS
		if (HWND window { GetConsoleWindow() })
		{
			if (HMENU menu { GetSystemMenu(window, false) })
			{
				EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			}
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::create(String const & title, VideoMode const & videoMode, WindowStyle const & style, ContextSettings const & context)
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

		// Renderer Profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, 
			((m_context.profile == 0)
				? GLFW_OPENGL_DEBUG_CONTEXT
				: ((m_context.profile == 1)
					? GLFW_OPENGL_CORE_PROFILE
					: ((m_context.profile == 2)
						? GLFW_OPENGL_COMPAT_PROFILE
						: GLFW_OPENGL_ANY_PROFILE
						))));
		
		// Window Style
		glfwWindowHint(GLFW_RESIZABLE,		m_style.resizable);
		glfwWindowHint(GLFW_VISIBLE,		m_style.visible);
		glfwWindowHint(GLFW_DECORATED,		m_style.decorated);
		glfwWindowHint(GLFW_FOCUSED,		m_style.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY,	m_style.autoIconify);
		glfwWindowHint(GLFW_FLOATING,		m_style.floating);
		glfwWindowHint(GLFW_MAXIMIZED,		m_style.maximized);

		// Create Window
		if (m_window = static_cast<GLFWwindow *>(glfwCreateWindow(
			this->getWidth(),
			this->getHeight(),
			this->getTitle().c_str(),
			static_cast<GLFWmonitor *>(m_monitor = nullptr),
			static_cast<GLFWwindow *>(m_share = nullptr)
		)))
		{
			this->makeContextCurrent();

			this->installCallbacks();

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
		return false;
	}

	void Window::installCallbacks()
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
#if (ML_DEBUG)
			std::cerr << "GLFW Error " << code << ": \'" << desc << "\'" << std::endl;
#endif
			ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
		});

		setFrameSizeCallback([](void *, int32_t w, int32_t h)
		{
			ML_EventSystem.fireEvent<FrameSizeEvent>(w, h);
		});

		setKeyCallback([](void *, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent<KeyEvent>(button, scan, action, BitMask_8 { {
				(mods & ML_MOD_SHIFT),
				(mods & ML_MOD_CTRL),
				(mods & ML_MOD_ALT),
				(mods & ML_MOD_SUPER),
				(mods & ML_MOD_CAPSLOCK),
				(mods & ML_MOD_NUMLOCK)
			} });
		});

		setMouseCallback([](void *, int32_t button, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent<MouseEvent>(button, action, mods);
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
	}

	void Window::onEvent(Event const & value)
	{
		switch (*value)
		{
		case WindowErrorEvent::ID: if (auto ev{ value.as<WindowErrorEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Debug::logError("[GLFW] {0}: \'{1}\'", ev.code, ev.desc);

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case WindowKillEvent::ID: if (auto ev{ value.as<WindowKillEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			this->close();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case WindowSizeEvent::ID: if (auto ev{ value.as<WindowSizeEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			m_videoMode.size = { (uint32_t)ev.width, (uint32_t)ev.height };

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case WindowFullscreenEvent::ID: if (auto ev{ value.as<WindowFullscreenEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			switch (ev.value)
			{
				case  0:
				case  1: this->setFullscreen(ev.value); break;
				case -1: this->setFullscreen(!this->isFullscreen()); break;
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	bool Window::dispose()
	{
		this->destroy();

		this->terminate();

		m_window = m_monitor = m_share	= nullptr;

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		if (m_window)
		{
			glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window), true);
		}
		return (*this);
	}

	Window & Window::destroy()
	{
		if (m_window)
		{
			glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::iconify()
	{
		if (m_window) 
		{
			glfwIconifyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		makeContextCurrent(m_window);
		return (*this);
	}
	
	Window & Window::maximize()
	{
		if (m_window)
		{
			glfwMaximizeWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::restore()
	{
		if (m_window)
		{
			glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::setCentered()
	{
		return setPosition((vec2i)(Window::getDesktopMode().size - this->getSize()) / 2);
	}

	Window & Window::setClipboardString(String const & value)
	{
		if (m_window)
		{
			glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value.c_str());
		}
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		if (m_window && value)
		{
			glfwSetCursor(static_cast<GLFWwindow *>(m_window), static_cast<GLFWcursor *>(value));
		}
		return (*this);
	}
	
	Window & Window::setCursorMode(const Cursor::Mode value)
	{
		if (m_window)
		{
			glfwSetInputMode(static_cast<GLFWwindow *>(m_window), GLFW_CURSOR, static_cast<int32_t>(value));
		}
		return (*this);
	}

	Window & Window::setCursorPos(vec2i const & value)
	{
		if (m_window)
		{
			glfwSetCursorPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::setFullscreen(bool value)
	{
		return setMonitor(value ? glfwGetPrimaryMonitor() : nullptr);
	}

	Window & Window::setIcon(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		if (m_window)
		{
			glfwSetWindowIcon(static_cast<GLFWwindow *>(m_window), 1, &map_glfw_image(w, h, pixels));
		}
		return (*this);
	}

	Window & Window::setPosition(vec2i const & value)
	{
		if (m_window)
		{
			glfwSetWindowPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::setMonitor(void * value)
	{
		if (m_window && (m_monitor != value))
		{
			if (m_monitor = value)
			{
				if (auto v = glfwGetVideoMode(static_cast<GLFWmonitor *>(m_monitor)))
				{
					glfwSetWindowMonitor(
						static_cast<GLFWwindow *>(m_window),
						static_cast<GLFWmonitor *>(m_monitor),
						0, 0, v->width, v->height,
						GLFW_DONT_CARE
					);
				}
			}
			else
			{
				glfwSetWindowMonitor(
					static_cast<GLFWwindow *>(m_window),
					static_cast<GLFWmonitor *>(m_monitor),
					0, 0, getWidth(), getHeight(),
					GLFW_DONT_CARE
				);
			}
		}
		return (*this);
	}

	Window & Window::setSize(vec2u const & value)
	{
		m_videoMode.size = value;
		if (m_window)
		{
			glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), getWidth(), getHeight());
		}
		return (*this);
	}

	Window & Window::setTitle(String const & value)
	{
		m_title = value;
		if (m_window)
		{
			glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), m_title.c_str());
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::isFocused() const
	{
		return getAttribute(GLFW_FOCUSED);
	}

	bool Window::isFullscreen() const
	{
		return m_window && m_monitor && (m_monitor == glfwGetPrimaryMonitor());
	}

	bool Window::isOpen() const
	{
		return m_window && !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t Window::getAttribute(int32_t value) const
	{
		return (m_window ? glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	C_String Window::getClipboardString() const
	{
		return (m_window 
			? glfwGetClipboardString(static_cast<GLFWwindow *>(m_window)) 
			: nullptr
		);
	}

	vec2 Window::getCursorPos() const
	{
		vec2d temp { 0 };
		if (m_window)
		{
			glfwGetCursorPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return (vec2)temp;
	}

	vec2i Window::getFrameSize() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetFramebufferSize(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * Window::getHandle() const
	{
		return m_window;
	}

	int32_t	Window::getKey(int32_t value) const
	{
		return (m_window ? glfwGetKey(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	int32_t Window::getInputMode() const
	{
		return (m_window ? glfwGetInputMode(static_cast<GLFWwindow *>(m_window), GLFW_CURSOR) : 0);
	}

	int32_t	Window::getMouseButton(int32_t value) const
	{
		return (m_window ? glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	vec2i Window::getPosition() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetWindowPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * Window::getRawHandle() const
	{
#ifdef ML_SYSTEM_WINDOWS
		return glfwGetWin32Window(static_cast<GLFWwindow *>(m_window));
#else
		return m_window;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Window::createCustomCursor(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		return glfwCreateCursor(&map_glfw_image(w, h, pixels), w, h);
	}

	void * Window::createStandardCursor(Cursor::Shape value)
	{
		return glfwCreateStandardCursor((int32_t)value);
	}

	bool Window::destroyCursor(void * value)
	{
		return (value ? ML_TRUE_EXPR(glfwDestroyCursor(static_cast<GLFWcursor *>(value))) : false);
	}

	int32_t Window::extensionSupported(C_String value)
	{
		return glfwExtensionSupported(value);
	}

	void * Window::getContextCurrent()
	{
		return glfwGetCurrentContext();
	}

	VideoMode const & Window::getDesktopMode()
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

	const List<VideoMode> & Window::getFullscreenModes()
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

	Window::ProcFun Window::getProcAddress(C_String value)
	{
		return reinterpret_cast<Window::ProcFun>(glfwGetProcAddress(value));
	}

	const List<void *> & Window::getMonitors()
	{
		static List<void *> temp {};
		if (temp.empty())
		{
			int32_t count { 0 };
			GLFWmonitor ** m { glfwGetMonitors(&count) };
			for (size_t i = 0, imax = (size_t)count; i < imax; i++)
			{
				temp.push_back(m[i]);
			}
		}
		return temp;
	}

	float64_t Window::getTime()
	{
		return glfwGetTime();
	}

	void Window::makeContextCurrent(void * value)
	{
		return glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void Window::pollEvents()
	{
		return glfwPollEvents();
	}

	void Window::swapInterval(int32_t value)
	{
		return glfwSwapInterval(value);
	}

	void Window::terminate()
	{
		return glfwTerminate();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::CharFun Window::setCharCallback(CharFun value)
	{
		return (m_window
			? glfwSetCharCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcharfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun value)
	{
		return (m_window
			? glfwSetCursorEnterCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorenterfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun value)
	{
		return (m_window
			? glfwSetCursorPosCallback(
				static_cast<GLFWwindow *>(m_window),
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
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun value)
	{
		return (m_window
			? glfwSetKeyCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWkeyfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::MouseFun Window::setMouseCallback(MouseFun value)
	{
		return (m_window
			? glfwSetMouseButtonCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun value)
	{
		return (m_window
			? glfwSetScrollCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWscrollfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun value)
	{
		return (m_window
			? glfwSetWindowCloseCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowclosefun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun value)
	{
		return (m_window
			? glfwSetWindowFocusCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun value)
	{
		return (m_window
			? glfwSetWindowPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr
		);
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun value)
	{
		return (m_window
			? glfwSetWindowSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif