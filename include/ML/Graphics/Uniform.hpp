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

		enum : int32_t
		{
			Invalid = -1,
			Boolean, Float, Integer,
			Vector2, Vector3, Vector4, Color,
			Matrix2, Matrix3, Matrix4,
			Sampler,
			MAX_UNIFORM_TYPE
		};

		static constexpr int32_t Type_values[MAX_UNIFORM_TYPE] =
		{
			Boolean, Float, Integer,
			Vector2, Vector3, Vector4, Color,
			Matrix2, Matrix3, Matrix4,
			Sampler
		};

		static constexpr C_String Type_names[MAX_UNIFORM_TYPE] = 
		{
			"bool", "float", "int",
			"vec2", "vec3", "vec4", "color",
			"mat2", "mat3", "mat4",
			"sampler"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const int32_t id;
		String name;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name, int32_t id)
			: name { name }, id { id }
		{
		}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Uniform * clone() const = 0;
		virtual bool_t isModifiable() const = 0;

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

		static constexpr bool_t value_at(int32_t i, int32_t & value)
		{
			return alg::value_at(i, value, Uniform::Type_values);
		}

		static constexpr int32_t index_of(const int32_t value)
		{
			return alg::index_of(value, Uniform::Type_values);
		}

		static constexpr C_String name_of(const int32_t value)
		{
			const int32_t i = index_of(value);
			return (i >= 0) ? Uniform::Type_names[i] : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const Uniform & value)
		{
			return out << Type_names[value.id];
		}

		inline friend bool_t operator<(const Uniform & lhs, const Uniform & rhs)
		{
			return ((lhs.id < rhs.id) || (lhs.name < rhs.name));
		}

		inline friend bool_t operator==(const Uniform & lhs, const Uniform & rhs)
		{
			return !(lhs < rhs) && !(rhs < lhs);
		}

		inline friend bool_t operator!=(const Uniform & lhs, const Uniform & rhs)
		{
			return !(lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Generic Uniform Interface
	template <class T, int32_t _ID> struct UniformImpl final : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename UniformImpl<T, _ID>;

		static constexpr int32_t ID { _ID };

		T data;

		explicit UniformImpl(const String & name, T data)
			: Uniform { name, ID }, data { data }
		{
		}

		inline self_type * clone() const override
		{ 
			return new self_type { name, data };
		}

		inline bool_t isModifiable() const override 
		{ 
			return // holds a value or a sampler
				std::is_same_v<T, detail::decay_t<T>> ||
				std::is_same_v<T, const Texture *>;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class T> static constexpr int32_t uniform_id()
		{
			switch (typeof<detail::decay_t<T>>::hash)
			{
			case typeof<bool_t>	::hash: return Uniform::Boolean;
			case typeof<float_t>::hash: return Uniform::Float;
			case typeof<int32_t>::hash: return Uniform::Integer;
			case typeof<vec2>	::hash: return Uniform::Vector2;
			case typeof<vec3>	::hash: return Uniform::Vector3;
			case typeof<vec4>	::hash: return Uniform::Vector4;
			case typeof<Color>	::hash: return Uniform::Color;
			case typeof<mat2>	::hash: return Uniform::Matrix2;
			case typeof<mat3>	::hash: return Uniform::Matrix3;
			case typeof<mat4>	::hash: return Uniform::Matrix4;
			case typeof<Texture>::hash: return Uniform::Sampler;
			}
			return Uniform::Invalid;
		}

		template <class T> static constexpr int32_t uniform_id_v 
		{ 
			uniform_id<T>() 
		};

		template <class T> using uni_t = typename UniformImpl<T, uniform_id_v<T>>;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_bool		= typename impl::uni_t<bool_t>;
	using uni_float		= typename impl::uni_t<float_t>;
	using uni_int		= typename impl::uni_t<int32_t>;
	using uni_vec2		= typename impl::uni_t<vec2>;
	using uni_vec3		= typename impl::uni_t<vec3>;
	using uni_vec4		= typename impl::uni_t<vec4>;
	using uni_color		= typename impl::uni_t<Color>;
	using uni_mat2		= typename impl::uni_t<mat2>;
	using uni_mat3		= typename impl::uni_t<mat3>;
	using uni_mat4		= typename impl::uni_t<mat4>;
	using uni_sampler	= typename impl::uni_t<const Texture *>; // All Texture Types

	using uni_bool_ptr	= typename impl::uni_t<const bool_t *>;
	using uni_float_ptr = typename impl::uni_t<const float_t *>;
	using uni_int_ptr	= typename impl::uni_t<const int32_t *>;
	using uni_vec2_ptr	= typename impl::uni_t<const vec2 *>;
	using uni_vec3_ptr	= typename impl::uni_t<const vec3 *>;
	using uni_vec4_ptr	= typename impl::uni_t<const vec4 *>;
	using uni_color_ptr = typename impl::uni_t<const Color *>;
	using uni_mat2_ptr	= typename impl::uni_t<const mat2 *>;
	using uni_mat3_ptr	= typename impl::uni_t<const mat3 *>;
	using uni_mat4_ptr	= typename impl::uni_t<const mat4 *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool_t * as_bool(const Uniform * value)
		{
			static bool_t temp {};
			if (!value || (value->id != uni_bool::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_bool>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_bool_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline float_t * as_float(const Uniform * value)
		{
			static float_t temp {};
			if (!value || (value->id != uni_float::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_float>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_float_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline int32_t * as_int(const Uniform * value)
		{
			static int32_t temp {};
			if (!value || (value->id != uni_int::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_int>())			{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_int_ptr>())		{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec2 * as_vec2(const Uniform * value)
		{
			static vec2 temp {};
			if (!value || value->id != uni_vec2::ID)		{ return nullptr; }
			else if (auto u = value->as<uni_vec2>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec3 * as_vec3(const Uniform * value)
		{
			static vec3 temp {};
			if (!value || (value->id != uni_vec3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline vec4 * as_vec4(const Uniform * value)
		{
			static vec4 temp {};
			if (!value || (value->id != uni_vec4::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_vec4>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_vec4_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline Color * as_color(const Uniform * value)
		{
			static Color temp {};
			if (!value || (value->id != uni_color::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_color>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_color_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat2 * as_mat2(const Uniform * value)
		{
			static mat2 temp {};
			if (!value || (value->id != uni_mat2::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat2>())		{ return &(temp = u->data); }
			else if (auto u = value->as<uni_mat2_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat3 * as_mat3(const Uniform * value)
		{
			static mat3 temp {};
			if (!value || (value->id != uni_mat3::ID))		{ return nullptr; }
			else if (auto u = value->as<uni_mat3>())		{ return &(temp =  u->data); }
			else if (auto u = value->as<uni_mat3_ptr>())	{ return &(temp = *u->data); }
			else { return nullptr; }
		}

		static inline mat4 * as_mat4(const Uniform * value)
		{
			static mat4 temp {};
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