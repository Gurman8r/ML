#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/Color.hpp>

// Uniforms names used internally
#define ML_UNI_MAIN_COL "u_color"
#define ML_UNI_MAIN_TEX "u_texture0"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Shader final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Source final { String vs, fs, gs; };
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		explicit Shader(const String & filename);
		Shader(const Shader & copy);
		~Shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & vs, const String & fs);
		bool loadFromFile(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const Shader::Source & source);
		bool loadFromMemory(const String & source);
		bool loadFromMemory(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & vs, const String & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Shader & bind(bool bindTextures = true) const;
		
		const Shader & unbind() const;

		static void bind(const Shader * shader, bool bindTextures = true);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto sources() const	-> const Source & { return m_sources; }
		inline auto vertSrc() const -> const String & { return sources().vs; }
		inline auto fragSrc() const -> const String & { return sources().fs; }
		inline auto geomSrc() const -> const String & { return sources().gs; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniform(const Uniform * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniform(const String & name, const bool value) const;
		bool setUniform(const String & name, const float_t value) const;
		bool setUniform(const String & name, const int32_t value) const;
		bool setUniform(const String & name, const vec2 & value) const;
		bool setUniform(const String & name, const vec3 & value) const;
		bool setUniform(const String & name, const vec4 & value) const;
		bool setUniform(const String & name, const vec2i & value) const;
		bool setUniform(const String & name, const vec3i & value) const;
		bool setUniform(const String & name, const vec4i & value) const;
		bool setUniform(const String & name, const mat3 & value) const;
		bool setUniform(const String & name, const mat4 & value) const;
		bool setUniform(const String & name, const Texture & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(const String & name, const int32_t count, const float_t * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec2 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec3 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec4 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const mat3 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const mat4 * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformList(const String & name, const List<float_t> & value) const;
		bool setUniformList(const String & name, const List<vec2> & value) const;
		bool setUniformList(const String & name, const List<vec3> & value) const;
		bool setUniformList(const String & name, const List<vec4> & value) const;
		bool setUniformList(const String & name, const List<mat3> & value) const;
		bool setUniformList(const String & name, const List<mat4> & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool	compile(C_String vs, C_String gs, C_String fs);
		int32_t	getAttribute(const String & value) const;
		int32_t	getUniform(const String & value) const;

		struct UniformBinder;

		using AttribTable	= Tree<String, int32_t>;
		using TextureTable	= Tree<int32_t, const Texture *>;
		using UniformTable	= Tree<String, int32_t>;

		mutable AttribTable		m_attribs;	// Attribute Cache
		mutable TextureTable	m_textures; // Texture Cache
		mutable UniformTable	m_uniforms; // Uniform Cache
		mutable Source			m_sources;	// Sources Cache

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_