#ifndef _ML_UNI_HPP_
#define _ML_UNI_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct CubeMap;
	struct Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	// Base Uniform
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Uniform 
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Types : int32_t
		{
			Flt1, Int1,
			Vec2, Vec3, Vec4, Col4,
			Mat3, Mat4,
			Tex2, Cube,
			MAX_UNI_TYPES
		};

		static constexpr C_String TypeNames[] = {
			"float", "int",
			"vec2", "vec3", "vec4", "vec4",
			"mat3", "mat4",
			"sampler2D", "samplerCube (NYI)"
		};

		/* * * * * * * * * * * * * * * * * * * * */

		const String	name;
		const int32_t	type;

		/* * * * * * * * * * * * * * * * * * * * */

		Uniform(const String & name, const int32_t type)
			: name(name)
			, type(type) 
		{}

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(Ostream & out, const Uniform & value)
		{
			return out << TypeNames[value.type];
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend bool operator==(const Uniform & a, const Uniform & b)
		{
			return ((a.type == b.type) && (a.name == b.name));
		}

		inline friend bool operator<(const Uniform & a, const Uniform & b)
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

	// Generic Uniform
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <
		class	_Elem,
		int32_t _Type
	> struct uni_t : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename _Elem;
		using self_type		= typename uni_t<value_type, _Type>;
		using base_type		= typename Uniform;

		/* * * * * * * * * * * * * * * * * * * * */

		enum : int32_t { ID = _Type };

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

	ML_GEN_UNIFORM(	uni_flt_t,	Uniform::Flt1	);
	ML_GEN_UNIFORM(	uni_int_t,	Uniform::Int1	);
	ML_GEN_UNIFORM(	uni_vec2_t,	Uniform::Vec2	);
	ML_GEN_UNIFORM(	uni_vec3_t,	Uniform::Vec3	);
	ML_GEN_UNIFORM(	uni_vec4_t,	Uniform::Vec4	);
	ML_GEN_UNIFORM(	uni_col4_t,	Uniform::Col4	);
	ML_GEN_UNIFORM(	uni_mat3_t,	Uniform::Mat3	);
	ML_GEN_UNIFORM(	uni_mat4_t,	Uniform::Mat4	);
	ML_GEN_UNIFORM(	uni_tex2_t,	Uniform::Tex2	);
	ML_GEN_UNIFORM( uni_cube_t,	Uniform::Cube	);


	// Value Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt		= typename uni_flt_t	<float_t>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2	>;
	using uni_vec3		= typename uni_vec3_t	<vec3	>;
	using uni_vec4		= typename uni_vec4_t	<vec4	>;
	using uni_col4		= typename uni_col4_t	<vec4	>;
	using uni_mat3		= typename uni_mat3_t	<mat3	>;
	using uni_mat4		= typename uni_mat4_t	<mat4	>;


	// Const Reference Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_ref	= typename uni_flt_t	<const float_t	&>;
	using uni_int_ref	= typename uni_int_t	<const int32_t	&>;
	using uni_vec2_ref	= typename uni_vec2_t	<const vec2		&>;
	using uni_vec3_ref	= typename uni_vec3_t	<const vec3		&>;
	using uni_vec4_ref	= typename uni_vec3_t	<const vec4		&>;
	using uni_col4_ref	= typename uni_col4_t	<const vec4		&>;
	using uni_mat3_ref	= typename uni_mat3_t	<const mat3		&>;
	using uni_mat4_ref	= typename uni_mat4_t	<const mat4		&>;


	// Const Pointer Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_flt_ptr	= typename uni_flt_t	<const float_t	*>;
	using uni_int_ptr	= typename uni_int_t	<const int32_t	*>;
	using uni_vec2_ptr	= typename uni_vec2_t	<const vec2		*>;
	using uni_vec3_ptr	= typename uni_vec3_t	<const vec3		*>;
	using uni_vec4_ptr	= typename uni_vec3_t	<const vec4		*>;
	using uni_col4_ptr	= typename uni_col4_t	<const vec4		*>;
	using uni_mat3_ptr	= typename uni_mat3_t	<const mat3		*>;
	using uni_mat4_ptr	= typename uni_mat4_t	<const mat4		*>;


	// Type-Class Uniforms
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_cube		= typename uni_cube_t	<const CubeMap	*>; // <- CubeMap (NYI)
	using uni_tex2		= typename uni_tex2_t	<const Texture	*>; // <- Texture

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		inline static float_t * toFloat(const Uniform * value)
		{
			static float_t temp;
			if (!value || value->type != uni_flt::ID)	return nullptr;
			else if (auto u = value->as<uni_flt>())		return &(temp =  u->data);
			else if (auto u = value->as<uni_flt_ref>())	return &(temp =  u->data);
			else if (auto u = value->as<uni_flt_ptr>())	return &(temp = *u->data);
			else return nullptr;
		}

		inline static int32_t * toInt(const Uniform * value)
		{
			static int32_t temp;
			if (!value || value->type != uni_int::ID)	return nullptr;
			else if (auto u = value->as<uni_int>())		return &(temp =  u->data);
			else if (auto u = value->as<uni_int_ref>())	return &(temp =  u->data);
			else if (auto u = value->as<uni_int_ptr>())	return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec2 * toVec2(const Uniform * value)
		{
			static vec2 temp;
			if (!value || value->type != uni_vec2::ID)	 return nullptr;
			else if (auto u = value->as<uni_vec2>())	 return &(temp =  u->data);
			else if (auto u = value->as<uni_vec2_ref>()) return &(temp =  u->data);
			else if (auto u = value->as<uni_vec2_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec3 * toVec3(const Uniform * value)
		{
			static vec3 temp;
			if (!value || value->type != uni_vec3::ID)	 return nullptr;
			else if (auto u = value->as<uni_vec3>())	 return &(temp = u->data);
			else if (auto u = value->as<uni_vec3_ref>()) return &(temp = u->data);
			else if (auto u = value->as<uni_vec3_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec4 * toVec4(const Uniform * value)
		{
			static vec4 temp;
			if (!value || value->type != uni_vec4::ID)	 return nullptr;
			else if (auto u = value->as<uni_vec4>())	 return &(temp =  u->data);
			else if (auto u = value->as<uni_vec4_ref>()) return &(temp =  u->data);
			else if (auto u = value->as<uni_vec4_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static vec4 * toCol4(const Uniform * value)
		{
			static vec4 temp;
			if (!value || value->type != uni_col4::ID)	 return nullptr;
			else if (auto u = value->as<uni_col4>())	 return &(temp =  u->data);
			else if (auto u = value->as<uni_col4_ref>()) return &(temp =  u->data);
			else if (auto u = value->as<uni_col4_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static mat3 * toMat3(const Uniform * value)
		{
			static mat3 temp;
			if (!value || value->type != uni_mat3::ID)	 return nullptr;
			else if (auto u = value->as<uni_mat3>())	 return &(temp =  u->data);
			else if (auto u = value->as<uni_mat3_ref>()) return &(temp =  u->data);
			else if (auto u = value->as<uni_mat3_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static mat4 * toMat4(const Uniform * value)
		{
			static mat4 temp;
			if (!value || value->type != uni_mat4::ID)	 return nullptr;
			else if (auto u = value->as<uni_mat4>())	 return &(temp =  u->data);
			else if (auto u = value->as<uni_mat4_ref>()) return &(temp =  u->data);
			else if (auto u = value->as<uni_mat4_ptr>()) return &(temp = *u->data);
			else return nullptr;
		}

		inline static const Texture * toTex2(const Uniform * value)
		{
			if (!value || value->type != uni_tex2::ID)	 return nullptr;
			else if (auto u = value->as<uni_tex2>())	 return u->data;
			else return nullptr;
		}

		inline static const CubeMap * toCube(const Uniform * value)
		{
			if (!value || value->type != uni_cube::ID)	 return nullptr;
			else if (auto u = value->as<uni_cube>())	 return u->data;
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNI_HPP_