#include <ML/Graphics/Material.hpp>
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
		/* * * * * * * * * * * * * * * * * * * * */

		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Material::apply(const Uniform * value) const
	{
		if (!value) return;
		switch (value->type)
		{
			// Flt1
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt1::ID:
			if (float_t * temp = detail::toFloat(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Int1
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int1::ID:
			if (int32_t * temp = detail::toInt(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (vec2 * temp = detail::toVec2(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (vec3 * temp = detail::toVec3(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (vec4 * temp = detail::toVec4(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (vec4 * temp = detail::toCol4(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (mat3 * temp = detail::toMat3(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (mat4 * temp = detail::toMat4(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Tex2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (const Texture * temp = detail::toTex2(value))
				m_shader->setUniform(value->name, (*temp));
			break;

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_cube::ID:
			if (const Texture * temp = detail::toCube(value))
				m_shader->setUniform(value->name, (*temp));
			break;
		}
	}

	bool Material::bind() const
	{
		if (m_shader && (*m_shader))
		{
			for (const auto & u : m_uniforms)
			{
				if (u && u->name)
				{
					this->apply(u);
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