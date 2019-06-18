#include <ML/Graphics/Material.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Graphics/Uniform.hpp>

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
			delete (IObject *)pair.second;
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(const String & filename)
	{
		if (auto file = std::ifstream(filename))
		{
			String line;
			while (std::getline(file, line))
			{
				if (line.empty() || line.trim().front() == '#')
					continue;


			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Material::apply(const uni_base * value) const
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