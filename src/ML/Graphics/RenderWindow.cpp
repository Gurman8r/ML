#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/GraphicsEvents.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow()
		: Window {}
	{
		ML_EventSystem.addListener(OpenGLErrorEvent::ID, this);
	}

	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::setup()
	{
		if (Window::setup() && ML_GL.init())
		{
			// Validate OpenGL Version
			ML_GL.validateVersion(m_context.major, m_context.minor);

			// Setup States
			RenderStates {}();
			ML_GL.enable(GL::Multisample, m_context.multisample);
			ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable);

			return true;
		}
		return Debug::logError("Failed Initializing GLEW");
	}

	void RenderWindow::onEvent(const Event & value)
	{
		Window::onEvent(value);

		switch (*value)
		{
		case OpenGLErrorEvent::ID:
			if (auto ev = value.as<OpenGLErrorEvent>())
			{
				// Error location
				String filename { ev->file };
				filename = filename.substr(filename.find_last_of("\\/") + 1);

				// Decode the error
				cout<< FG::Red		<< "\nAn OpenGL call failed in \'" << ev->file << "\' (" << ev->line << ")"
					<< FG::Yellow	<< "\nCode: "
					<< FG::White	<< "\n\t" << ev->code
					<< FG::Yellow	<< "\nExpression: "
					<< FG::White	<< "\n\t" << ev->expr
					<< FG::Yellow	<< "\nDescription:"
					<< FG::White	<< "\n\t" << GL::name_of((GL::Err)ev->code)
					<< FG::White	<< "\n\t" << GL::desc_of((GL::Err)ev->code)
					<< FMT()		<< endl;
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}