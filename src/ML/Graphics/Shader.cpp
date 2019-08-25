#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Uniform.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct Shader::UniformBinder
	{
		uint32_t	cached;
		uint32_t	program;
		int32_t		location;

		UniformBinder(const Shader * value, const String & name)
			: cached { NULL }
			, program { *value }
			, location { -1 }
		{
			if (program)
			{
				cached = ML_GL.getProgramHandle(GL::ProgramObject);

				if (program != cached)
				{
					ML_GL.useShader(program);
				}

				location = value->getUniform(name);
			}
		}

		~UniformBinder()
		{
			if (program && (program != cached))
			{
				ML_GL.useShader(cached);
			}
		}

		inline operator bool() const
		{
			return (location != -1);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		template <class, size_t, size_t> class M,
		class T, size_t X, size_t Y
	> static inline List<T> makeContiguous(const M<T, X, Y> * value, const size_t length)
	{
		List<T> temp;
		if (value)
		{
			if (const size_t imax = (length * (X * Y)))
			{
				if (temp.size() != imax)
				{
					temp.resize(imax);
				}
				for (size_t i = 0; i < imax; i++)
				{
					temp[i] = value[i / (X * Y)][i % (X * Y)];
				}
			}
			else if (!temp.empty())
			{
				temp.clear();
			}
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Shader::Shader()
		: I_Handle	(NULL)
		, m_attribs	()
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(const Shader & copy)
		: I_Handle	(copy)
		, m_attribs	(copy.m_attribs)
		, m_textures(copy.m_textures)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Shader::~Shader() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::dispose()
	{
		m_attribs.clear();
		m_textures.clear();
		m_uniforms.clear();

		ML_GL.useShader(NULL);
		if (*this)
		{
			ML_GL.deleteShader(*this);
			set_handle(NULL);
		}
		return !(*this);
	}

	bool Shader::loadFromFile(const String & filename)
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			return loadFromMemory(file.str());
		}
		return Debug::logError("Failed to open shader source file \"{0}\"", filename);
	}

	bool Shader::loadFromFile(const String & vs, const String & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return loadFromMemory(vert.str(), frag.str());
	}

	bool Shader::loadFromFile(const String & vs, const String & gs, const String & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the geometry shader file
		static File geom;
		if (!geom.loadFromFile(gs))
		{
			return Debug::logError("Failed to open geometry source file \"{0}\"", gs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return loadFromMemory(vert.str(), geom.str(), frag.str());
	}

	bool Shader::loadFromMemory(const Shader::Source & source)
	{
		return source.gs
			? loadFromMemory(source.vs, source.gs, source.fs)
			: loadFromMemory(source.vs, source.fs);
	}

	bool Shader::loadFromMemory(const String & source)
	{
		SStream vert, geom, frag;
		return (ShaderParser::parseShader(source, vert, geom, frag)
			? loadFromMemory(vert.str(), geom.str(), frag.str())
			: Debug::logError("Failed Parsing Shader")
		);
	}

	bool Shader::loadFromMemory(const String & vs, const String & gs, const String & fs)
	{
		if (gs)
		{
			if (compile(vs.c_str(), gs.c_str(), fs.c_str()))
			{
				m_sources.vs = ShaderParser::parseShader(vs);
				m_sources.gs = ShaderParser::parseShader(gs);
				m_sources.fs = ShaderParser::parseShader(fs);
				return true;
			}
			return false;
		}
		return loadFromMemory(vs, fs);
	}

	bool Shader::loadFromMemory(const String & vs, const String & fs)
	{
		if (compile(vs.c_str(), nullptr, fs.c_str()))
		{
			m_sources.vs = ShaderParser::parseShader(vs);
			m_sources.gs = String();
			m_sources.fs = ShaderParser::parseShader(fs);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Shader & Shader::bind(bool bindTextures) const
	{
		bind(this, bindTextures);
		return (*this);
	}

	const Shader & Shader::unbind() const
	{
		bind(nullptr, false);
		return (*this);
	}

	void Shader::bind(const Shader * shader, bool bindTextures)
	{
		if (shader && (*shader))
		{
			ML_GL.useShader(*shader);

			if (bindTextures)
			{
				uint32_t i = 0;
				for (const auto & pair : shader->m_textures)
				{
					ML_GL.activeTexture(GL::Texture0 + (i++));
					Texture::bind(pair.second);
				}
			}
		}
		else
		{
			ML_GL.useShader(NULL);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniform(const Uniform * value) const
	{
		if (value && value->name)
		{
			switch (value->id)
			{
			case uni_bool::ID: if (auto temp = detail::as_bool(value))
				return this->setUniform(value->name, (*temp));

			case uni_float::ID: if (auto temp = detail::as_float(value))
				return this->setUniform(value->name, (*temp));

			case uni_int::ID: if (auto temp = detail::as_int(value))
				return this->setUniform(value->name, (*temp));

			case uni_vec2::ID: if (auto temp = detail::as_vec2(value))
				return this->setUniform(value->name, (*temp));

			case uni_vec3::ID: if (auto temp = detail::as_vec3(value))
				return this->setUniform(value->name, (*temp));

			case uni_vec4::ID: if (auto temp = detail::as_vec4(value))
				return this->setUniform(value->name, (*temp));

			case uni_color::ID: if (auto temp = detail::as_color(value))
				return this->setUniform(value->name, (*temp));

			case uni_mat3::ID: if (auto temp = detail::as_mat3(value))
				return this->setUniform(value->name, (*temp));

			case uni_mat4::ID: if (auto temp = detail::as_mat4(value))
				return this->setUniform(value->name, (*temp));

			case uni_sampler::ID: if (auto temp = detail::as_sampler(value))
				return this->setUniform(value->name, (*temp));
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniform(const String & name, const bool value) const
	{
		return setUniform(name, (int32_t)value);
	}

	bool Shader::setUniform(const String & name, const float_t value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1f(u.location, value);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const int32_t value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1i(u.location, value);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec2 & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2f(u.location, value[0], value[1]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec3 & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3f(u.location, value[0], value[1], value[2]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec4 & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec2i & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2i(u.location, value[0], value[1]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec3i & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3i(u.location, value[0], value[1], value[2]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const vec4i & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const mat3 & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, 1, false, value.data());
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const mat4 & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, 1, false, value.data());
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const Texture & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			static const size_t max_tex_units
			{
				static_cast<size_t>(ML_GL.getMaxTextureUnits())
			};

			TextureTable::iterator it;
			if ((it = m_textures.find(u.location)) != m_textures.end())
			{
				it->second = &value;
			}
			else if ((m_textures.size() + 1) < max_tex_units)
			{
				m_textures.insert({ u.location, &value });
			}
			else
			{
				return Debug::logError("All available texture units are used: {0}", 
					max_tex_units
				);
			}
		}
		return u;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniformArray(const String & name, const int32_t count, const float_t * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1fv(u.location, count, value);
		}
		return u;
	}

	bool Shader::setUniformArray(const String & name, const int32_t count, const vec2 * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(const String & name, const int32_t count, const vec3 * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(const String & name, const int32_t count, const vec4 * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(const String & name, const int32_t count, const mat3 * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, count, false, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(const String & name, const int32_t count, const mat4 * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, count, false, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniformList(const String & name, const List<float_t> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(const String & name, const List<vec2> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(const String & name, const List<vec3> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(const String & name, const List<vec4> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(const String & name, const List<mat3> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(const String & name, const List<mat4> & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::compile(C_String vs, C_String gs, C_String fs)
	{
		if (!ML_GL.shadersAvailable())
		{
			return ml::Debug::logError("Shaders are not available on your system.");
		}

		if (gs && !ML_GL.geometryShadersAvailable())
		{
			return Debug::logError("Geometry shaders are not available on your system.");
		}

		// Create Program
		if (dispose() && set_handle(ML_GL.createProgramObject()))
		{
			// Compile Vertex
			uint32_t vert = NULL;
			switch (ML_GL.compileShader(vert, GL::VertexShader, 1, &vs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), vert);
				ML_GL.deleteShader(vert);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader(*this);
				return false;
			}

			// Compile Geometry
			uint32_t geom = NULL;
			switch (ML_GL.compileShader(geom, GL::GeometryShader, 1, &gs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), geom);
				ML_GL.deleteShader(geom);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader(*this);
				return false;
			}

			// Compile Fragment
			uint32_t frag = NULL;
			switch (ML_GL.compileShader(frag, GL::FragmentShader, 1, &fs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), frag);
				ML_GL.deleteShader(frag);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader(*this);
				return false;
			}

			// Link the program
			if (!ML_GL.linkShader(*this))
			{
				C_String log = ML_GL.getProgramInfoLog(*this);
				ML_GL.deleteShader(*this);
				cout << log << endl;
				return Debug::logError("Failed linking shader");
			}

			// Refresh OpenGL
			ML_GL.flush();
			return true;
		}
		else
		{
			return Debug::logError("Failed compiling shader");
		}
	}

	int32_t Shader::getAttribute(const String & name) const
	{
		Tree<String, int32_t>::iterator it;
		if ((it = m_attribs.find(name)) != m_attribs.end())
		{
			return it->second;
		}
		else
		{
			int32_t value;
			if ((value = ML_GL.getAttribLocation((*this), name.c_str())) == -1)
			{
				/* attribute not found */
			}
			return m_attribs.insert({ name, value }).first->second;
		}
	}

	int32_t Shader::getUniform(const String & name) const
	{
		Tree<String, int32_t>::iterator it;
		if ((it = m_uniforms.find(name)) != m_uniforms.end())
		{
			return it->second;
		}
		else
		{
			int32_t value;
			if ((value = ML_GL.getUniformLocation((*this), name.c_str())) == -1)
			{
				/* uniform not found */
			}
			return m_uniforms.insert({ name, value }).first->second;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}