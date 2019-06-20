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
	struct uni_base : public INewable, public INonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Types : int32_t
		{
			Flt1, Int1,
			Vec2, Vec3, Vec4, Col4,
			Mat3, Mat4,
			Tex2,
			MAX_UNI_TYPES
		};

		static constexpr CString TypeNames[] = {
			"float", "int",
			"vec2", "vec3", "vec4", "vec4",
			"mat3", "mat4",
			"sampler2D",
		};

		using id_type = typename const int32_t;

		/* * * * * * * * * * * * * * * * * * * * */

		String	name;
		id_type type;

		/* * * * * * * * * * * * * * * * * * * * */

		uni_base(const String & name, id_type type) 
			: name(name)
			, type(type) 
		{}

		/* * * * * * * * * * * * * * * * * * * * */

		inline void serialize(ostream & out) const override
		{
			out << TypeNames[this->type];
		}

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

		template <
			class Derived
		> inline const Derived * as() const
		{
			return dynamic_cast<const Derived *>(this);
		}

		template <
			class Derived
		> inline Derived * as()
		{
			return dynamic_cast<Derived *>(this);
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

		uni_t(const String & name, value_type data) 
			: base_type(name, ID)
			, data(data) 
		{}
		
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

#define ML_GEN_UNIFORM(NAME, TYPE)							\
	template <class T> struct NAME : public uni_t<T, TYPE>	\
	{														\
		using value_type	= typename T;					\
		using self_type		= typename NAME<value_type>;	\
		using base_type		= typename uni_t<T, TYPE>;		\
		NAME(const String & name, T data)					\
			: base_type(name, data)							\
		{}													\
		virtual ~NAME() {}									\
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_GEN_UNIFORM(	uni_flt_t,	uni_base::Flt1	);
	ML_GEN_UNIFORM(	uni_int_t,	uni_base::Int1	);
	ML_GEN_UNIFORM(	uni_vec2_t,	uni_base::Vec2	);
	ML_GEN_UNIFORM(	uni_vec3_t,	uni_base::Vec3	);
	ML_GEN_UNIFORM(	uni_vec4_t,	uni_base::Vec4	);
	ML_GEN_UNIFORM(	uni_col4_t,	uni_base::Col4	);
	ML_GEN_UNIFORM(	uni_mat3_t,	uni_base::Mat3	);
	ML_GEN_UNIFORM(	uni_mat4_t,	uni_base::Mat4	);
	ML_GEN_UNIFORM(	uni_tex_t,	uni_base::Tex2	);


	// Value Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt		= typename uni_flt_t	<float	>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2	>;
	using uni_vec3		= typename uni_vec3_t	<vec3	>;
	using uni_vec4		= typename uni_vec4_t	<vec4	>;
	using uni_col4		= typename uni_col4_t	<vec4	>;
	using uni_mat3		= typename uni_mat3_t	<mat3	>;
	using uni_mat4		= typename uni_mat4_t	<mat4	>;


	// Const Reference Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_cr	= typename uni_flt_t	<const float	&>;
	using uni_int_cr	= typename uni_int_t	<const int32_t	&>;
	using uni_vec2_cr	= typename uni_vec2_t	<const vec2		&>;
	using uni_vec3_cr	= typename uni_vec3_t	<const vec3		&>;
	using uni_vec4_cr	= typename uni_vec3_t	<const vec4		&>;
	using uni_col4_cr	= typename uni_col4_t	<const vec4		&>;
	using uni_mat3_cr	= typename uni_mat3_t	<const mat3		&>;
	using uni_mat4_cr	= typename uni_mat4_t	<const mat4		&>;


	// Const Pointer Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_cp	= typename uni_flt_t	<const float	*>;
	using uni_int_cp	= typename uni_int_t	<const int32_t	*>;
	using uni_vec2_cp	= typename uni_vec2_t	<const vec2		*>;
	using uni_vec3_cp	= typename uni_vec3_t	<const vec3		*>;
	using uni_vec4_cp	= typename uni_vec3_t	<const vec4		*>;
	using uni_col4_cp	= typename uni_col4_t	<const vec4		*>;
	using uni_mat3_cp	= typename uni_mat3_t	<const mat3		*>;
	using uni_mat4_cp	= typename uni_mat4_t	<const mat4		*>;


	// Type-Class Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_tex2 = typename uni_tex_t<const Texture *>; // <- Texture

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		inline static float * toFloat(const uni_base * value)
		{
			if (!value) return nullptr;
			static float temp;
			if (auto u = value->as<uni_flt>())			return &(temp = u->data);
			else if (auto u = value->as<uni_flt_cr>())	return &(temp = u->data);
			else if (auto u = value->as<uni_flt_cp>())	return &(temp = *u->data);
			else return nullptr;
		}

		inline static int32_t * toInt(const uni_base * value)
		{
			if (!value) return nullptr;
			static int32_t temp;
			if (auto u = value->as<uni_int>())			return &(temp = u->data);
			else if (auto u = value->as<uni_int_cr>())	return &(temp = u->data);
			else if (auto u = value->as<uni_int_cp>())	return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec2 * toVec2(const uni_base * value)
		{
			if (!value) return nullptr;
			static vec2 temp;
			if (auto u = value->as<uni_vec2>())			return &(temp = u->data);
			else if (auto u = value->as<uni_vec2_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_vec2_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec3 * toVec3(const uni_base * value)
		{
			if (!value) return nullptr;
			static vec3 temp;
			if (auto u = value->as<uni_vec3>())			return &(temp = u->data);
			else if (auto u = value->as<uni_vec3_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_vec3_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec4 * toVec4(const uni_base * value)
		{
			if (!value) return nullptr;
			static vec4 temp;
			if (auto u = value->as<uni_vec4>())			return &(temp = u->data);
			else if (auto u = value->as<uni_vec4_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_vec4_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec4 * toCol4(const uni_base * value)
		{
			if (!value) return nullptr;
			static vec4 temp;
			if (auto u = value->as<uni_col4>())			return &(temp = u->data);
			else if (auto u = value->as<uni_col4_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_col4_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static mat3 * toMat3(const uni_base * value)
		{
			if (!value) return nullptr;
			static mat3 temp;
			if (auto u = value->as<uni_mat3>())			return &(temp = u->data);
			else if (auto u = value->as<uni_mat3_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_mat3_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static mat4 * toMat4(const uni_base * value)
		{
			if (!value) return nullptr;
			static mat4 temp;
			if (auto u = value->as<uni_mat4>())			return &(temp = u->data);
			else if (auto u = value->as<uni_mat4_cr>()) return &(temp = u->data);
			else if (auto u = value->as<uni_mat4_cp>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static const Texture * toTex2(const uni_base * value)
		{
			if (!value) return nullptr;
			if (auto u = value->as<uni_tex2>()) return u->data;
			else return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNI_HPP_