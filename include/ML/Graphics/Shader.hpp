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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniform(String const & name, bool value) const;
		bool setUniform(String const & name, float_t value) const;
		bool setUniform(String const & name, int32_t value) const;
		bool setUniform(String const & name, vec2f const & value) const;
		bool setUniform(String const & name, vec3f const & value) const;
		bool setUniform(String const & name, vec4f const & value) const;
		bool setUniform(String const & name, vec2i const & value) const;
		bool setUniform(String const & name, vec3i const & value) const;
		bool setUniform(String const & name, vec4i const & value) const;
		bool setUniform(String const & name, mat2f const & value) const;
		bool setUniform(String const & name, mat3f const & value) const;
		bool setUniform(String const & name, mat4f const & value) const;
		bool setUniform(String const & name, Texture const & value) const;
		bool setUniform(String const & name, Texture const * value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(String const & name, int32_t count, int32_t const * value) const;
		bool setUniformArray(String const & name, int32_t count, float_t const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec2f const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec3f const * value) const;
		bool setUniformArray(String const & name, int32_t count, vec4f const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat2f const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat3f const * value) const;
		bool setUniformArray(String const & name, int32_t count, mat4f const * value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(String const & name, std::vector<int32_t> const & value) const;
		bool setUniformArray(String const & name, std::vector<float_t> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec2f> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec3f> const & value) const;
		bool setUniformArray(String const & name, std::vector<vec4f> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat2f> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat3f> const & value) const;
		bool setUniformArray(String const & name, std::vector<mat4f> const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t	getAttributeLocation(String const & value) const;
		int32_t	getUniformLocation(String const & value) const;
		int32_t compile(C_String vs, C_String gs, C_String fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		struct UniformBinder;

		using AttribCache	= std::map<String, int32_t>;
		using TextureCache	= std::map<int32_t, Texture const *>;
		using UniformCache	= std::map<String, int32_t>;

		mutable AttribCache		m_attribs;
		mutable TextureCache	m_textures;
		mutable UniformCache	m_uniforms;
		mutable Source			m_sources;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_