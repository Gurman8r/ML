#include <ML/Graphics/Material.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Input.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_shader	(nullptr)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader)
		: m_shader	(shader)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader, const List<Uniform *> & uniforms)
		: m_shader	(shader)
		, m_uniforms(uniforms)
	{
	}

	Material::~Material() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::dispose()
	{
		for (auto & elem : m_uniforms)
		{
			delete elem;
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(const String & filename)
	{
		return loadFromFile(filename, nullptr);
	}

	bool Material::loadFromFile(const String & filename, const Map<String, Texture *> * textures)
	{
		// Load uniforms from file
		if (Ifstream file { filename })
		{
			auto pop_front = [](List<String> & toks)
			{
				// Erase begin and return front
				if (toks.empty()) return String();
				String temp = toks.front();
				toks.erase(toks.begin());
				return temp;
			};

			String line;
			while (std::getline(file, line))
			{
				// Skip if empty or comment
				/* * * * * * * * * * * * * * * * * * * * */
				if (line.empty() || line.trim().front() == '#')
					continue;

				// Parse tokens from line
				/* * * * * * * * * * * * * * * * * * * * */
				List<String> tokens = ([](String line)
				{
					List<String> toks;
					if (!line) return toks;
					line.trim()
						.replaceAll("\t", " ")
						.replaceAll(",", "");
					size_t idx = 0;
					while ((idx = line.find(" ")) != String::npos)
					{
						String temp = line.substr(0, idx);
						if (temp) toks.push_back(temp);
						line.erase(0, idx + 1);
					}
					if (line) toks.push_back(line);
					return toks;
				})(line);

				// Parse uniform from tokens
				/* * * * * * * * * * * * * * * * * * * * */
				if (tokens && (pop_front(tokens) == "uniform"))
				{
					// Uniform Type
					/* * * * * * * * * * * * * * * * * * * * */
					const int32_t u_type = ([](C_String type)
					{
						if (!type) return -1;
						for (size_t i = 0; i < Uniform::MAX_UNI_TYPES; i++)
							if (std::strcmp(type, Uniform::TypeNames[i]) == 0)
								return (int32_t)i;
						return -1;
					})(pop_front(tokens).c_str());

					// Uniform Name
					/* * * * * * * * * * * * * * * * * * * * */
					const String u_name = pop_front(tokens);

					// Uniform Data
					/* * * * * * * * * * * * * * * * * * * * */
					SStream u_data = ([](List<String> & toks)
					{
						SStream out;
						if ((toks.size() > 2 && toks.front() == "{" && toks.back() == "}"))
						{
							toks.erase(toks.begin());
							String temp;
							while (toks && ((temp = toks.front()) != "}"))
							{
								out << temp << ' ';
								toks.erase(toks.begin());
							}
						}
						return out;
					})(tokens);

					// Generate Uniform
					/* * * * * * * * * * * * * * * * * * * * */
					if (Uniform * u = ([](int32_t type, const String & name, SStream & ss, const auto * t)
					{
						Uniform * u;
						if ((type == -1) || name.empty() || ss.str().empty())
						{
							return u = nullptr;
						}
						switch (type)
						{
						case uni_bool::ID: return u = new uni_bool(name, input<bool>()(ss));
						case uni_int1::ID: return u = new uni_int1(name, input<int32_t>()(ss));
						case uni_flt1::ID: return u = new uni_flt1(name, input<float_t>()(ss));
						case uni_vec2::ID: return u = new uni_vec2(name, input<vec2>()(ss));
						case uni_vec3::ID: return u = new uni_vec3(name, input<vec3>()(ss));
						case uni_vec4::ID: return u = new uni_vec4(name, input<vec4>()(ss));
						case uni_col4::ID: return u = new uni_col4(name, input<vec4>()(ss));
						case uni_mat3::ID: return u = new uni_mat3(name, input<mat3>()(ss));
						case uni_mat4::ID: return u = new uni_mat4(name, input<mat4>()(ss));
						case uni_sampler::ID:
						{
							Map<String, Texture *>::const_iterator it;
							return (t && ((it = t->find(String(ss.str()).trim())) != t->end()))
								? u = new uni_sampler(name, it->second)
								: u = new uni_sampler(name, nullptr);
						}
						}
						return u = nullptr;
					})(u_type, u_name, u_data, textures))
					{
						m_uniforms.push_back(u);
					}
				}
			}
			
			file.close();
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::apply(const Uniform * value) const
	{
		if (!value || !value->name) 
		{ 
			return false; 
		}
		switch (value->type)
		{
			// Flt1
		case uni_flt1::ID:
			if (auto temp = detail::toFloat(value))
				return m_shader->setUniform(value->name, (*temp));

			// Int1
		case uni_int1::ID:
			if (auto temp = detail::toInt(value))
				return m_shader->setUniform(value->name, (*temp));

			// Vec2
		case uni_vec2::ID:
			if (auto temp = detail::toVec2(value))
				return m_shader->setUniform(value->name, (*temp));

			// Vec3
		case uni_vec3::ID:
			if (auto temp = detail::toVec3(value))
				return m_shader->setUniform(value->name, (*temp));

			// Vec4
		case uni_vec4::ID:
			if (auto temp = detail::toVec4(value))
				return m_shader->setUniform(value->name, (*temp));

			// Col4
		case uni_col4::ID:
			if (auto temp = detail::toCol4(value))
				return m_shader->setUniform(value->name, (*temp));

			// Mat3
		case uni_mat3::ID:
			if (auto temp = detail::toMat3(value))
				return m_shader->setUniform(value->name, (*temp));

			// Mat4
		case uni_mat4::ID:
			if (auto temp = detail::toMat4(value))
				return m_shader->setUniform(value->name, (*temp));

			// Texture
		case uni_sampler::ID:
			if (auto temp = detail::toTex2(value))
				return m_shader->setUniform(value->name, (*temp));
		}
		return false;
	}

	const Material & Material::bind(bool bindTextures) const
	{
		if (m_shader && (*m_shader))
		{
			for (const Uniform * u : (*this))
			{
				if (u && u->name) { this->apply(u); }
			}
			m_shader->bind(bindTextures);
		}
		return (*this);
	}

	const Material & Material::unbind() const
	{
		if (m_shader && (*m_shader))
		{
			m_shader->unbind();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}