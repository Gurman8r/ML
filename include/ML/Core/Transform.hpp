#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <ML/Core/Quaternion.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Transform final
		: public INewable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform();
		Transform(const vec3 & pos, const vec3 & scl, const quat & rot);
		Transform(const mat4 & value);
		Transform(const Transform & copy);
		~Transform();


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool Decompose(const mat4 & value, vec3 & scl, quat & rot, vec3 & tns, vec3 & skw, vec4 & psp);
		static mat4 Inverse(const mat4 & value);
		static mat4 LookAt(const vec3 & eye, const vec3 & pos, const vec3 & up);
		static mat4 LookDir(const vec3 & eye, const vec3 & dir);
		static mat4 Orthographic(const FloatRect & area);
		static mat4 Orthographic(const FloatRect & area, const float zNear, const float zFar);
		static mat4 Perspective(const float fov, const float aspect, const float zNear, const float zFar);
		static mat3 RebaseMatrix(const mat3 & m3, const mat4 & m4);
		static mat4 Rotate(const mat4 & value, const float angle, const vec3 & axis);
		static mat4 Rotate(const mat4 & value, const quat & rot);
		static vec3 RebasePoint(const vec3 & value, const mat4 & t);
		static mat4 Scale(const mat4 & value, const vec3 & scl);
		static mat4 Translate(const mat4 & value, const vec3 & tns);
		static mat4 Transpose(const mat4 & value);


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform &	rotate(const float angle, const vec3 & axis);
		Transform & rotate(const quat & value);
		Transform &	scale(const vec3 & value);
		Transform &	translate(const vec3 & value);
		Transform & lookAt(const vec3 & eye, const vec3 & pos, const vec3 & up);
		Transform & lookAt(const vec3 & eye, const vec3 & pos);

		/* * * * * * * * * * * * * * * * * * * * */

		Transform & update(const Transform & other);// Set Matrix
		Transform & update(const mat4 & value);	// Set Matrix

		/* * * * * * * * * * * * * * * * * * * * */

		bool decompose(vec3 & scl, quat & rot, vec3 & tns, vec3 & skw, vec4 & psp) const;
		const Transform & decompose() const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline operator const mat4 &() const { return getMat(); }

		const mat4 &	getMat() const;		// Get Matrix
		const mat4		getInv() const;		// Get Inverse
		const vec4 &	getPsp() const;		// Get Perspective
		const vec3 &	getPos() const;		// Get Position
		const quat &	getRot() const;		// Get Rotation
		const vec3 &	getScl() const;		// Get Scale
		const vec3 &	getSkw() const;		// Get Skew


	private: // Data
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		mat4 m_matrix;

		mutable bool	m_changed	= true;
		mutable vec4	m_psp		= 0.0f;	// Perspective
		mutable vec3	m_pos		= 0.0f;	// Position
		mutable quat	m_rot		= 1.0f;	// Rotation
		mutable vec3	m_scl		= 0.0f;	// Scale
		mutable vec3	m_skw		= 0.0f;	// Skew
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_