#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Graphics/GraphicsEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct Shader::UniformBinder
	{
		uint32_t	cached;
		uint32_t	program;
		int32_t		location;

		UniformBinder(Shader const * value, String const & name)
			: cached { NULL }
			, program { *value }
			, location { -1 }
		{
			if (program)
			{
				cached = ML_GL.getProgramHandle(GL::ProgramObject);

				if (program != cached)
				{
					ML_GL.useProgram(program);
				}

				location = value->getUniformLocation(name);
			}
		}

		~UniformBinder()
		{
			if (program && (program != cached))
			{
				ML_GL.useProgram(cached);
			}
		}

		inline operator bool() const { return (location != -1); }
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		template <class, size_t, size_t> class M,
		class T, size_t X, size_t Y
	> static inline ArrayList<T> makeContiguous(const M<T, X, Y> * value, const size_t length)
	{
		ArrayList<T> temp;
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
		: Handle	(NULL)
		, m_attribs	()
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(String const & filename)
		: Shader {}
	{
		loadFromFile(filename);
	}

	Shader::Shader(Shader const & copy)
		: Shader {}
	{
		loadFromMemory(copy.sources());
	}

	Shader::~Shader() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::dispose()
	{
		m_attribs.clear();
		m_textures.clear();
		m_uniforms.clear();

		ML_GL.useProgram(NULL);
		if (*this)
		{
			ML_GL.deleteShader(*this);
			set_handle(NULL);
		}
		return !(*this);
	}

	bool Shader::loadFromFile(String const & filename)
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			return loadFromMemory(file.str());
		}
		return Debug::logError("Failed opening shader source file \"{0}\"", filename);
	}

	bool Shader::loadFromFile(String const & vs, String const & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed opening vertex source file \"{0}\"", vs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed opening fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return loadFromMemory(vert.str(), frag.str());
	}

	bool Shader::loadFromFile(String const & vs, String const & gs, String const & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed opening vertex source file \"{0}\"", vs);
		}

		// Read the geometry shader file
		static File geom;
		if (!geom.loadFromFile(gs))
		{
			return Debug::logError("Failed opening geometry source file \"{0}\"", gs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed opening fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return loadFromMemory(vert.str(), geom.str(), frag.str());
	}

	bool Shader::loadFromMemory(Source const & value)
	{
		return value.gs
			? loadFromMemory(value.vs, value.gs, value.fs)
			: loadFromMemory(value.vs, value.fs);
	}

	bool Shader::loadFromMemory(String const & value)
	{
		SStream v, g, f;
		return 
			ShaderParser::parseShader(value, v, g, f) &&
			loadFromMemory(v.str(), g.str(), f.str())
			;
	}

	bool Shader::loadFromMemory(String const & source, GL::ShaderType type)
	{
		switch (type)
		{
		case GL::FragmentShader: 
			return loadFromMemory({}, {}, ShaderParser::parseShader(source));
		
		case GL::VertexShader: 
			return loadFromMemory(ShaderParser::parseShader(source), {}, {});
		
		case GL::GeometryShader: 
			return loadFromMemory({}, ShaderParser::parseShader(source), {});
		}
		return false;
	}

	bool Shader::loadFromMemory(String const & vs, String const & fs)
	{
		return loadFromMemory(
			ShaderParser::parseShader(vs), {}, ShaderParser::parseShader(fs)
		);
	}

	bool Shader::loadFromMemory(String const & vs, String const & gs, String const & fs)
	{
		m_sources.vs = (vs ? ShaderParser::parseShader(vs) : String());
		m_sources.gs = (gs ? ShaderParser::parseShader(gs) : String());
		m_sources.fs = (fs ? ShaderParser::parseShader(fs) : String());
		return
			((vs && fs && gs) && compile(vs.c_str(), gs.c_str(), fs.c_str())) ||
			((vs && fs) && compile(vs.c_str(), nullptr, fs.c_str())) ||
			(vs && compile(vs.c_str(), nullptr, nullptr)) ||
			(fs && compile(nullptr, nullptr, fs.c_str())) ||
			(gs && compile(nullptr, gs.c_str(), nullptr))
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Shader const & Shader::bind(bool bindTextures) const
	{
		bind(this, bindTextures);
		return (*this);
	}

	Shader const & Shader::unbind() const
	{
		bind(nullptr, false);
		return (*this);
	}

	void Shader::bind(Shader const * shader, bool bindTextures)
	{
		if (shader && (*shader))
		{
			ML_GL.useProgram(*shader);

			if (bindTextures)
			{
				uint32_t index{ 0 };
				for (auto const & [ key, value ] : shader->m_textures)
				{
					ML_GL.uniform1i(key, index);

					ML_GL.activeTexture(GL::Texture0 + (index++));

					Texture::bind(value);
				}
			}
		}
		else 
		{
			ML_GL.useProgram(NULL);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniform(Uniform const * value) const
	{
		if (!value || !value->getName()) { return false; }
		switch (value->getID())
		{
		case uni_bool	::ID: return setUniform(value->getName(), (*detail::as_bool(value)));
		case uni_float	::ID: return setUniform(value->getName(), (*detail::as_float(value)));
		case uni_int	::ID: return setUniform(value->getName(), (*detail::as_int(value)));
		case uni_vec2	::ID: return setUniform(value->getName(), (*detail::as_vec2(value)));
		case uni_vec3	::ID: return setUniform(value->getName(), (*detail::as_vec3(value)));
		case uni_vec4	::ID: return setUniform(value->getName(), (*detail::as_vec4(value)));
		case uni_color	::ID: return setUniform(value->getName(), (*detail::as_color(value)));
		case uni_mat2	::ID: return setUniform(value->getName(), (*detail::as_mat2(value)));
		case uni_mat3	::ID: return setUniform(value->getName(), (*detail::as_mat3(value)));
		case uni_mat4	::ID: return setUniform(value->getName(), (*detail::as_mat4(value)));
		case uni_sampler::ID: return setUniform(value->getName(), (*detail::as_sampler(value)));
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniform(String const & name, const bool value) const
	{
		return setUniform(name, (int32_t)value);
	}

	bool Shader::setUniform(String const & name, const float_t value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1f(u.location, value);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, const int32_t value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1i(u.location, value);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec2 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2f(u.location, value[0], value[1]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec3 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3f(u.location, value[0], value[1], value[2]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec4 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec2i const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2i(u.location, value[0], value[1]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec3i const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3i(u.location, value[0], value[1], value[2]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, vec4i const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}

	bool Shader::setUniform(String const & name, mat2 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix2fv(u.location, 1, false, value.data());
		}
		return u;
	}

	bool Shader::setUniform(String const & name, mat3 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, 1, false, value.data());
		}
		return u;
	}

	bool Shader::setUniform(String const & name, mat4 const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, 1, false, value.data());
		}
		return u;
	}

	bool Shader::setUniform(String const & name, Texture const & value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			static const size_t max_units
			{
				static_cast<size_t>(ML_GL.getMaxTextureUnits())
			};

			TextureTable::iterator it;
			if ((it = m_textures.find(u.location)) != m_textures.end())
			{
				it->second = &value;
			}
			else if ((m_textures.size() + 1) < max_units)
			{
				m_textures.insert({ u.location, &value });
			}
			else
			{
				return Debug::logError("All available texture units are used: {0}", 
					max_units
				);
			}
		}
		return u;
	}

	bool Shader::setUniform(String const & name, Texture const * value) const
	{
		return value ? setUniform(name, (*value)) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniformArray(String const & name, const int32_t count, float_t const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform1fv(u.location, count, value);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, vec2 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform2fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, vec3 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform3fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, vec4 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniform4fv(u.location, count, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, mat2 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix2fv(u.location, count, false, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, mat3 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, count, false, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	bool Shader::setUniformArray(String const & name, const int32_t count, mat4 const * value) const
	{
		UniformBinder u { this, name };
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, count, false, &makeContiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniformList(String const & name, ArrayList<float_t> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<vec2> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<vec3> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<vec4> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<mat2> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<mat3> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	bool Shader::setUniformList(String const & name, ArrayList<mat4> const & value) const
	{
		return setUniformArray(name, (int32_t)value.size(), value.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Shader::getAttributeLocation(String const & name) const
	{
		if (auto it{ m_attribs.find(name) }; it != m_attribs.end())
		{
			return it->second;
		}
		return m_attribs.insert({ 
			name, ML_GL.getAttribLocation((*this), name.c_str())
		}).first->second;
	}

	int32_t Shader::getUniformLocation(String const & name) const
	{
		if (auto it{ m_uniforms.find(name) }; it != m_uniforms.end())
		{
			return it->second;
		}
		return m_uniforms.insert({
			name, ML_GL.getUniformLocation((*this), name.c_str())
		}).first->second;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::compile(C_String v_src, C_String g_src, C_String f_src)
	{
		if (!ML_GL.shadersAvailable())
		{
			return ml::Debug::logError("Shaders are not available on your system.");
		}

		if (g_src && !ML_GL.geometryShadersAvailable())
		{
			return Debug::logError("Geometry shaders are not available on your system.");
		}

		// Create Program
		if (dispose() && set_handle(ML_GL.createProgram()))
		{
			C_String log { nullptr };

			// Compile Vertex
			/* * * * * * * * * * * * * * * * * * * * */
			uint32_t v { NULL };
			switch (ML_GL.compileShader(v, GL::VertexShader, 1, &v_src, log))
			{
			case ML_SUCCESS: 
				ML_GL.attachShader((*this), v); 
				ML_GL.deleteShader(v);
				break;
			case ML_FAILURE:
				ML_EventSystem.fireEvent<ShaderErrorEvent>(this, GL::VertexShader, log);
				ML_GL.deleteShader(*this);
				return false;
			}

			// Compile Geometry
			/* * * * * * * * * * * * * * * * * * * * */
			uint32_t g { NULL };
			switch (ML_GL.compileShader(g, GL::GeometryShader, 1, &g_src, log))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), g);
				ML_GL.deleteShader(g); 
				break;
			case ML_FAILURE:
				ML_EventSystem.fireEvent<ShaderErrorEvent>(this, GL::GeometryShader, log);
				ML_GL.deleteShader(*this); 
				return false;
			}

			// Compile Fragment
			/* * * * * * * * * * * * * * * * * * * * */
			uint32_t f { NULL };
			switch (ML_GL.compileShader(f, GL::FragmentShader, 1, &f_src, log))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), f); 
				ML_GL.deleteShader(f); 
				break;
			case ML_FAILURE:
				ML_EventSystem.fireEvent<ShaderErrorEvent>(this, GL::FragmentShader, log);
				ML_GL.deleteShader(*this); 
				return false;
			}

			// Link the program
			/* * * * * * * * * * * * * * * * * * * * */
			if (!ML_GL.linkProgram(*this))
			{
				log = ML_GL.getProgramInfoLog(*this);
				ML_GL.deleteShader(*this);
				Debug::logError("Failed linking shader");
				std::cout << log << std::endl;
				ML_GL.flush();
				return false;
			}

			// Good
			ML_GL.flush();
			return true;
		}
		else
		{
			ML_GL.flush();
			return Debug::logError("Failed creating program object");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}