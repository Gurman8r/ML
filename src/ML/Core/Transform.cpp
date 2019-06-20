#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_matrix(mat4::Identity())
	{
	}

	Transform::Transform(const vec3 & pos, const vec3 & scl, const quat & rot)
		: m_matrix(mat4::Identity())
	{
		(*this)
			.translate(pos)
			.rotate(rot.real(), rot.complex())
			.scale(scl);
	}

	Transform::Transform(const mat4 & value)
		: m_matrix(value)
	{
	}

	Transform::Transform(const Transform & copy)
		: m_matrix(copy.m_matrix)
	{
	}

	Transform::~Transform()
	{
	}

	
	// Static Functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Transform::Decompose(const mat4 & value, vec3 & scl, quat & rot, vec3 & tns, vec3 & skw, vec4 & psp)
	{
		static glm::vec3 _scl; // scale
		static glm::quat _rot; // orientation
		static glm::vec3 _tns; // translation
		static glm::vec3 _skw; // skew
		static glm::vec4 _psp; // perspective
		if (glm::decompose((glm::mat4)(value), _scl, _rot, _tns, _skw, _psp))
		{
			scl = _scl;
			rot = _rot;
			tns = _tns;
			skw = _skw;
			psp = _psp;
			return true;
		}
		return false;
	}

	mat4 Transform::LookAt(const vec3 & eye, const vec3 & pos, const vec3 & up)
	{
		return glm::lookAt(
			(glm::vec3)(eye),
			(glm::vec3)(pos),
			(glm::vec3)(up)
		);
	}

	mat4 Transform::LookDir(const vec3 & eye, const vec3 & dir)
	{
		return LookAt(
			eye,
			(dir - eye).normalized(),
			vec3::Up
		);
	}

	mat4 Transform::Inverse(const mat4 & value)
	{
		return mat4(glm::inverse((glm::mat4)value));
	}

	mat4 Transform::Orthographic(const FloatRect & area)
	{
		return mat4(glm::ortho(
			area.left(),
			area.right(),
			area.bottom(),
			area.top()
		));
	}

	mat4 Transform::Orthographic(const FloatRect & area, const float zNear, const float zFar)
	{
		return mat4(glm::ortho(
			area.left(),
			area.right(),
			area.bottom(),
			area.top(),
			zNear,
			zFar
		));
	}

	mat4 Transform::Perspective(const float fov, const float aspect, const float zNear, const float zFar)
	{
		return mat4(glm::perspective(
			fov,
			aspect,
			zNear,
			zFar
		));
	}

	mat3 Transform::RebaseMatrix(const mat3 & mIn, const mat4 & tIn)
	{
		glm::mat3 m = (glm::mat3)mIn;
		glm::mat4 t = (glm::mat4)tIn;

		//sourced directly from a3ds a3particleRebaseMatrix_internal by Daniel Buckstein
		glm::mat3 tmp;
		// first column
		tmp[0][0] = m[0][0] * t[0][0] + m[1][0] * t[1][0] + m[2][0] * t[2][0];
		tmp[0][1] = m[0][1] * t[0][0] + m[1][1] * t[1][0] + m[2][1] * t[2][0];
		tmp[0][2] = m[0][2] * t[0][0] + m[1][2] * t[1][0] + m[2][2] * t[2][0];

		// second column
		tmp[1][0] = m[0][0] * t[0][1] + m[1][0] * t[1][1] + m[2][0] * t[2][1];
		tmp[1][1] = m[0][1] * t[0][1] + m[1][1] * t[1][1] + m[2][1] * t[2][1];
		tmp[1][2] = m[0][2] * t[0][1] + m[1][2] * t[1][1] + m[2][2] * t[2][1];

		// third column
		tmp[2][0] = m[0][0] * t[0][2] + m[1][0] * t[1][2] + m[2][0] * t[2][2];
		tmp[2][1] = m[0][1] * t[0][2] + m[1][1] * t[1][2] + m[2][1] * t[2][2];
		tmp[2][2] = m[0][2] * t[0][2] + m[1][2] * t[1][2] + m[2][2] * t[2][2];

		// now (t * tmp)
		glm::mat3 out;
		out[0][0] = t[0][0] * tmp[0][0] + t[1][0] * tmp[0][1] + t[2][0] * tmp[0][2];
		out[0][1] = t[0][1] * tmp[0][0] + t[1][1] * tmp[0][1] + t[2][1] * tmp[0][2];
		out[0][2] = t[0][2] * tmp[0][0] + t[1][2] * tmp[0][1] + t[2][2] * tmp[0][2];

		out[1][0] = t[0][0] * tmp[1][0] + t[1][0] * tmp[1][1] + t[2][0] * tmp[1][2];
		out[1][1] = t[0][1] * tmp[1][0] + t[1][1] * tmp[1][1] + t[2][1] * tmp[1][2];
		out[1][2] = t[0][2] * tmp[1][0] + t[1][2] * tmp[1][1] + t[2][2] * tmp[1][2];

		out[2][0] = t[0][0] * tmp[2][0] + t[1][0] * tmp[2][1] + t[2][0] * tmp[2][2];
		out[2][1] = t[0][1] * tmp[2][0] + t[1][1] * tmp[2][1] + t[2][1] * tmp[2][2];
		out[2][2] = t[0][2] * tmp[2][0] + t[1][2] * tmp[2][1] + t[2][2] * tmp[2][2];

		return out;
	}

	vec3 Transform::RebasePoint(const vec3 & value, const mat4 & tt)
	{
		glm::vec3 v = (glm::vec3)value;
		glm::mat4 t = (glm::mat4)tt;

		return vec3(
			t[0][0] * v[0] + t[1][0] * v[1] + t[2][0] * v[2] + t[3][0],
			t[0][1] * v[0] + t[1][1] * v[1] + t[2][1] * v[2] + t[3][1],
			t[0][2] * v[0] + t[1][2] * v[1] + t[2][2] * v[2] + t[3][2]
		);
	}
	
	mat4 Transform::Rotate(const mat4 & m, const float angle, const vec3 & v)
	{
		//const float a = angle;
		//const float c = std::cosf(a);
		//const float s = std::sinf(a);
		//vec3 axis = v.normalized();
		//vec3 temp = (axis * (1.0f * c));

		return glm::rotate(
			(glm::mat4)(m), 
			angle, 
			(glm::vec3)(v)
		);
	}

	mat4 Transform::Rotate(const mat4 & value, const quat & rot)
	{
		return Rotate(
			value,
			rot.angle(),
			rot.axis()
		);
	}

	mat4 Transform::Scale(const mat4 & value, const vec3 & scl)
	{
		return glm::scale((glm::mat4)(value), (glm::vec3)(scl));
	}

	mat4 Transform::Translate(const mat4 & value, const vec3 & tns)
	{
		return glm::translate((glm::mat4)(value), (glm::vec3)(tns));
	}

	mat4 Transform::Transpose(const mat4 & value)
	{
		return glm::transpose((glm::mat4)value);
	}

	
	// Member Functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::rotate(const float angle, const vec3 & axis)
	{
		return update(Rotate(m_matrix, angle, axis));
	}

	Transform & Transform::rotate(const quat & value)
	{
		return update(Rotate(m_matrix, value));
	}

	Transform & Transform::scale(const vec3 & value)
	{
		return update(Scale(m_matrix, value));
	}

	Transform & Transform::translate(const vec3 & value)
	{
		return update(Translate(m_matrix, value));
	}

	Transform & Transform::lookAt(const vec3 & eye, const vec3 & pos, const vec3 & up)
	{
		return update(LookAt(eye, pos, up));
	}

	Transform & Transform::lookAt(const vec3 & eye, const vec3 & pos)
	{
		return lookAt(eye, pos, vec3::Up);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::update(const Transform & other)
	{
		return update(other.getMat());
	}

	Transform & Transform::update(const mat4 & value)
	{
		m_matrix = value;
		m_changed = true;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Transform::decompose(vec3 & scl, quat & rot, vec3 & tns, vec3 & skw, vec4 & psp) const
	{
		return Decompose(m_matrix, scl, rot, tns, skw, psp);
	}

	const Transform & Transform::decompose() const
	{
		if (m_changed)
		{
			if (decompose(m_scl, m_rot, m_pos, m_skw, m_psp))
			{
				m_changed = false;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const mat4 & Transform::getMat()	const { return m_matrix; }
	const mat4	 Transform::getInv()	const { return Inverse(m_matrix); }
	const vec4 & Transform::getPsp()	const { return decompose().m_psp; }
	const vec3 & Transform::getPos()	const { return decompose().m_pos; }
	const quat & Transform::getRot()	const { return decompose().m_rot; }
	const vec3 & Transform::getScl()	const { return decompose().m_scl; }
	const vec3 & Transform::getSkw()	const { return decompose().m_skw; }
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}