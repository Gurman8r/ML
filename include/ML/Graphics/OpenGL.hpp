#ifndef _ML_OPENGL_HPP_
#define _ML_OPENGL_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/I_Singleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

// In debug mode, perform a test on every OpenGL call
// The do loop is needed so that glCheck can be used as a single statement in branches
// Source: https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/GLCheck.hpp
# if ML_DEBUG
#	define glCheck(EX) do { EX; ML_GL.checkError(cout, __FILE__, __LINE__, #EX); } while (0)
# else
#	define glCheck(EX) (EX)
# endif

/* * * * * * * * * * * * * * * * * * * * */

#define ML_GL _ML OpenGL::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_GRAPHICS_API OpenGL final : public I_Singleton<OpenGL>
	{
		// Errors
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	getError() -> GL::Err;
		static auto	checkError(C_String file, uint32_t line, C_String expr) -> Ostream &;
		static auto	checkError(Ostream & out, C_String file, uint32_t line, C_String expr) -> Ostream &;

		// Initialization
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	init(bool reinit = false);
		static void	validateVersion(uint32_t & major, uint32_t & minor);

		// Flags
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool isEnabled(uint32_t value);
		static bool enable(uint32_t value, bool cond = true);
		static bool disable(uint32_t value, bool cond = true);
		
		// Getters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto getString(uint32_t name) -> C_String;
		static auto getString(uint32_t name, uint32_t index) -> C_String;
		static bool	getBool(uint32_t name);
		static auto	getDouble(uint32_t name) -> float64_t;
		static auto	getFloat(uint32_t name)	-> float_t;
		static auto	getInt(uint32_t name) -> int32_t;
		static auto	getIntv(uint32_t name, int32_t * params) -> int32_t *;

		// Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	activeTexture(uint32_t value);
		static void	alphaFunc(GL::Predicate predicate, float_t value);
		static void	blendFunc(uint32_t sFactor, uint32_t dFactor);
		static void	blendEquation(GL::Equation equation);
		static void	cullFace(GL::Face value);
		static void	depthFunc(GL::Predicate value);
		static void	depthMask(bool value);
		static void	viewport(int32_t x, int32_t y, int32_t w, int32_t h);
		static void	blendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha);
		static void	blendFuncSeparate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha);
		static void	polygonMode(uint32_t face, uint32_t mode);

		// Drawing
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void clear(uint32_t mask);
		static void clearColor(float_t r, float_t g, float_t b, float_t a);
		static void drawElements(GL::Mode mode, int32_t count, GL::Type type, const void * indices);
		static void drawArrays(GL::Mode mode, int32_t first, int32_t count);
		static void flush();

		// Buffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto genBuffers(uint32_t count) -> uint32_t;
		static auto genVertexArrays(uint32_t count) -> uint32_t;
		static void	bindBuffer(uint32_t type, uint32_t buffer);
		static void	bindVertexArray(uint32_t value);
		static void	bufferData(uint32_t type, uint32_t size, const void * data, GL::Usage usage);
		static void	bufferSubData(uint32_t target, int32_t offset, int32_t size, const void * data);
		static void	deleteBuffers(uint32_t count, const uint32_t * buffers);
		static void	deleteVertexArrays(uint32_t count, const uint32_t * arrays);
		static void	vertexAttribPointer(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, const void * pointer);
		static void	vertexAttribPointer(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width);
		static void	enableVertexAttribArray(uint32_t index);

		// Textures
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	getMaxTextureUnits() -> int32_t;
		static auto	getMaxTextureSize() -> uint32_t;
		static auto getValidTextureSize(uint32_t value) -> uint32_t;
		static bool	edgeClampAvailable();
		static bool	textureSrgbAvailable();
		static bool	nonPowerOfTwoAvailable();

		static auto genTextures(uint32_t count) -> uint32_t;
		static void	deleteTextures(uint32_t count, const uint32_t * id);
		static void	bindTexture(uint32_t target, uint32_t id);
		static void	texImage2D(uint32_t target, int32_t level, GL::Format internalFormat, int32_t width, int32_t height, int32_t border, GL::Format format, GL::Type type, const void * pixels);
		static void	texSubImage2D(uint32_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, GL::Format format, GL::Type type, const void * pixels);
		static void	texParameter(uint32_t target, uint32_t name, uint32_t param);
		static void	getTexImage(uint32_t target, int32_t level, GL::Format format, GL::Type type, void * pixels);
		static void	generateMipmap(uint32_t target);
		static void	pixelStore(uint32_t name, int32_t param);
		static void	scissor(int32_t x, int32_t y, int32_t width, int32_t height);
		static void	bindSampler(uint32_t unit, int32_t sampler);

		// Framebuffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	framebuffersAvailable();
		static auto genFramebuffers(uint32_t count) -> uint32_t;
		static auto	checkFramebufferStatus(uint32_t target) -> uint32_t;
		static void	bindFramebuffer(uint32_t target, uint32_t framebuffer);
		static void	deleteFramebuffers(uint32_t count, const uint32_t * framebuffers);
		static void	blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, GL::Mask mask, uint32_t filter);
		static void	framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget, uint32_t texture, int32_t level);

		// Renderbuffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto genRenderbuffers(uint32_t count) -> uint32_t;
		static void	deleteRenderbuffers(uint32_t count, const uint32_t * renderbuffers);
		static void	bindRenderbuffer(uint32_t target, uint32_t renderbuffer);
		static void	renderbufferStorage(uint32_t target, uint32_t value, int32_t width, int32_t height);
		static void	framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget, uint32_t renderbuffer);

		// Shaders
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	shadersAvailable();
		static bool	geometryShadersAvailable();

		static auto getProgramInfoLog(uint32_t obj) -> C_String;
		static auto getProgramHandle(uint32_t name) -> uint32_t;
		static auto createProgramObject() -> uint32_t;
		static auto createShaderObject(GL::ShaderType type) -> uint32_t;
		static auto getProgramParameter(int32_t obj, GL::Status param) -> int32_t;
		static auto getProgramiv(uint32_t program, uint32_t name) -> int32_t;
		static auto getAttribLocation(uint32_t program, C_String name) -> int32_t;
		static auto getUniformLocation(uint32_t program, C_String name) -> int32_t;

		static void	useShader(uint32_t obj);
		static void	deleteShader(uint32_t obj);
		static void	detachShader(uint32_t containerObj, uint32_t obj);
		static void	attachShader(uint32_t containerObj, uint32_t obj);
		static void	shaderSource(uint32_t obj, int32_t count, C_String const * src, const int32_t * length);
		static auto	compileShader(uint32_t obj) -> int32_t;
		static auto	compileShader(uint32_t & obj, GL::ShaderType type, int32_t count, const C_String * source) -> int32_t;
		static auto	linkShader(uint32_t obj) -> int32_t;

		static void	uniform1f(int32_t location, float_t value);
		static void	uniform1i(int32_t location, int32_t value);
		static void	uniform2f(int32_t location, float_t x, float_t y);
		static void	uniform3f(int32_t location, float_t x, float_t y, float_t z);
		static void	uniform4f(int32_t location, float_t x, float_t y, float_t z, float_t w);
		static void	uniform2i(int32_t location, int32_t x, int32_t y);
		static void	uniform3i(int32_t location, int32_t x, int32_t y, int32_t z);
		static void	uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w);
		static void	uniform1fv(int32_t location, uint32_t count, const float_t * value);
		static void	uniform2fv(int32_t location, uint32_t count, const float_t * value);
		static void	uniform3fv(int32_t location, uint32_t count, const float_t * value);
		static void	uniform4fv(int32_t location, uint32_t count, const float_t * value);
		static void	uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, const float_t * value);
		static void	uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, const float_t * value);
		static void	uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, const float_t * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: friend struct I_Singleton<OpenGL>;
	};
}

#endif // !_ML_OPENGL_HPP_