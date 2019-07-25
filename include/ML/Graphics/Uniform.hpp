#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/I_Newable.hpp>

// Base Uniform
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct CubeMap;
	struct Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Types : int32_t
		{
			INVALID_UNI = -1,
			Bool, Flt1, Int1,
			Vec2, Vec3, Vec4, Col4,
			Mat3, Mat4,
			Tex2, Tex3, Cube,
			MAX_UNI_TYPES
		};

		static constexpr C_String TypeNames[] = {
			"bool", "float", "int",
			"vec2", "vec3", "vec4", "col4",
			"mat3", "mat4",
			"sampler2D", "sampler3D", "samplerCube"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const String	name;
		const int32_t	type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name, const int32_t type)
			: name(name)
			, type(type) 
		{
		}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class U
		> static inline T * duplicate(const U * value)
		{
			return ((value && (*value))
				? new T { 
					static_cast<const T *>(value)->name,
					static_cast<const T *>(value)->data }
				: static_cast<T *>(nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * as()
		{
			return dynamic_cast<T *>(this);
		}

		template <class T> inline const T * as() const
		{
			return dynamic_cast<const T *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(Ostream & out, const Uniform & value)
		{
			return out << TypeNames[value.type];
		}

		inline operator bool() const
		{
			return (type > Types::INVALID_UNI) && (type < Types::MAX_UNI_TYPES);
		}

		inline friend bool operator==(const Uniform & a, const Uniform & b)
		{
			return ((a.type == b.type) && (a.name == b.name));
		}

		inline friend bool operator<(const Uniform & a, const Uniform & b)
		{
			return ((a.type < b.type) || (a.name < b.name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

// Template Uniform
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <
		class Elem, int32_t TypeID
	> struct uni_t : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename Elem;
		using self_type		= typename uni_t<value_type, TypeID>;
		using base_type		= typename Uniform;

		enum : int32_t { ID = TypeID };

		value_type data;

		explicit uni_t(const String & name, value_type data)
			: base_type(name, ID)
			, data(data)
		{
		}

		virtual ~uni_t() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

// Generated Uniforms
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_UNIFORM(NAME, TYPE)								\
	template <class T> struct NAME : public _ML uni_t<T, TYPE>	\
	{															\
		using value_type	= typename T;						\
		using type			= typename _STD decay<T>::type;		\
		using self_type		= typename NAME<value_type>;		\
		using base_type		= typename _ML uni_t<T, TYPE>;		\
		explicit NAME(const _ML String & name, T data)			\
			: base_type(name, data)								\
		{														\
		}														\
		virtual ~NAME() {}										\
	};

	ML_GEN_UNIFORM(uni_bool_t, Uniform::Bool);
	ML_GEN_UNIFORM(uni_flt1_t, Uniform::Flt1);
	ML_GEN_UNIFORM(uni_int1_t, Uniform::Int1);
	ML_GEN_UNIFORM(uni_vec2_t, Uniform::Vec2);
	ML_GEN_UNIFORM(uni_vec3_t, Uniform::Vec3);
	ML_GEN_UNIFORM(uni_vec4_t, Uniform::Vec4);
	ML_GEN_UNIFORM(uni_col4_t, Uniform::Col4);
	ML_GEN_UNIFORM(uni_mat3_t, Uniform::Mat3);
	ML_GEN_UNIFORM(uni_mat4_t, Uniform::Mat4);
	ML_GEN_UNIFORM(uni_tex2_t, Uniform::Tex2);
	ML_GEN_UNIFORM(uni_tex3_t, Uniform::Tex3);
	ML_GEN_UNIFORM(uni_cube_t, Uniform::Cube);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using uni_bool		= typename uni_bool_t<bool>;
	using uni_flt1		= typename uni_flt1_t<float_t>;
	using uni_int1		= typename uni_int1_t<int32_t>;
	using uni_vec2		= typename uni_vec2_t<vec2>;
	using uni_vec3		= typename uni_vec3_t<vec3>;
	using uni_vec4		= typename uni_vec4_t<vec4>;
	using uni_col4		= typename uni_col4_t<vec4>;
	using uni_mat3		= typename uni_mat3_t<mat3>;
	using uni_mat4		= typename uni_mat4_t<mat4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using uni_bool_ref	= typename uni_bool_t<const bool &>;
	using uni_flt1_ref	= typename uni_flt1_t<const float_t	&>;
	using uni_int1_ref	= typename uni_int1_t<const int32_t	&>;
	using uni_vec2_ref	= typename uni_vec2_t<const vec2 &>;
	using uni_vec3_ref	= typename uni_vec3_t<const vec3 &>;
	using uni_vec4_ref	= typename uni_vec3_t<const vec4 &>;
	using uni_col4_ref	= typename uni_col4_t<const vec4 &>;
	using uni_mat3_ref	= typename uni_mat3_t<const mat3 &>;
	using uni_mat4_ref	= typename uni_mat4_t<const mat4 &>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using uni_bool_ptr	= typename uni_bool_t<const bool *>;
	using uni_flt1_ptr	= typename uni_flt1_t<const float_t	*>;
	using uni_int1_ptr	= typename uni_int1_t<const int32_t	*>;
	using uni_vec2_ptr	= typename uni_vec2_t<const vec2 *>;
	using uni_vec3_ptr	= typename uni_vec3_t<const vec3 *>;
	using uni_vec4_ptr	= typename uni_vec3_t<const vec4 *>;
	using uni_col4_ptr	= typename uni_col4_t<const vec4 *>;
	using uni_mat3_ptr	= typename uni_mat3_t<const mat3 *>;
	using uni_mat4_ptr	= typename uni_mat4_t<const mat4 *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using uni_tex2		= typename uni_tex2_t<const Texture	*>; // <- Texture 2D
	using uni_tex3		= typename uni_tex3_t<const Texture	*>; // <- Texture 3D (NYI)
	using uni_cube		= typename uni_cube_t<const Texture	*>; // <- Texture Cube Map (NYI)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		static inline bool * toBool(const Uniform * value)
		{
			static bool temp;
			if (!value || (value->type != uni_bool::ID)) { return nullptr; }
			else if (auto u = value->as<uni_bool>()) { return &(temp = u->data); }
			else if (auto u = value->as<uni_bool_ref>()) { return &(temp = u->data); }
			else if (auto u = value->as<uni_bool_ptr>()) { return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline float_t * toFloat(const Uniform * value)
		{
			static float_t temp;
			if (!value || (value->type != uni_flt1::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_flt1>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_flt1_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_flt1_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline int32_t * toInt(const Uniform * value)
		{
			static int32_t temp;
			if (!value || (value->type != uni_int1::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_int1>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int1_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int1_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec2 * toVec2(const Uniform * value)
		{
			static vec2 temp;
			if (!value || value->type != uni_vec2::ID)		{ return nullptr; }
			else if (auto u = value->as<uni_vec2>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec3 * toVec3(const Uniform * value)
		{
			static vec3 temp;
			if (!value || (value->type != uni_vec3::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_vec3>())		{ return &(temp = u->data); }
			else if (auto u = value->as<uni_vec3_ref>())	{ return &(temp = u->data); }
			else if (auto u = value->as<uni_vec3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * toVec4(const Uniform * value)
		{
			static vec4 temp;
			if (!value || (value->type != uni_vec4::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_vec4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * toCol4(const Uniform * value)
		{
			static vec4 temp;
			if (!value || (value->type != uni_col4::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_col4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_col4_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_col4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat3 * toMat3(const Uniform * value)
		{
			static mat3 temp;
			if (!value || (value->type != uni_mat3::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_mat3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat4 * toMat4(const Uniform * value)
		{
			static mat4 temp;
			if (!value || (value->type != uni_mat4::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_mat4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat4_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline const Texture * toTex2(const Uniform * value)
		{
			if (!value || (value->type != uni_tex2::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_tex2>())		{ return u->data; }
			else { return nullptr; }
		}

		static inline const Texture * toTex3(const Uniform * value)
		{
			if (!value || (value->type != uni_tex3::ID)) { return nullptr; }
			else if (auto u = value->as<uni_tex3>()) { return u->data; }
			else { return nullptr; }
		}

		static inline const Texture * toCube(const Uniform * value)
		{
			if (!value || (value->type != uni_cube::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_cube>())		{ return u->data; }
			{ return nullptr; }
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_