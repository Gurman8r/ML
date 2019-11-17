#if defined(ML_IMPL_RENDERER_OPENGL3)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Graphics/GraphicsEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_OPENGL_ES2)
#	include <GLES2/gl2.h>
#elif defined(ML_IMPL_OPENGL_ES3)
#	if defined(ML_SYSTEM_APPLE && (TARGET_OS_IOS || TARGET_OS_TV))
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif
#elif defined(ML_IMPL_OPENGL_LOADER_GLEW)
#	include <GL/glew.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
#	include <GL/gl3w.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
#	include <GL/glad.h>
#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
#	include ML_IMPL_OPENGL_LOADER_CUSTOM
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Errors
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::getError() -> uint32_t
	{
		return glGetError();
	}

	void OpenGL::checkError(C_String file, uint32_t line, C_String expr)
	{
		if (const auto code { getError() })
		{
			ML_EventSystem.fireEvent<OpenGLErrorEvent>(file, line, expr, code);
		}
	}


	// Initialization
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenGL::init()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		glewExperimental = true;
		return (glewInit() == GLEW_OK);
#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
		return (gl3wInit() != 0);
#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
		return (gladLoadGL() == 0);
#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
		return false;
#endif
	}

	void OpenGL::validateVersion(uint32_t & major, uint32_t & minor)
	{
		major = (uint32_t)getInt(GL::MajorVersion);
		minor = (uint32_t)getInt(GL::MinorVersion);
		if (getError() == GL::InvalidEnum)
		{
			if (C_String version = getString(GL::Version))
			{
				major = version[0] - '0';
				minor = version[2] - '0';
				Debug::logWarning("Using OpenGL Version: {0}.{1}", major, minor);
			}
			else
			{
				major = 1;
				minor = 1;
				Debug::logWarning("Can't get the version number, assuming 1.1");
			}
		}

		if (!shadersAvailable())
		{
			Debug::logError("Shaders are not available on your system.");
		}

		if (!geometryShadersAvailable())
		{
			Debug::logError("Geometry shaders are not available on your system.");
		}

		if (!framebuffersAvailable())
		{
			Debug::logWarning("Framebuffers Unavailable");
		}

		if (!edgeClampAvailable())
		{
			Debug::logWarning("Texture Edge Clamp Unavailable");
		}

		if (!textureSrgbAvailable())
		{
			Debug::logWarning("Texture sRGB Unavailable");
		}
	}

	
	// Flags
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenGL::isEnabled(uint32_t value)
	{
		bool temp{ false };
		glCheck(temp = glIsEnabled(value));
		return temp;
	}

	bool OpenGL::enable(uint32_t value, bool cond)
	{
		if (cond)
		{
			glCheck(glEnable(value));
			return isEnabled(value);
		}
		return true;
	}

	bool OpenGL::disable(uint32_t value, bool cond)
	{
		if (cond)
		{
			glCheck(glDisable(value));
			return !isEnabled(value);
		}
		return true;
	}


	// Getters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::getString(uint32_t name) -> C_String
	{
		C_String temp{ nullptr };
		glCheck(temp = reinterpret_cast<C_String>(glGetString(name)));
		return temp;
	}

	auto OpenGL::getString(uint32_t name, uint32_t index) -> C_String
	{
		C_String temp{ nullptr };
		glCheck(temp = reinterpret_cast<C_String>(glGetStringi(name, index)));
		return temp;
	}

	bool OpenGL::getBool(uint32_t name)
	{
		uint8_t temp{ 0 };
		glCheck(glGetBooleanv(name, &temp));
		return (bool)temp;
	}

	auto OpenGL::getDouble(uint32_t name) -> float64_t
	{
		float64_t temp{ 0 };
		glCheck(glGetDoublev(name, &temp));
		return temp;
	}

	auto OpenGL::getFloat(uint32_t name) -> float_t
	{
		float_t temp{ 0 };
		glCheck(glGetFloatv(name, &temp));
		return temp;
	}

	auto OpenGL::getInt(uint32_t name) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(getIntv(name, &temp));
		return temp;
	}

	auto OpenGL::getIntv(uint32_t name, int32_t * params) -> int32_t *
	{
		glCheck(glGetIntegerv(name, params));
		return params;
	}


	// Functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenGL::activeTexture(uint32_t value)
	{
		glCheck(glActiveTexture(value));
	}

	void OpenGL::alphaFunc(uint32_t func, float_t value)
	{
		glCheck(glAlphaFunc(func, value));
	}

	void OpenGL::blendFunc(uint32_t sFactor, uint32_t dFactor)
	{
		glCheck(glBlendFunc(sFactor, dFactor));
	}

	void OpenGL::blendEquation(uint32_t equation)
	{
		glCheck(glBlendEquation(equation));
	}

	void OpenGL::cullFace(uint32_t mode)
	{
		glCheck(glCullFace(mode));
	}

	void OpenGL::depthFunc(uint32_t cmp)
	{
		glCheck(glDepthFunc(cmp));
	}

	void OpenGL::depthMask(bool value)
	{
		glCheck(glDepthMask(value));
	}

	void OpenGL::viewport(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		glCheck(glViewport(x, y, w, h));
	}

	void OpenGL::blendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha)
	{
		glCheck(glBlendEquationSeparate(modeRGB, modeAlpha));
	}

	void OpenGL::blendFuncSeparate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha)
	{
		glCheck(glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
	}

	void OpenGL::polygonMode(uint32_t face, uint32_t mode)
	{
		glCheck(glPolygonMode(face, mode));
	}


	// Drawing
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenGL::clear(uint32_t mask)
	{
		glCheck(glClear(mask));
	}

	void OpenGL::clearColor(float_t r, float_t g, float_t b, float_t a)
	{
		glCheck(glClearColor(r, g, b, a));
	}

	void OpenGL::drawElements(uint32_t mode, int32_t count, uint32_t type, voidptr_t indices)
	{
		glCheck(glDrawElements(mode, count, type, indices));
	}

	void OpenGL::drawArrays(uint32_t mode, int32_t first, int32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void OpenGL::flush()
	{
		glCheck(glFlush());
	}



	// Buffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::genBuffer() -> uint32_t
	{
		return genBuffers(1);
	}

	auto OpenGL::genBuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenBuffers(count, &temp));
		return temp;
	}

	auto OpenGL::genVertexArray() -> uint32_t
	{
		return genVertexArrays(1);
	}

	auto OpenGL::genVertexArrays(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenVertexArrays(count, &temp));
		return temp;
	}

	void OpenGL::bindBuffer(uint32_t type, uint32_t buffer)
	{
		glCheck(glBindBuffer(type, buffer));
	}

	void OpenGL::bindVertexArray(uint32_t value)
	{
		glCheck(glBindVertexArray(value));
	}

	void OpenGL::bufferData(uint32_t type, uint32_t size, voidptr_t data, uint32_t usage)
	{
		glCheck(glBufferData(type, size, data, usage));
	}

	void OpenGL::bufferSubData(uint32_t target, int32_t offset, int32_t size, voidptr_t data)
	{
		glCheck(glBufferSubData(target, offset, size, data));
	}

	bool OpenGL::deleteBuffer(const uint32_t * value)
	{
		return value ? ML_TRUE_EXPR(deleteBuffers(1, value)) : false;
	}

	void OpenGL::deleteBuffers(uint32_t count, const uint32_t * buffers)
	{
		glCheck(glDeleteBuffers(count, buffers));
	}

	bool OpenGL::deleteVertexArray(const uint32_t * value)
	{
		return value ? ML_TRUE_EXPR(deleteVertexArrays(1, value)) : false;
	}

	void OpenGL::deleteVertexArrays(uint32_t count, const uint32_t * arrays)
	{
		glCheck(glDeleteVertexArrays(count, arrays));
	}

	void OpenGL::vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, voidptr_t pointer)
	{
		glCheck(glVertexAttribPointer(
			index,
			size,
			type,
			normalized,
			stride,
			pointer
		));
	}

	void OpenGL::vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width)
	{
		return vertexAttribPointer(
			index,
			size,
			type,
			normalized,
			(stride * width),
# pragma warning(push)
# pragma warning(disable: 4312)
# pragma warning(disable: 26451)
			// causes a warning in x64
			reinterpret_cast<voidptr_t>(offset * width)
# pragma warning(pop)
		);
	}

	void OpenGL::enableVertexAttribArray(uint32_t index)
	{
		glCheck(glEnableVertexAttribArray(index));
	}



	// Textures
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenGL::edgeClampAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GL_EXT_texture_edge_clamp || GLEW_EXT_texture_edge_clamp;
		}
		return temp;
	}

	bool OpenGL::textureSrgbAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GL_EXT_texture_sRGB;
		}
		return temp;
	}

	bool OpenGL::nonPowerOfTwoAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GLEW_ARB_texture_non_power_of_two;
		}
		return temp;
	}

	auto OpenGL::getMaxTextureUnits() -> int32_t
	{
		static int32_t temp{ 0 };
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			temp = getInt(GL::MaxCombTexImgUnits);
		}
		return temp;
	}

	auto OpenGL::getMaxTextureSize() -> uint32_t
	{
		static uint32_t temp{ 0 };
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			temp = (uint32_t)(getInt(GL::MaxTextureSize));
		}
		return temp;
	}

	auto OpenGL::getValidTextureSize(uint32_t value) -> uint32_t
	{
		if (!nonPowerOfTwoAvailable())
		{
			uint32_t powerOfTwo = 1;
			while (powerOfTwo < value)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::genTexture() -> uint32_t
	{
		return genTextures(1);
	}

	auto OpenGL::genTextures(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenTextures(count, &temp));
		return temp;
	}

	bool OpenGL::deleteTexture(const uint32_t * value)
	{
		return value ? ML_TRUE_EXPR(deleteTextures(1, value)) : false;
	}

	void OpenGL::deleteTextures(uint32_t count, const uint32_t * id)
	{
		glCheck(glDeleteTextures(count, id));
	}

	void OpenGL::bindTexture(uint32_t target, uint32_t id)
	{
		glCheck(glBindTexture(target, id));
	}

	void OpenGL::texImage2D(uint32_t target, int32_t level, uint32_t internalFormat, int32_t width, int32_t height, int32_t border, uint32_t format, uint32_t type, voidptr_t pixels)
	{
		glCheck(glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels));
	}

	void OpenGL::texSubImage2D(uint32_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, uint32_t format, uint32_t type, voidptr_t pixels)
	{
		glCheck(glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels));
	}

	void OpenGL::texParameter(uint32_t target, uint32_t name, uint32_t param)
	{
		glCheck(glTexParameteri(target, name, param));
	}

	void OpenGL::getTexImage(uint32_t target, int32_t level, uint32_t format, uint32_t type, voidptr_t pixels)
	{
		glCheck(glGetTexImage(target, level, format, type, pixels));
	}

	void OpenGL::generateMipmap(uint32_t target)
	{
		glCheck(glGenerateMipmap(target));
	}

	void OpenGL::pixelStore(uint32_t name, int32_t param)
	{
		glCheck(glPixelStorei(name, param));
	}

	void OpenGL::scissor(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		glCheck(glScissor(x, y, width, height));
	}

	void OpenGL::bindSampler(uint32_t unit, int32_t sampler)
	{
		glCheck(glBindSampler(unit, sampler));
	}


	// Framebuffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenGL::framebuffersAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked)
		{
			checked = true;
			temp = GL_EXT_framebuffer_object && GL_EXT_framebuffer_blit;
		}
		return temp;
	}

	auto OpenGL::genFramebuffer() -> uint32_t
	{
		return genFramebuffers(1);
	}
	
	auto OpenGL::genFramebuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenFramebuffers(count, &temp));
		return temp;
	}

	auto OpenGL::checkFramebufferStatus(uint32_t target) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(temp = glCheckFramebufferStatus(target));
		return temp;
	}

	void OpenGL::bindFramebuffer(uint32_t target, uint32_t framebuffer)
	{
		glCheck(glBindFramebuffer(target, framebuffer));
	}

	bool OpenGL::deleteFramebuffer(const uint32_t * value)
	{
		return value ? ML_TRUE_EXPR(deleteFramebuffers(1, value)) : false;
	}

	void OpenGL::deleteFramebuffers(uint32_t count, const uint32_t * framebuffers)
	{
		glCheck(glDeleteFramebuffers(count, framebuffers));
	}

	void OpenGL::blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, GL::Mask mask, uint32_t filter)
	{
		glCheck(glBlitFramebuffer(
			srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1,
			mask,
			filter
		));
	}

	void OpenGL::framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget, uint32_t texture, int32_t level)
	{
		glCheck(glFramebufferTexture2D(target, attachment, textarget, texture, level));
	}


	// Renderbuffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::genRenderbuffer() -> uint32_t
	{
		return genRenderbuffers(1);
	}

	auto OpenGL::genRenderbuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenRenderbuffers(count, &temp));
		return temp;
	}

	bool OpenGL::deleteRenderbuffer(const uint32_t * value)
	{
		return value ? ML_TRUE_EXPR(deleteRenderbuffers(1, value)) : false;
	}

	void OpenGL::deleteRenderbuffers(uint32_t count, const uint32_t * renderbuffers)
	{
		glCheck(glDeleteRenderbuffers(count, renderbuffers));
	}

	void OpenGL::bindRenderbuffer(uint32_t target, uint32_t renderbuffer)
	{
		glCheck(glBindRenderbuffer(target, renderbuffer));
	}

	void OpenGL::renderbufferStorage(uint32_t target, uint32_t value, int32_t width, int32_t height)
	{
		glCheck(glRenderbufferStorage(target, value, width, height));
	}

	void OpenGL::framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget, uint32_t renderbuffer)
	{
		glCheck(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));

	}


	// Shaders
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenGL::shadersAvailable()
	{
		static bool checked{ false };
		static bool available{ false };
		if (!checked && (checked = true))
		{
			available =
				GL_ARB_multitexture &&
				GL_ARB_shading_language_100 &&
				GL_ARB_shader_objects &&
				GL_ARB_vertex_shader &&
				GL_ARB_fragment_shader;
		}
		return available;
	}

	bool OpenGL::geometryShadersAvailable()
	{
		static bool available = false;
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			checked = true;
			available = shadersAvailable() && GL_ARB_geometry_shader4;
		}
		return available;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto OpenGL::getProgramInfoLog(uint32_t obj) -> C_String
	{
		static char temp[512];
		glCheck(glGetInfoLogARB(obj, sizeof(temp), 0, temp));
		return temp;
	}

	auto OpenGL::getProgramHandle(uint32_t name) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(temp = glGetHandleARB(name));
		return temp;
	}

	auto OpenGL::createProgram() -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(temp = glCreateProgramObjectARB());
		return temp;
	}

	auto OpenGL::createShader(uint32_t type) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(temp = glCreateShaderObjectARB(type));
		return temp;
	}

	auto OpenGL::getProgramParameter(int32_t obj, uint32_t param) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(glGetObjectParameterivARB(obj, param, &temp));
		return temp;
	}

	auto OpenGL::getProgramiv(uint32_t program, uint32_t name) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(glGetProgramiv(program, name, &temp));
		return temp;
	}
	
	auto OpenGL::getAttribLocation(uint32_t program, C_String name) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(temp = glGetAttribLocationARB(program, name));
		return temp;
	}

	auto OpenGL::getUniformLocation(uint32_t program, C_String name) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(temp = glGetUniformLocationARB(program, name));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenGL::useProgram(uint32_t obj)
	{
		glCheck(glUseProgramObjectARB(obj));
	}

	void OpenGL::deleteShader(uint32_t obj)
	{
		glCheck(glDeleteObjectARB(obj));
	}

	void OpenGL::detachShader(uint32_t containerObj, uint32_t obj)
	{
		glCheck(glDetachObjectARB(containerObj, obj));
	}

	void OpenGL::attachShader(uint32_t containerObj, uint32_t obj)
	{
		glCheck(glAttachObjectARB(containerObj, obj));
	}

	void OpenGL::shaderSource(uint32_t obj, int32_t count, C_String const * src, const_ptr_t<int32_t> length)
	{
		glCheck(glShaderSource(obj, count, &src[0], length));
	}

	auto OpenGL::compileShader(uint32_t obj) -> int32_t
	{
		glCheck(glCompileShaderARB(obj));

		return getProgramParameter(obj, GL::ObjectCompileStatus);
	}

	auto OpenGL::compileShader(uint32_t & obj, uint32_t type, int32_t count, const C_String * source) -> int32_t
	{
		C_String log { nullptr };
		return compileShader(obj, type, count, source, log);
	}

	auto OpenGL::compileShader(uint32_t & obj, uint32_t type, int32_t count, const C_String * source, C_String & log) -> int32_t
	{
		log = nullptr;
		if ((count < 1) || !source || !(*source))
		{
			return ML_WARNING; // -1 (true)
		}
		else if (obj = createShader(type))
		{
			shaderSource(obj, count, source, nullptr);
			if (!compileShader(obj))
			{
				log = getProgramInfoLog(obj);
				deleteShader(obj);
				return ML_FAILURE; // 0 (false)
			}
			return ML_SUCCESS; // +1 (true)
		}
		return ML_FAILURE; // 0 (false)
	}

	auto OpenGL::linkProgram(uint32_t obj) -> int32_t
	{
		glCheck(glLinkProgramARB(obj));

		return getProgramParameter(obj, GL::ObjectLinkStatus);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenGL::uniform1f(int32_t location, float_t value)
	{
		glCheck(glUniform1fARB(location, value));
	}

	void OpenGL::uniform1i(int32_t location, int32_t value)
	{
		glCheck(glUniform1iARB(location, value));
	}

	void OpenGL::uniform2f(int32_t location, float_t x, float_t y)
	{
		glCheck(glUniform2fARB(location, x, y));
	}

	void OpenGL::uniform3f(int32_t location, float_t x, float_t y, float_t z)
	{
		glCheck(glUniform3fARB(location, x, y, z));
	}

	void OpenGL::uniform4f(int32_t location, float_t x, float_t y, float_t z, float_t w)
	{
		glCheck(glUniform4fARB(location, x, y, z, w));
	}

	void OpenGL::uniform2i(int32_t location, int32_t x, int32_t y)
	{
		glCheck(glUniform2iARB(location, x, y));
	}

	void OpenGL::uniform3i(int32_t location, int32_t x, int32_t y, int32_t z)
	{
		glCheck(glUniform3iARB(location, x, y, z));
	}

	void OpenGL::uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w)
	{
		glCheck(glUniform4iARB(location, x, y, z, w));
	}

	void OpenGL::uniform1fv(int32_t location, uint32_t count, const_ptr_t<float_t> value)
	{
		glCheck(glUniform1fvARB(location, count, value));
	}

	void OpenGL::uniform2fv(int32_t location, uint32_t count, const_ptr_t<float_t> value)
	{
		glCheck(glUniform2fvARB(location, count, value));
	}

	void OpenGL::uniform3fv(int32_t location, uint32_t count, const_ptr_t<float_t> value)
	{
		glCheck(glUniform3fvARB(location, count, value));
	}

	void OpenGL::uniform4fv(int32_t location, uint32_t count, const_ptr_t<float_t> value)
	{
		glCheck(glUniform4fvARB(location, count, value));
	}

	void OpenGL::uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, const_ptr_t<float_t> value)
	{
		glCheck(glUniformMatrix2fvARB(location, count, transpose, value));
	}

	void OpenGL::uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, const_ptr_t<float_t> value)
	{
		glCheck(glUniformMatrix3fvARB(location, count, transpose, value));
	}

	void OpenGL::uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, const_ptr_t<float_t> value)
	{
		glCheck(glUniformMatrix4fvARB(location, count, transpose, value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif