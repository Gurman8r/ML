#include <ML/Graphics/Material.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Input.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_shader { nullptr }
		, m_uniforms {}
	{
	}

	Material::Material(const Shader * shader)
		: m_shader { shader }
		, m_uniforms {}
	{
	}

	Material::Material(const Shader * shader, List<Uniform *> && uniforms)
		: m_shader { shader }
		, m_uniforms {}
	{
		for (Uniform *& u : uniforms)
		{
			if (u && u->name && !get(u->name) && (std::find(begin(), end(), u) == end()))
			{
				m_uniforms.push_back(std::move(u));
			}
		}
	}

	Material::Material(const Material & copy)
		: m_shader { copy.m_shader }
		, m_uniforms {}
	{
		for (const Uniform * u : copy)
		{
			this->insert(u->clone());
		}
	}

	Material::~Material() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		if (std::ifstream file { filename })
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
				if (!tokens.empty() && (pop_front(tokens) == "uniform"))
				{
					// Uniform Type
					/* * * * * * * * * * * * * * * * * * * * */
					const int32_t u_type = ([](C_String type)
					{
						if (!type) return -1;
						for (size_t i = 0; i < ML_ARRAYSIZE(Uniform::Type_names); i++)
							if (std::strcmp(type, Uniform::Type_names[i]) == 0)
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
							while (!toks.empty() && ((temp = toks.front()) != "}"))
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
						case uni_bool::	ID	: return u = new uni_bool(name, input<bool>()(ss));
						case uni_int::	ID	: return u = new uni_int(name, input<int32_t>()(ss));
						case uni_float::ID	: return u = new uni_float(name, input<float_t>()(ss));
						case uni_vec2::	ID	: return u = new uni_vec2(name, input<vec2>()(ss));
						case uni_vec3::	ID	: return u = new uni_vec3(name, input<vec3>()(ss));
						case uni_vec4::	ID	: return u = new uni_vec4(name, input<vec4>()(ss));
						case uni_color::ID	: return u = new uni_color(name, input<vec4>()(ss));
						case uni_mat2::	ID	: return u = new uni_mat2(name, input<mat2>()(ss));
						case uni_mat3::	ID	: return u = new uni_mat3(name, input<mat3>()(ss));
						case uni_mat4::	ID	: return u = new uni_mat4(name, input<mat4>()(ss));
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material & Material::setShader(const Shader * value)
	{
		m_shader = value;
		return (*this);
	}

	const Material & Material::bind() const
	{
		if (m_shader && (*m_shader))
		{
			m_shader->bind(false);
			for (const auto & elem : (*this))
			{
				if (!m_shader->setUniform(elem))
				{
					/* error */
				}
			}
			m_shader->bind(true);
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}