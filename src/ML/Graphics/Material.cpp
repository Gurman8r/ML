#include <ML/Graphics/Material.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_shader	(NULL)
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
			delete (IObject *)pair.second;
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(const String & filename)
	{
		SStream ss;
		if (ML_FS.getFileContents(filename, ss))
		{
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::bind() const
	{
		if (m_shader && (*m_shader))
		{
			for (const auto & pair : m_uniforms)
			{
				if (!pair.second || !pair.second->name)
					continue;

				switch (pair.second->type)
				{
					// Flt
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_flt::ID:
					if (auto u = dynamic_cast<const uni_flt *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_flt_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_flt_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Int
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_int::ID:
					if (auto u = dynamic_cast<const uni_int *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_int_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_int_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Vec2
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_vec2::ID:
					if (auto u = dynamic_cast<const uni_vec2 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec2_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec2_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Vec3
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_vec3::ID:
					if (auto u = dynamic_cast<const uni_vec3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec3_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec3_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Vec4
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_vec4::ID:
					if (auto u = dynamic_cast<const uni_vec4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec4_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec4_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Col4
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_col4::ID:
					if (auto u = dynamic_cast<const uni_col4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_col4_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_col4_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Mat3
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_mat3::ID:
					if (auto u = dynamic_cast<const uni_mat3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat3_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat3_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Mat4
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_mat4::ID:
					if (auto u = dynamic_cast<const uni_mat4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat4_cr *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat4_cp *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;

					// Tex
					/* * * * * * * * * * * * * * * * * * * * */
				case uni_tex2::ID:
					if (auto u = dynamic_cast<const uni_tex2 *>(pair.second))
						m_shader->setUniform(u->name, (*u->data));
					break;
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