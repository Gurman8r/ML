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
			static RenderStates states { {
				new AlphaTest	{ true, GL::Greater, 0.01f },
				new BlendFunc	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
				new CullFace	{ true, GL::Back },
				new DepthTest	{ true, GL::Less }
			} };
			states.apply();

			ML_GL.enable(GL::Multisample, m_context.multisample);
			ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable);

			return true;
		}
		return Debug::logError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const Event * value)
	{
		Window::onEvent(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}