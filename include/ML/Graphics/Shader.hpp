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

	struct ML_GRAPHICS_API Shader final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Source final { String vs, fs, gs; };
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		explicit Shader(String const & filename);
		Shader(Shader const & copy);
		~Shader() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);
		bool loadFromFile(String const & vs, String const & fs);
		bool loadFromFile(String const & vs, String const & gs, String const & fs);
		bool loadFromMemory(Source const & source);
		bool loadFromMemory(String const & source);
		bool loadFromMemory(String const & source, GL::ShaderType type);
		bool loadFromMemory(String const & vs, String const & fs);
		bool loadFromMemory(String const & vs, String const & gs, String const & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader const & bind(bool bindTextures = true) const;
		
		Shader const & unbind() const;

		static void bind(Shader const * shader, bool bindTextures = true);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto sources() const	-> Source const & { return m_sources; }
		inline auto vertSrc() const -> String const & { return sources().vs; }
		inline auto fragSrc() const -> String const & { return sources().fs; }
		inline auto geomSrc() const -> String const & { return sources().gs; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniform(Uniform const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniform(String const & name, const bool value) const;
		bool setUniform(String const & name, const float_t value) const;
		bool setUniform(String const & name, const int32_t value) const;
		bool setUniform(String const & name, vec2 const & value) const;
		bool setUniform(String const & name, vec3 const & value) const;
		bool setUniform(String const & name, vec4 const & value) const;
		bool setUniform(String const & name, vec2i const & value) const;
		bool setUniform(String const & name, vec3i const & value) const;
		bool setUniform(String const & name, vec4i const & value) const;
		bool setUniform(String const & name, mat2 const & value) const;
		bool setUniform(String const & name, mat3 const & value) const;
		bool setUniform(String const & name, mat4 const & value) const;
		bool setUniform(String const & name, Texture const & value) const;
		bool setUniform(String const & name, Texture const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(String const & name, const int32_t count, float_t const * value) const;
		bool setUniformArray(String const & name, const int32_t count, vec2 const * value) const;
		bool setUniformArray(String const & name, const int32_t count, vec3 const * value) const;
		bool setUniformArray(String const & name, const int32_t count, vec4 const * value) const;
		bool setUniformArray(String const & name, const int32_t count, mat2 const * value) const;
		bool setUniformArray(String const & name, const int32_t count, mat3 const * value) const;
		bool setUniformArray(String const & name, const int32_t count, mat4 const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformList(String const & name, ArrayList<float_t> const & value) const;
		bool setUniformList(String const & name, const ArrayList<vec2> & value) const;
		bool setUniformList(String const & name, const ArrayList<vec3> & value) const;
		bool setUniformList(String const & name, const ArrayList<vec4> & value) const;
		bool setUniformList(String const & name, const ArrayList<mat2> & value) const;
		bool setUniformList(String const & name, const ArrayList<mat3> & value) const;
		bool setUniformList(String const & name, const ArrayList<mat4> & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t	getAttributeLocation(String const & value) const;
		int32_t	getUniformLocation(String const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool compile(C_String vs, C_String gs, C_String fs);

		struct UniformBinder;

		using AttribTable	= Tree<String, int32_t>;
		using TextureTable	= Tree<int32_t, Texture const *>;
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