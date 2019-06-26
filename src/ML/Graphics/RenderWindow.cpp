#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Graphics/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow(EventSystem & eventSystem)
		: Window(eventSystem)
	{
	}

	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::setup()
	{
		if (Window::setup() && ML_GL.init())
		{
			// Validate OpenGL Version
			ML_GL.validateVersion(m_context.majorVersion, m_context.minorVersion);

			// Setup States
			RenderStates states
			{
				{ true, GL::Greater, 0.01f },
				{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
				{ true, GL::Back },
				{ true, GL::Less },
				{ true, GL::Texture2D, GL::Texture0 },
				{ m_context.multisample, m_context.srgbCapable }
			};
			states.apply();

			return true;
		}
		return ml::Debug::logError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const Event * value)
	{
		Window::onEvent(value);

		switch (*value)
		{
		case FrameSizeEvent::ID:
			if (auto ev = value->as<FrameSizeEvent>())
			{
				this->setViewport(vec2 { 0.f, 0.f }, { ev->width, ev->height });
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}