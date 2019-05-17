#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Graphics/ShaderParser.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct Shader::UniformBinder
	{
		uint32_t	cached;
		uint32_t	program;
		int32_t		location;

		UniformBinder(const Shader * value, const String & name)
			: cached	(NULL)
			, program	(*value)
			, location	(-1)
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
	Shader::Shader()
		: IHandle(NULL)
		, m_attribs()
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(const Shader & copy)
		: IHandle(copy)
		, m_attribs(copy.m_attribs)
		, m_textures(copy.m_textures)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Shader::~Shader()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::dispose()
	{
		m_attribs.clear();
		m_textures.clear();
		m_uniforms.clear();

		ML_GL.useShader(NULL);
		if ((*this))
		{
			ML_GL.deleteShader((*this));
			get_reference() = NULL;
		}
		return !(*this);
	}

	bool Shader::loadFromFile(const String & filename)
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			return loadFromMemory(file.ToString());
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
		return compile(vert.ToCString(), NULL, frag.ToCString());
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
		return compile(vert.ToCString(), geom.ToCString(), frag.ToCString());
	}

	bool Shader::loadFromMemory(const String & source)
	{
		SStream vert, geom, frag;
		return (ShaderParser::parseShader(source, vert, geom, frag)
			? (loadFromMemory(
				vert.str(), 
				geom.str(), 
				frag.str()))
			: (Debug::logError("Failed Parsing Shader")));
	}

	bool Shader::loadFromMemory(const String & vs, const String & gs, const String & fs)
	{
		return ((gs)
			? (compile(vs.c_str(), gs.c_str(), fs.c_str()))
			: (loadFromMemory(vs, fs))
		);
	}

	bool Shader::loadFromMemory(const String & vs, const String & fs)
	{
		return compile(vs.c_str(), NULL, fs.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Shader::bind(bool bindTextures) const
	{
		bind(this, bindTextures);
	}

	void Shader::unbind() const
	{
		bind(NULL, false);
	}

	void Shader::bind(const Shader * shader, bool bindTextures)
	{
		if (shader && (*shader))
		{
			ML_GL.useShader(*shader);

			if (bindTextures)
			{
				GL::TextureID tex = GL::Texture0;

				for (const auto & pair : shader->m_textures)
				{
					ML_GL.activeTexture(tex++);

					Texture::bind(pair.second);
				}
			}
		}
		else
		{
			ML_GL.useShader(NULL);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Shader::setUniform(const String & name, const float value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform1f(u.location, value);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const int32_t value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform1i(u.location, value);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec2 & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform2f(u.location, value[0], value[1]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec3 & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform3f(u.location, value[0], value[1], value[2]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec4 & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec2i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform2i(u.location, value[0], value[1]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec3i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform3i(u.location, value[0], value[1], value[2]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const vec4i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const mat3 & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, 1, false, value.data());
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const mat4 & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, 1, false, value.data());
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const Texture & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			TextureTable::iterator it;
			if ((it = m_textures.find(u.location)) == m_textures.end())
			{
				static const size_t maxTexUnits = (size_t)(ML_GL.getMaxTextureUnits());

				if ((m_textures.size() + 1) >= maxTexUnits)
				{
					return Debug::logError("All available texture units are used: {0}", 
						maxTexUnits
					);
				}
				else
				{
					m_textures[u.location] = &value;
				}
			}
			else
			{
				it->second = &value;
			}
		}
		return u;
	}

	bool Shader::setUniform(const String & name, const Transform & value) const
	{
		return setUniform(name, value.getMat());
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const float * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform1fv(u.location, count, value);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const vec2 * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform2fv(u.location, count, &vec2::Contiguous(value, (size_t)count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const vec3 * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform3fv(u.location, count, &vec3::Contiguous(value, (size_t)count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const vec4 * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniform4fv(u.location, count, &vec4::Contiguous(value, (size_t)count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const mat3 * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniformMatrix3fv(u.location, count, false, &mat3::Contiguous(value, (size_t)count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, const int32_t count, const mat4 * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			ML_GL.uniformMatrix4fv(u.location, count, false, &mat4::Contiguous(value, (size_t)count)[0]);
		}
		return u;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::setUniformList(const String & name, const List<float> & value) const
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

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::compile(CString vs, CString gs, CString fs)
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
			switch (ML_GL.compileShader(vert, GL::VertexShader, vs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), vert);
				ML_GL.deleteShader(vert);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader((*this));
				return false;
			}

			// Compile Geometry
			uint32_t geom = NULL;
			switch (ML_GL.compileShader(geom, GL::GeometryShader, gs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), geom);
				ML_GL.deleteShader(geom);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader((*this));
				return false;
			}

			// Compile Fragment
			uint32_t frag = NULL;
			switch (ML_GL.compileShader(frag, GL::FragmentShader, fs))
			{
			case ML_SUCCESS:
				ML_GL.attachShader((*this), frag);
				ML_GL.deleteShader(frag);
				break;
			case ML_FAILURE:
				ML_GL.deleteShader((*this));
				return false;
			}

			// Link the program
			if (!ML_GL.linkShader(*this))
			{
				CString log = ML_GL.getProgramInfoLog(*this);
				ML_GL.deleteShader(*this);
				return Debug::logError("Failed linking shader source:\n{0}", log);
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
	
	int32_t Shader::getAttribute(const String & value) const
	{
		AttribTable::const_iterator it;
		if ((it = m_attribs.find(value)) != m_attribs.end())
		{
			return it->second;
		}
		else
		{
			int32_t location;
			if ((location = ML_GL.getAttribLocation((*this), value.c_str())) == -1)
			{
				Debug::logWarning("Attribute Not Found: \"{0}\"", value);
			}
			return m_attribs.insert({ value, location }).first->second;
		}
	}
	
	int32_t Shader::getUniform(const String & value) const
	{
		UniformTable::const_iterator it;
		if ((it = m_uniforms.find(value)) != m_uniforms.end())
		{
			return it->second;
		}
		else
		{
			int32_t location;
			if ((location = ML_GL.getUniformLocation((*this), value.c_str())) == -1)
			{
				Debug::logWarning("Uniform Not Found: \"{0}\"", value);
			}
			return m_uniforms.insert({ value, location }).first->second;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}