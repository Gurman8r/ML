#ifndef _ML_UNI_HPP_
#define _ML_UNI_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Texture;

	/* * * * * * * * * * * * * * * * * * * * */


	// Base Uni
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct uni_base : public ITrackable, public INonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Types : int32_t
		{
			Flt,	// Float
			Int,	// Integer
			Vec2,	// Vector2
			Vec3,	// Vector3
			Vec4,	// Vector4
			Col4,	// Color
			Mat3,	// Matrix3x3
			Mat4,	// Matrix4x4
			Tex,	// Texture
			MAX_UNI_TYPES
		};

		using id_type = typename const int32_t;

		/* * * * * * * * * * * * * * * * * * * * */

		String	name;
		id_type type;

		/* * * * * * * * * * * * * * * * * * * * */

		uni_base(const String & name, id_type type) : name(name), type(type) {}

		virtual ~uni_base() {}

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend bool operator==(const uni_base & a, const uni_base & b)
		{
			return ((a.type == b.type) && (a.name == b.name));
		}

		inline friend bool operator<(const uni_base & a, const uni_base & b)
		{
			return ((a.type < b.type) || (a.name < b.name));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};


	// Base Generic Uni
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <
		class	_Elem,
		int32_t _Type
	> struct uni_t : public uni_base
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename _Elem;
		using self_type		= typename uni_t<value_type, _Type>;
		using base_type		= typename uni_base;
		using id_type		= typename base_type::id_type;

		/* * * * * * * * * * * * * * * * * * * * */

		enum : id_type { ID = _Type };

		/* * * * * * * * * * * * * * * * * * * * */

		value_type data;

		/* * * * * * * * * * * * * * * * * * * * */

		uni_t(const String & name, value_type data) : base_type(name, ID), data(data) {}
		
		virtual ~uni_t() {}

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend bool operator==(const self_type & a, const self_type & b)
		{
			return ((a.name == b.name) && (a.data == b.data));
		}

		inline friend bool operator<(const self_type & a, const self_type & b)
		{
			return ((a.name < b.name) || (a.data < b.data));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};


	// Generators
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_UNIFORM(NAME, TYPE)											\
	template <class T> struct NAME : public uni_t<T, TYPE>					\
	{																		\
		using value_type	= typename T;									\
		using self_type		= typename NAME<value_type>;					\
		using base_type		= typename uni_t<T, TYPE>;						\
		NAME(const String & name, T data) : base_type(name, data) {}		\
		virtual ~NAME() {}													\
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_GEN_UNIFORM(	uni_flt_t,	uni_base::Flt	);
	ML_GEN_UNIFORM(	uni_int_t,	uni_base::Int	);
	ML_GEN_UNIFORM(	uni_vec2_t,	uni_base::Vec2	);
	ML_GEN_UNIFORM(	uni_vec3_t,	uni_base::Vec3	);
	ML_GEN_UNIFORM(	uni_vec4_t,	uni_base::Vec4	);
	ML_GEN_UNIFORM(	uni_col4_t,	uni_base::Col4	);
	ML_GEN_UNIFORM(	uni_mat3_t,	uni_base::Mat3	);
	ML_GEN_UNIFORM(	uni_mat4_t,	uni_base::Mat4	);
	ML_GEN_UNIFORM(	uni_tex_t,	uni_base::Tex	);


	// Value Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt		= typename uni_flt_t	<float	>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2	>;
	using uni_vec3		= typename uni_vec3_t	<vec3	>;
	using uni_vec4		= typename uni_vec4_t	<vec4	>;
	using uni_col4		= typename uni_col4_t	<vec4	>;
	using uni_mat3		= typename uni_mat3_t	<mat3	>;
	using uni_mat4		= typename uni_mat4_t	<mat4	>;


	// Const Reference Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_cr	= typename uni_flt_t	<const float	&>;
	using uni_int_cr	= typename uni_int_t	<const int32_t	&>;
	using uni_vec2_cr	= typename uni_vec2_t	<const vec2		&>;
	using uni_vec3_cr	= typename uni_vec3_t	<const vec3		&>;
	using uni_vec4_cr	= typename uni_vec3_t	<const vec4		&>;
	using uni_col4_cr	= typename uni_col4_t	<const vec4		&>;
	using uni_mat3_cr	= typename uni_mat3_t	<const mat3		&>;
	using uni_mat4_cr	= typename uni_mat4_t	<const mat4		&>;


	// Const Pointer Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_cp	= typename uni_flt_t	<const float	*>;
	using uni_int_cp	= typename uni_int_t	<const int32_t	*>;
	using uni_vec2_cp	= typename uni_vec2_t	<const vec2		*>;
	using uni_vec3_cp	= typename uni_vec3_t	<const vec3		*>;
	using uni_vec4_cp	= typename uni_vec3_t	<const vec4		*>;
	using uni_col4_cp	= typename uni_col4_t	<const vec4		*>;
	using uni_mat3_cp	= typename uni_mat3_t	<const mat3		*>;
	using uni_mat4_cp	= typename uni_mat4_t	<const mat4		*>;
	using uni_tex_cp	= typename uni_tex_t	<const Texture	*>; // <- Texture

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNI_HPP_