#ifndef _ML_GLM_HPP_
#define _ML_GLM_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/matrix_decompose.hpp>
#include <GLM/gtx/projection.hpp>
#include <GLM/gtx/quaternion.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Quaternion.hpp>

#define ML_GLM GLM_VERSION

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> inline glm::tvec2<T, glm::defaultp> to_glm(const tvec2<T> & v)
	{
		return { v[0], v[1] };
	}

	template <
		class T
	> inline glm::tvec3<T, glm::defaultp> to_glm(const tvec3<T> & v)
	{
		return { v[0], v[1], v[2] };
	}

	template <
		class T
	> inline glm::tvec4<T, glm::defaultp> to_glm(const tvec4<T> & v)
	{
		return { v[0], v[1], v[2], v[3] };
	}

	template <
		class T
	> inline glm::tquat<T, glm::defaultp> to_glm(const tquat<T> & v)
	{
		return { v[0], v[1], v[2], v[3] };
	}

	template <
		class T
	> inline glm::tmat3x3<T, glm::defaultp> to_glm(const tmat3<T> & v)
	{
		return { 
			v[0], v[1], v[2],
			v[3], v[4], v[5],
			v[6], v[7], v[8]
		};
	}

	template <
		class T
	> inline glm::tmat4x4<T, glm::defaultp> to_glm(const tmat4<T> & v)
	{
		return {
			v[ 0], v[ 1], v[ 2], v[ 3],
			v[ 4], v[ 5], v[ 6], v[ 7], 
			v[ 8], v[ 9], v[10], v[11],
			v[12], v[13], v[14], v[15]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> inline tvec2<T> from_glm(const glm::tvec2<T, glm::defaultp> & v)
	{
		return { v[0], v[1] };
	}

	template <
		class T
	> inline tvec3<T> from_glm(const glm::tvec3<T, glm::defaultp> & v)
	{
		return { v[0], v[1], v[2] };
	}

	template <
		class T
	> inline tvec4<T> from_glm(const glm::tvec4<T, glm::defaultp> & v)
	{
		return { v[0], v[1], v[2], v[3] };
	}

	template <
		class T
	> inline tquat<T> from_glm(const glm::tquat<T, glm::defaultp> & v)
	{
		return { v[0], v[1], v[2], v[3] };
	}

	template <
		class T
	> inline tmat3<T> from_glm(const glm::tmat3x3<T, glm::defaultp> & v)
	{
		return 
		{
			v[0], v[1], v[2],
			v[3], v[4], v[5],
			v[6], v[7], v[8]
		};
	}

	template <
		class T
	> inline tmat4<T> from_glm(const glm::tmat4x4<T, glm::defaultp> & v)
	{
		return {
			v[ 0], v[ 1], v[ 2], v[ 3],
			v[ 4], v[ 5], v[ 6], v[ 7], 
			v[ 8], v[ 9], v[10], v[11],
			v[12], v[13], v[14], v[15]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_GLM_HPP_