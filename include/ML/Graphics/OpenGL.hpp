#ifndef _ML_OPENGL_HPP_
#define _ML_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/Singleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_GL ::ml::OpenGL::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#if (ML_DEBUG)
# 	define glCheck(expr) do { expr; ML_GL.checkError(__FILE__, __LINE__, #expr); } while (0)
#else
# 	define glCheck(expr) (expr)
#endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_GRAPHICS_API OpenGL final : public Singleton<OpenGL>
	{
		// Errors
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	getError() -> uint32_t;
		static void	checkError(C_String file, uint32_t line, C_String expr);

		// Initialization
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	init();
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
		static auto	getFloat(uint32_t name)	-> float32_t;
		static auto	getInt(uint32_t name) -> int32_t;
		static auto	getIntv(uint32_t name, int32_t * params) -> int32_t *;

		// Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	activeTexture(uint32_t value);
		static void	alphaFunc(uint32_t func, float32_t value);
		static void	blendFunc(uint32_t sFactor, uint32_t dFactor);
		static void	blendEquation(uint32_t equation);
		static void	cullFace(uint32_t value);
		static void	depthFunc(uint32_t value);
		static void	depthMask(bool value);
		static void	viewport(int32_t x, int32_t y, int32_t w, int32_t h);
		static void	blendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha);
		static void	blendFuncSeparate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha);
		static void	polygonMode(uint32_t face, uint32_t mode);

		// Drawing
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void clear(uint32_t mask);
		static void clearColor(float32_t r, float32_t g, float32_t b, float32_t a);
		static void drawElements(uint32_t mode, int32_t count, uint32_t type, void * indices);
		static void drawArrays(uint32_t mode, int32_t first, int32_t count);
		static void flush();

		// Buffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto genBuffer() -> uint32_t;
		static auto genBuffers(uint32_t count) -> uint32_t;
		static auto genVertexArray() -> uint32_t;
		static auto genVertexArrays(uint32_t count) -> uint32_t;
		static void	bindBuffer(uint32_t type, uint32_t buffer);
		static void	bindVertexArray(uint32_t value);
		static void	bufferData(uint32_t type, uint32_t size, void * data, uint32_t usage);
		static void	bufferSubData(uint32_t target, int32_t offset, int32_t size, void * data);
		static bool	deleteBuffer(uint32_t const * value);
		static void	deleteBuffers(uint32_t count, uint32_t const * buffers);
		static bool deleteVertexArray(uint32_t const * value);
		static void	deleteVertexArrays(uint32_t count, uint32_t const * arrays);
		static void	vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, void * pointer);
		static void	vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width);
		static void	enableVertexAttribArray(uint32_t index);

		// Textures
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	getMaxTextureUnits() -> int32_t;
		static auto	getMaxTextureSize() -> uint32_t;
		static auto getValidTextureSize(uint32_t value) -> uint32_t;
		static bool	edgeClampAvailable();
		static bool	textureSrgbAvailable();
		static bool	nonPowerOfTwoAvailable();

		static auto genTexture() -> uint32_t;
		static auto genTextures(uint32_t count) -> uint32_t;
		static bool deleteTexture(uint32_t const * value);
		static void	deleteTextures(uint32_t count, uint32_t const * id);
		static void	bindTexture(uint32_t target, uint32_t id);
		static void	texImage2D(uint32_t target, int32_t level, uint32_t internalFormat, int32_t width, int32_t height, int32_t border, uint32_t format, uint32_t type, void * pixels);
		static void	texSubImage2D(uint32_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, uint32_t format, uint32_t type, void * pixels);
		static void	texParameter(uint32_t target, uint32_t name, uint32_t param);
		static void	getTexImage(uint32_t target, int32_t level, uint32_t format, uint32_t type, void * pixels);
		static void	generateMipmap(uint32_t target);
		static void	pixelStore(uint32_t name, int32_t param);
		static void	scissor(int32_t x, int32_t y, int32_t width, int32_t height);
		static void	bindSampler(uint32_t unit, int32_t sampler);

		// Framebuffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	framebuffersAvailable();
		static auto genFramebuffer() -> uint32_t;
		static auto genFramebuffers(uint32_t count) -> uint32_t;
		static auto	checkFramebufferStatus(uint32_t target) -> uint32_t;
		static void	bindFramebuffer(uint32_t target, uint32_t framebuffer);
		static bool deleteFramebuffer(uint32_t const * value);
		static void	deleteFramebuffers(uint32_t count, uint32_t const * framebuffers);
		static void	blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, uint8_t mask, uint32_t filter);
		static void	framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget, uint32_t texture, int32_t level);

		// Renderbuffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto genRenderbuffer() -> uint32_t;
		static auto genRenderbuffers(uint32_t count) -> uint32_t;
		static bool deleteRenderbuffer(uint32_t const * value);
		static void	deleteRenderbuffers(uint32_t count, uint32_t const * renderbuffers);
		static void	bindRenderbuffer(uint32_t target, uint32_t renderbuffer);
		static void	renderbufferStorage(uint32_t target, uint32_t value, int32_t width, int32_t height);
		static void	framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget, uint32_t renderbuffer);

		// Shaders
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	shadersAvailable();
		static bool	geometryShadersAvailable();

		static auto getProgramInfoLog(uint32_t obj) -> C_String;
		static auto getProgramHandle(uint32_t name) -> uint32_t;
		static auto createProgram() -> uint32_t;
		static auto createShader(uint32_t type) -> uint32_t;
		static auto getProgramParameter(int32_t obj, uint32_t param) -> int32_t;
		static auto getProgramiv(uint32_t program, uint32_t name) -> int32_t;
		static auto getAttribLocation(uint32_t program, C_String name) -> int32_t;
		static auto getUniformLocation(uint32_t program, C_String name) -> int32_t;

		static void	useProgram(uint32_t obj);
		static void	deleteShader(uint32_t obj);
		static void	detachShader(uint32_t containerObj, uint32_t obj);
		static void	attachShader(uint32_t containerObj, uint32_t obj);
		static void	shaderSource(uint32_t obj, int32_t count, C_String const * src, int32_t const * length);
		static auto	compileShader(uint32_t obj) -> int32_t;
		static auto	compileShader(uint32_t & obj, uint32_t type, int32_t count, C_String const * source) -> int32_t;
		static auto	compileShader(uint32_t & obj, uint32_t type, int32_t count, C_String const * source, C_String & log) -> int32_t;
		static auto	linkProgram(uint32_t obj) -> int32_t;

		static void	uniform1i(int32_t location, int32_t value);
		static void	uniform2i(int32_t location, int32_t x, int32_t y);
		static void	uniform3i(int32_t location, int32_t x, int32_t y, int32_t z);
		static void	uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w);
		static void	uniform1f(int32_t location, float32_t value);
		static void	uniform2f(int32_t location, float32_t x, float32_t y);
		static void	uniform3f(int32_t location, float32_t x, float32_t y, float32_t z);
		static void	uniform4f(int32_t location, float32_t x, float32_t y, float32_t z, float32_t w);
		static void	uniform1fv(int32_t location, uint32_t count, float32_t const * value);
		static void	uniform2fv(int32_t location, uint32_t count, float32_t const * value);
		static void	uniform3fv(int32_t location, uint32_t count, float32_t const * value);
		static void	uniform4fv(int32_t location, uint32_t count, float32_t const * value);
		static void	uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, float32_t const * value);
		static void	uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, float32_t const * value);
		static void	uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, float32_t const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: friend struct Singleton<OpenGL>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_OPENGL_HPP_