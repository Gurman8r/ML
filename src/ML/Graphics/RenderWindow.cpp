#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/GraphicsEvents.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow() : Window {}
	{
		ML_EventSystem.addListener<OpenGLErrorEvent>(this);
	}

	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::create(String const & title, VideoMode const & videoMode, WindowStyle const & style, ContextSettings const & context)
	{
		if (Window::create(title, videoMode, style, context))
		{
			if (!ML_GL.init())
			{
				return Debug::logError("Failed initializing OpenGL");
			}

			ML_GL.validateVersion(m_context.major, m_context.minor);

			RenderStates{}();

			ML_GL.enable(GL::Multisample, m_context.multisample);

			ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderWindow::onEvent(Event const & value)
	{
		Window::onEvent(value);

		switch (*value)
		{
			case OpenGLErrorEvent::ID: if (auto ev = value.as<OpenGLErrorEvent>())
			{
				// Error location
				String filename { ev.file };
				filename = filename.substr(filename.find_last_of("\\/") + 1);

				// Decode the error
				std::cout
					<< FG::Red		<< "An OpenGL call failed in \'" << ev.file << "\' (" << ev.line << ")"
					<< FG::Yellow	<< "\nCode: "
					<< FG::White	<< "\n\t" << ev.code
					<< FG::Yellow	<< "\nExpression: "
					<< FG::White	<< "\n\t" << ev.expr
					<< FG::Yellow	<< "\nDescription:"
					<< FG::White	<< "\n\t" << GL::name_of((GL::Err)ev.code)
					<< FG::White	<< "\n\t" << GL::desc_of((GL::Err)ev.code)
					<< FMT()		<< std::endl;
			} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}