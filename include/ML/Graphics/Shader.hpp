#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/Color.hpp>

// uniform names used internally by text and sprite
// would like to phase these out
#define ML_UNI_MAIN_COL "u_color"
#define ML_UNI_MAIN_TEX "u_texture0"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Shader final : public Trackable, public Disposable, public Handle<uint32_t>
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
		bool loadFromFile(const String & filename);
		bool loadFromFile(const String & vs, const String & fs);
		bool loadFromFile(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const Source & source);
		bool loadFromMemory(const String & source);
		bool loadFromMemory(const String & source, GL::ShaderType type);
		bool loadFromMemory(const String & vs, const String & fs);
		bool loadFromMemory(const String & vs, const String & gs, const String & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Shader & bind(bool bindTextures = true) const;
		
		const Shader & unbind() const;

		static void bind(Shader const * shader, bool bindTextures = true);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto sources() const	-> const Source & { return m_sources; }
		inline auto vertSrc() const -> const String & { return sources().vs; }
		inline auto fragSrc() const -> const String & { return sources().fs; }
		inline auto geomSrc() const -> const String & { return sources().gs; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniform(Uniform const * value) const;

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
		bool setUniform(const String & name, const mat2 & value) const;
		bool setUniform(const String & name, const mat3 & value) const;
		bool setUniform(const String & name, const mat4 & value) const;
		bool setUniform(const String & name, const Texture & value) const;
		bool setUniform(const String & name, Texture const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(const String & name, const int32_t count, float_t const * value) const;
		bool setUniformArray(const String & name, const int32_t count, vec2 const * value) const;
		bool setUniformArray(const String & name, const int32_t count, vec3 const * value) const;
		bool setUniformArray(const String & name, const int32_t count, vec4 const * value) const;
		bool setUniformArray(const String & name, const int32_t count, mat2 const * value) const;
		bool setUniformArray(const String & name, const int32_t count, mat3 const * value) const;
		bool setUniformArray(const String & name, const int32_t count, mat4 const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformList(const String & name, const List<float_t> & value) const;
		bool setUniformList(const String & name, const List<vec2> & value) const;
		bool setUniformList(const String & name, const List<vec3> & value) const;
		bool setUniformList(const String & name, const List<vec4> & value) const;
		bool setUniformList(const String & name, const List<mat2> & value) const;
		bool setUniformList(const String & name, const List<mat3> & value) const;
		bool setUniformList(const String & name, const List<mat4> & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t	getAttributeLocation(const String & value) const;
		int32_t	getUniformLocation(const String & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool compile(C_String vs, C_String gs, C_String fs);

		struct UniformBinder;

		using AttribTable	= Map<String, int32_t>;
		using TextureTable	= Map<int32_t, Texture const *>;
		using UniformTable	= Map<String, int32_t>;

		mutable AttribTable		m_attribs;	// Attribute Cache
		mutable TextureTable	m_textures; // Texture Cache
		mutable UniformTable	m_uniforms; // Uniform Cache
		mutable Source			m_sources;	// Sources Cache

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_