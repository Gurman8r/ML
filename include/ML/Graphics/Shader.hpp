#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/Color.hpp>

#ifndef ML_IMPL_UNI_MAIN_COLOR
#	define ML_IMPL_UNI_MAIN_COLOR "u_color"
#endif

#ifndef ML_IMPL_UNI_MAIN_TEXTURE
#	define ML_IMPL_UNI_MAIN_TEXTURE "u_texture0"
#endif

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

		bool setUniform(String const & name, bool value) const;
		bool setUniform(String const & name, float_t value) const;
		bool setUniform(String const & name, int32_t value) const;
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

		bool setUniformArray(String const & name, int32_t count, int32_t const * value) const;
		bool setUniformArray(String const & name, int32_t count, float_t const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec2 const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec3 const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec4 const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat2 const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat3 const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat4 const * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(String const & name, std::vector<int32_t> const & value) const;
		bool setUniformArray(String const & name, std::vector<float_t> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec2> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec3> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec4> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat2> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat3> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat4> const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t	getAttributeLocation(String const & value) const;
		int32_t	getUniformLocation(String const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool compile(C_String vs, C_String gs, C_String fs);

		struct UniformBinder;

		using AttribTable	= std::map<String, int32_t>;
		using TextureTable	= std::map<int32_t, Texture const *>;
		using UniformTable	= std::map<String, int32_t>;

		mutable AttribTable		m_attribs;	// Attribute Cache
		mutable TextureTable	m_textures; // Texture Cache
		mutable UniformTable	m_uniforms; // Uniform Cache
		mutable Source			m_sources;	// Sources Cache

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_