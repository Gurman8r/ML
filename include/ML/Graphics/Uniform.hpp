#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/BitMask.hpp>

// this entire system needs to be reworked

struct ml::Texture;

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base Uniform
	struct Uniform : public Newable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Type : uint32_t {
			Boolean, Float, Integer,
			Vector2, Vector3, Vector4, Color,
			Matrix2, Matrix3, Matrix4,
			Sampler
		};

		static constexpr Type Type_values[] = {
			Boolean, Float, Integer,
			Vector2, Vector3, Vector4, Color,
			Matrix2, Matrix3, Matrix4,
			Sampler
		};

		static constexpr C_String Type_names[] = {
			"bool", "float", "int",
			"vec2", "vec3", "vec4", "color",
			"mat2", "mat3", "mat4",
			"sampler"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const uint32_t id;
		String name;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name, uint32_t id)
			: name { name }, id { id }
		{
		}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Uniform * clone() const = 0;
		virtual bool isModifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * as()
		{
			return dynamic_cast<T *>(this);
		}

		template <class T> inline const T * as() const
		{
			return dynamic_cast<const T *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const Uniform & value)
		{
			return out << Type_names[value.id];
		}

		inline friend bool operator<(const Uniform & lhs, const Uniform & rhs)
		{
			return ((lhs.id < rhs.id) || (lhs.name < rhs.name));
		}

		inline friend bool operator==(const Uniform & lhs, const Uniform & rhs)
		{
			return !(lhs < rhs) && !(rhs < lhs);
		}

		inline friend bool operator!=(const Uniform & lhs, const Uniform & rhs)
		{
			return !(lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Generic Uniform Interface
	template <class T, uint32_t ID> struct I_Uniform final : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename I_Uniform<T, ID>;

		enum : int32_t { ID = ID };

		T data;

		explicit I_Uniform(const String & name, T data)
			: Uniform { name, ID }, data { data }
		{
		}

		inline self_type * clone() const override
		{ 
			return new self_type { name, data };
		}

		inline bool isModifiable() const override 
		{ 
			return // holds a value or a sampler
				std::is_same_v<T, detail::decay_t<T>> ||
				std::is_same_v<T, const Texture *>;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> using uni_bool_t		= I_Uniform<T, Uniform::Boolean>;
	template <class T> using uni_float_t	= I_Uniform<T, Uniform::Float>;
	template <class T> using uni_int_t		= I_Uniform<T, Uniform::Integer>;
	template <class T> using uni_vec2_t		= I_Uniform<T, Uniform::Vector2>;
	template <class T> using uni_vec3_t		= I_Uniform<T, Uniform::Vector3>;
	template <class T> using uni_vec4_t		= I_Uniform<T, Uniform::Vector4>;
	template <class T> using uni_color_t	= I_Uniform<T, Uniform::Color>;
	template <class T> using uni_mat2_t		= I_Uniform<T, Uniform::Matrix2>;
	template <class T> using uni_mat3_t		= I_Uniform<T, Uniform::Matrix3>;
	template <class T> using uni_mat4_t		= I_Uniform<T, Uniform::Matrix4>;
	template <class T> using uni_sampler_t	= I_Uniform<T, Uniform::Sampler>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_bool		= typename uni_bool_t	<bool>;
	using uni_float		= typename uni_float_t	<float_t>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2>;
	using uni_vec3		= typename uni_vec3_t	<vec3>;
	using uni_vec4		= typename uni_vec4_t	<vec4>;
	using uni_color		= typename uni_color_t	<Color>;
	using uni_mat2		= typename uni_mat2_t	<mat2>;
	using uni_mat3		= typename uni_mat3_t	<mat3>;
	using uni_mat4		= typename uni_mat4_t	<mat4>;
	using uni_sampler	= typename uni_sampler_t<const Texture *>; // All Texture Types

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using uni_bool_ptr	= typename uni_bool_t	<const bool *>;
	using uni_float_ptr = typename uni_float_t	<const float_t *>;
	using uni_int_ptr	= typename uni_int_t	<const int32_t *>;
	using uni_vec2_ptr	= typename uni_vec2_t	<const vec2 *>;
	using uni_vec3_ptr	= typename uni_vec3_t	<const vec3 *>;
	using uni_vec4_ptr	= typename uni_vec4_t	<const vec4 *>;
	using uni_color_ptr = typename uni_color_t	<const Color *>;
	using uni_mat2_ptr	= typename uni_mat2_t	<const mat2 *>;
	using uni_mat3_ptr	= typename uni_mat3_t	<const mat3 *>;
	using uni_mat4_ptr	= typename uni_mat4_t	<const mat4 *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool value_at(int32_t i, Uniform::Type & value)
		{
			return alg::value_at(i, value, Uniform::Type_values);
		}

		static constexpr int32_t index_of(const Uniform::Type value)
		{
			return alg::index_of(value, Uniform::Type_values);
		}

		static constexpr C_String name_of(const Uniform::Type value)
		{
			const int32_t i = index_of(value);
			return (i >= 0) ? Uniform::Type_names[i] : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool * as_bool(const Uniform * value)
		{
			static bool temp;
			if (!value || (value->id != uni_bool::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_bool>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_bool_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline float_t * as_float(const Uniform * value)
		{
			static float_t temp;
			if (!value || (value->id != uni_float::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_float>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_float_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline int32_t * as_int(const Uniform * value)
		{
			static int32_t temp;
			if (!value || (value->id != uni_int::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_int>())			{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int_ptr>())		{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec2 * as_vec2(const Uniform * value)
		{
			static vec2 temp;
			if (!value || value->id != uni_vec2::ID)		{ return nullptr; }
			else if (auto u = value->as<uni_vec2>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec3 * as_vec3(const Uniform * value)
		{
			static vec3 temp;
			if (!value || (value->id != uni_vec3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * as_vec4(const Uniform * value)
		{
			static vec4 temp;
			if (!value || (value->id != uni_vec4::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline Color * as_color(const Uniform * value)
		{
			static Color temp;
			if (!value || (value->id != uni_color::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_color>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_color_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat2 * as_mat2(const Uniform * value)
		{
			static mat2 temp;
			if (!value || (value->id != uni_mat2::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat2>())		{ return &(temp = u->data); }
			else if (auto u = value->as<uni_mat2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat3 * as_mat3(const Uniform * value)
		{
			static mat3 temp;
			if (!value || (value->id != uni_mat3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat4 * as_mat4(const Uniform * value)
		{
			static mat4 temp;
			if (!value || (value->id != uni_mat4::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline const Texture * as_sampler(const Uniform * value)
		{
			if (!value || (value->id != uni_sampler::ID))	{ return nullptr; }
			else if (auto u = value->as<uni_sampler>())		{ return u->data; }
			else { return nullptr; }
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_