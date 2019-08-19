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
			AlphaState	{ true, GL::Greater, 0.01f }();
			BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha }();
			CullState	{ true, GL::Back }(); 
			DepthState	{ true, GL::Less, true }(); 

			ML_GL.enable(GL::Multisample, m_context.multisample);
			ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable);

			this->setViewport(vec2i { 0, 0 }, getFrameSize());

			return true;
		}
		return Debug::logError("Failed Initializing GLEW");
	}

	void RenderWindow::onEvent(const Event & value)
	{
		Window::onEvent(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}