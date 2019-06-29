#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>

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

	Material::Material(const Shader * shader, const UniformList & uniforms)
		: m_shader	(shader)
		, m_uniforms()
	{
		for (auto & e : uniforms)
		{
			m_uniforms[e->name] = e;
		}
	}

	Material::Material(const Material & copy)
		: m_shader	(copy.m_shader)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Material::~Material()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::dispose()
	{
		for (auto & pair : m_uniforms)
		{
			delete (I_Newable *)pair.second;
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(const String & filename)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto pop_front = [](List<String> & toks)
		{
			if (toks.empty()) return String();
			String temp = toks.front();
			toks.erase(toks.begin());
			return temp;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (auto file = Ifstream(filename))
		{
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

				// Parse tokens
				/* * * * * * * * * * * * * * * * * * * * */
				if (tokens && (pop_front(tokens) == "uniform"))
				{
					const String value_type = pop_front(tokens); // Uniform Type
					const String value_name = pop_front(tokens); // Uniform Name
					if (value_type && value_name)
					{
						// Uniform Values
						/* * * * * * * * * * * * * * * * * * * * */
						List<String> value_list = ([](List<String> & value)
						{
							List<String> out;
							if ((value.size() > 2) &&
								(value.front() == "{") && 
								(value.back() == "}"))
							{
								value.erase(value.begin());
								String temp;
								while (value && ((temp = value.front()) != "}"))
								{
									out.push_back(temp);
									value.erase(value.begin());
								}
							}
							return out;
						})(tokens);

						// Index of Type Name
						/* * * * * * * * * * * * * * * * * * * * */
						int32_t type_index = ([](C_String typeName) {
							for (size_t i = 0; i < Uniform::MAX_UNI_TYPES; i++)
								if (std::strcmp(typeName, Uniform::TypeNames[i]) == 0)
									return (int32_t)i;
							return -1;
						})(value_type.c_str());

						// Display
						/* * * * * * * * * * * * * * * * * * * * */
						C_String type_name = nullptr;
						if (alg::value_at(type_index, type_name, Uniform::TypeNames))
						{
							cout 
								<< "OK: "
								<< "[" << type_name << "] "
								<< "\'" << value_name << "\'";
							for (size_t i = 0; i < value_list.size(); i++)
							{
								cout
									<< (i == 0 ? " { " : "")
									<< value_list[i]
									<< ((i < value_list.size() - 1) ? ", " : " }\n");
							}
						}
					}
				}
			}
			file.close();
			return true;
		}
		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Material::apply(const Uniform * value) const
	{
		switch (value->type)
		{
			// Flt
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt::ID:
			if (auto * temp = impl::toFloat(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Int
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int::ID:
			if (auto * temp = impl::toInt(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (auto * temp = impl::toVec2(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (auto * temp = impl::toVec3(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (auto * temp = impl::toVec4(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (auto * temp = impl::toCol4(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (auto * temp = impl::toMat3(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (auto * temp = impl::toMat4(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;

			// Tex
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto * temp = impl::toTex2(value))
			{
				m_shader->setUniform(value->name, (*temp));
			}
			break;
		}
	}

	bool Material::bind() const
	{
		if (m_shader && (*m_shader))
		{
			for (const auto & pair : m_uniforms)
			{
				if (pair.second && pair.second->name)
				{
					this->apply(pair.second);
				}
			}
			m_shader->bind();
			return true;
		}
		return false;
	}

	void Material::unbind() const
	{
		if (m_shader && (*m_shader))
		{
			m_shader->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}