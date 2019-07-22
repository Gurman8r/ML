#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Core/Cache.hpp>
#include <ML/Graphics/Color.hpp>

// Uniforms names used internally
#define ML_FRAG_MAIN_COL "u_mainColor"
#define ML_FRAG_MAIN_TEX "u_mainTexture"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Shader final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		Shader(const Shader & copy);
		~Shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const String & vertSrc() const { return m_vs; }
		inline const String & fragSrc() const { return m_fs; }
		inline const String & geomSrc() const { return m_gs; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & vs, const String & fs);
		bool loadFromFile(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & source);
		bool loadFromMemory(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & vs, const String & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind(bool bindTextures = true) const;
		
		void unbind() const;

		static void bind(const Shader * shader, bool bindTextures = true);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniformArray(const String & name, const int32_t count, const float_t * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec2 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec3 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const vec4 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const mat3 * value) const;
		bool setUniformArray(const String & name, const int32_t count, const mat4 * value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setUniformList(const String & name, const List<float_t> & value) const;
		bool setUniformList(const String & name, const List<vec2> & value) const;
		bool setUniformList(const String & name, const List<vec3> & value) const;
		bool setUniformList(const String & name, const List<vec4> & value) const;
		bool setUniformList(const String & name, const List<mat3> & value) const;
		bool setUniformList(const String & name, const List<mat4> & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	private:
		bool	compile(C_String vs, C_String gs, C_String fs);
		int32_t	getAttribute(const String & value) const;
		int32_t	getUniform(const String & value) const;

		struct UniformBinder;

		using AttribTable	= TreeCache<String, int32_t>;
		using TextureTable	= Map<int32_t, const Texture *>;
		using UniformTable	= TreeCache<String, int32_t>;

		mutable AttribTable		m_attribs;
		mutable TextureTable	m_textures;
		mutable UniformTable	m_uniforms;
		mutable String			m_vs, m_fs, m_gs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_