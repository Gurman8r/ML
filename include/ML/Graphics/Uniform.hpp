#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/I_Newable.hpp>

struct ml::Texture;

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Base Uniform
	struct Uni : public I_Newable, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Type : uint32_t
		{
			U_Bool, U_Float, U_Int,
			U_Vec2, U_Vec3, U_Vec4, U_Color,
			U_Mat3, U_Mat4,
			U_Sampler
		};

		static constexpr Type Type_values[] = {
			U_Bool, U_Float, U_Int,
			U_Vec2, U_Vec3, U_Vec4, U_Color,
			U_Mat3, U_Mat4,
			U_Sampler
		};

		static constexpr C_String Type_names[] = {
			"bool", "float", "int",
			"vec2", "vec3", "vec4", "color",
			"mat3", "mat4",
			"sampler"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using id_type = const Type;

		String	name;
		id_type id;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uni(const String & name, uint32_t id)
			: name { name }, id { static_cast<id_type>(id) }
		{
		}

		virtual ~Uni() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Uni * clone() const = 0;

		virtual bool isValue() const = 0;
		virtual bool isPointer() const = 0;
		virtual bool isReference() const = 0;
		virtual bool isConstPointer() const = 0;
		virtual bool isConstReference() const = 0;

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

		inline friend ML_SERIALIZE(Ostream & out, const Uni & value)
		{
			return out << Type_names[value.id];
		}

		inline friend bool operator==(const Uni & lhs, const Uni & rhs)
		{
			return ((lhs.id == rhs.id) && (lhs.name == rhs.name));
		}

		inline friend bool operator<(const Uni & lhs, const Uni & rhs)
		{
			return ((lhs.id < rhs.id) || (lhs.name < rhs.name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// Generic Uniform
	template <class T, uint32_t ID> struct uni_t final : public Uni
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type				= typename T;
		using self_type			= typename uni_t<type, ID>;
		using value_type		= typename detail::decay_t<type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		static constexpr bool is_pointer			{ std::is_pointer_v<type> };
		static constexpr bool is_reference			{ std::is_reference_v<type> };
		static constexpr bool is_value				{ !is_pointer && !is_reference };
		static constexpr bool is_const_pointer		{ std::is_same_v<type, const_pointer> };
		static constexpr bool is_const_reference	{ std::is_same_v<type, const_reference> };

		static_assert(
			is_value || is_const_pointer || is_const_reference,
			"an unacceptable uniform value type has been specified"
		);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { ID = ID };

		type data;

		explicit uni_t(const String & name, type data)
			: Uni { name, ID }, data { data }
		{
		}

		inline self_type * clone() const override
		{ 
			return new self_type { name, data }; 
		}

		inline bool isValue()			const override { return this->is_value; }
		inline bool isPointer()			const override { return this->is_pointer; }
		inline bool isReference()		const override { return this->is_reference; }
		inline bool isConstPointer()	const override { return this->is_const_pointer; }
		inline bool isConstReference()	const override { return this->is_const_reference; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> using uni_bool_t		= uni_t<T, Uni::U_Bool>;
	template <class T> using uni_float_t	= uni_t<T, Uni::U_Float>;
	template <class T> using uni_int_t		= uni_t<T, Uni::U_Int>;
	template <class T> using uni_vec2_t		= uni_t<T, Uni::U_Vec2>;
	template <class T> using uni_vec3_t		= uni_t<T, Uni::U_Vec3>;
	template <class T> using uni_vec4_t		= uni_t<T, Uni::U_Vec4>;
	template <class T> using uni_color_t	= uni_t<T, Uni::U_Color>;
	template <class T> using uni_mat3_t		= uni_t<T, Uni::U_Mat3>;
	template <class T> using uni_mat4_t		= uni_t<T, Uni::U_Mat4>;
	template <class T> using uni_sampler_t	= uni_t<T, Uni::U_Sampler>;

	using uni_bool		= typename uni_bool_t	<bool>;
	using uni_float		= typename uni_float_t	<float_t>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2>;
	using uni_vec3		= typename uni_vec3_t	<vec3>;
	using uni_vec4		= typename uni_vec4_t	<vec4>;
	using uni_color		= typename uni_color_t	<vec4>;
	using uni_mat3		= typename uni_mat3_t	<mat3>;
	using uni_mat4		= typename uni_mat4_t	<mat4>;
	using uni_sampler	= typename uni_sampler_t<const Texture *>; // All Texture Types

	using uni_bool_ref	= typename uni_bool_t	<const bool &>;
	using uni_float_ref = typename uni_float_t	<const float_t &>;
	using uni_int_ref	= typename uni_int_t	<const int32_t &>;
	using uni_vec2_ref	= typename uni_vec2_t	<const vec2 &>;
	using uni_vec3_ref	= typename uni_vec3_t	<const vec3 &>;
	using uni_vec4_ref	= typename uni_vec4_t	<const vec4 &>;
	using uni_color_ref = typename uni_color_t	<const vec4 &>;
	using uni_mat3_ref	= typename uni_mat3_t	<const mat3 &>;
	using uni_mat4_ref	= typename uni_mat4_t	<const mat4 &>;
	
	using uni_bool_ptr	= typename uni_bool_t	<const bool *>;
	using uni_float_ptr = typename uni_float_t	<const float_t *>;
	using uni_int_ptr	= typename uni_int_t	<const int32_t *>;
	using uni_vec2_ptr	= typename uni_vec2_t	<const vec2 *>;
	using uni_vec3_ptr	= typename uni_vec3_t	<const vec3 *>;
	using uni_vec4_ptr	= typename uni_vec4_t	<const vec4 *>;
	using uni_color_ptr = typename uni_color_t	<const vec4 *>;
	using uni_mat3_ptr	= typename uni_mat3_t	<const mat3 *>;
	using uni_mat4_ptr	= typename uni_mat4_t	<const mat4 *>;

	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool valueAt(const int32_t i, Uni::Type & value)
		{
			return alg::value_at(i, value, Uni::Type_values);
		}

		static constexpr int32_t indexOf(const Uni::Type value)
		{
			return alg::index_of(value, Uni::Type_values);
		}

		static constexpr C_String nameOf(const Uni::Type value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Uni::Type_names[i] : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool * as_bool(const Uni * value)
		{
			static bool temp;
			if (!value || (value->id != uni_bool::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_bool>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_bool_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_bool_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline float_t * as_float(const Uni * value)
		{
			static float_t temp;
			if (!value || (value->id != uni_float::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_float>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_float_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_float_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline int32_t * as_int(const Uni * value)
		{
			static int32_t temp;
			if (!value || (value->id != uni_int::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_int>())			{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int_ref>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int_ptr>())		{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec2 * as_vec2(const Uni * value)
		{
			static vec2 temp;
			if (!value || value->id != uni_vec2::ID)		{ return nullptr; }
			else if (auto u = value->as<uni_vec2>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec3 * as_vec3(const Uni * value)
		{
			static vec3 temp;
			if (!value || (value->id != uni_vec3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec3_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * as_vec4(const Uni * value)
		{
			static vec4 temp;
			if (!value || (value->id != uni_vec4::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * as_color(const Uni * value)
		{
			static vec4 temp;
			if (!value || (value->id != uni_color::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_color>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_color_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_color_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat3 * as_mat3(const Uni * value)
		{
			static mat3 temp;
			if (!value || (value->id != uni_mat3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat4 * as_mat4(const Uni * value)
		{
			static mat4 temp;
			if (!value || (value->id != uni_mat4::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat4_ref>())	{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline const Texture * as_sampler(const Uni * value)
		{
			if (!value || (value->id != uni_sampler::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_sampler>())		{ return u->data; }
			else { return nullptr; }
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_