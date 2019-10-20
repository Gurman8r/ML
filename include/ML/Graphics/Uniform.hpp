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

		String name;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name) : name { name } {}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Uniform * clone() const = 0;
		
		virtual const int32_t & getID() const = 0;

		virtual const typeof<> & getType() const = 0;
		
		virtual bool isModifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * as() { return dynamic_cast<T *>(this); }

		template <
			class T
		> inline const T * as() const { return dynamic_cast<const T *>(this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool value_at(int32_t i, int32_t & value)
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
			return out << Uniform::name_of(value.getID());
		}

		inline friend bool operator<(const Uniform & lhs, const Uniform & rhs)
		{
			return ((lhs.getID() < rhs.getID()) || (lhs.name < rhs.name));
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

	namespace detail
	{
		template <class T> static constexpr int32_t uniform_id()
		{
			switch (typeof<detail::decay_t<T>>::hash)
			{
			case typeof<bool>	::hash:	return Uniform::Boolean;
			case typeof<float_t>::hash: return Uniform::Float;
			case typeof<int32_t>::hash: return Uniform::Integer;
			case typeof<vec2>	::hash:	return Uniform::Vector2;
			case typeof<vec3>	::hash:	return Uniform::Vector3;
			case typeof<vec4>	::hash:	return Uniform::Vector4;
			case typeof<Color>	::hash:	return Uniform::Color;
			case typeof<mat2>	::hash:	return Uniform::Matrix2;
			case typeof<mat3>	::hash:	return Uniform::Matrix3;
			case typeof<mat4>	::hash:	return Uniform::Matrix4;
			case typeof<Texture>::hash: return Uniform::Sampler;
			}
			return Uniform::Invalid;
		}
	}

	// Uniform Implementation
	template <class T> struct UniformImpl final : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type = typename T;

		using self_type = typename UniformImpl<value_type>;

		static constexpr int32_t ID { detail::uniform_id<value_type>() };

		value_type data;

		explicit UniformImpl(const String & name, value_type data)
			: Uniform { name }, data { data }
		{
		}

		inline self_type * clone() const override 
		{ 
			return new self_type { name, data }; 
		}

		inline const int32_t & getID() const override
		{
			return this->ID;
		}

		inline const typeof<> & getType() const override
		{
			static typeof<> temp { typeof<value_type>() };
			return temp;
		}

		inline bool isModifiable() const override
		{
			return // uniform owns its value or points to a texture
				std::is_same_v<value_type, detail::decay_t<value_type>> ||
				std::is_same_v<value_type, const Texture *>
				;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_bool		= typename UniformImpl<bool>;
	using uni_float		= typename UniformImpl<float_t>;
	using uni_int		= typename UniformImpl<int32_t>;
	using uni_vec2		= typename UniformImpl<vec2>;
	using uni_vec3		= typename UniformImpl<vec3>;
	using uni_vec4		= typename UniformImpl<vec4>;
	using uni_color		= typename UniformImpl<Color>;
	using uni_mat2		= typename UniformImpl<mat2>;
	using uni_mat3		= typename UniformImpl<mat3>;
	using uni_mat4		= typename UniformImpl<mat4>;
	using uni_sampler	= typename UniformImpl<const Texture *>; // All Texture Types
	using uni_bool_ptr	= typename UniformImpl<const bool *>;
	using uni_float_ptr = typename UniformImpl<const float_t *>;
	using uni_int_ptr	= typename UniformImpl<const int32_t *>;
	using uni_vec2_ptr	= typename UniformImpl<const vec2 *>;
	using uni_vec3_ptr	= typename UniformImpl<const vec3 *>;
	using uni_vec4_ptr	= typename UniformImpl<const vec4 *>;
	using uni_color_ptr = typename UniformImpl<const Color *>;
	using uni_mat2_ptr	= typename UniformImpl<const mat2 *>;
	using uni_mat3_ptr	= typename UniformImpl<const mat3 *>;
	using uni_mat4_ptr	= typename UniformImpl<const mat4 *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline const bool * as_bool(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<bool>::hash:
				return &(static_cast<const uni_bool *>(value)->data);
			
			case typeof<const bool *>::hash:
				return static_cast<const uni_bool_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const float_t * as_float(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<float_t>::hash:
				return &(static_cast<const uni_float *>(value)->data);
			
			case typeof<const float_t *>::hash:
				return static_cast<const uni_float_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const int32_t * as_int(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<int32_t>::hash:
				return &(static_cast<const uni_int *>(value)->data);
			
			case typeof<const int32_t *>::hash:
				return static_cast<const uni_int_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const vec2 * as_vec2(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec2>::hash:
				return &(static_cast<const uni_vec2 *>(value)->data);
			
			case typeof<const vec2 *>::hash:
				return static_cast<const uni_vec2_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const vec3 * as_vec3(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec3>::hash:
				return &(static_cast<const uni_vec3 *>(value)->data);
			
			case typeof<const vec3 *>::hash:
				return static_cast<const uni_vec3_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const vec4 * as_vec4(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec4>::hash:
				return &(static_cast<const uni_vec4 *>(value)->data);
			
			case typeof<const vec4 *>::hash:
				return static_cast<const uni_vec4_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const Color * as_color(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<Color>::hash:
				return &(static_cast<const uni_color *>(value)->data);
			
			case typeof<const Color *>::hash:
				return static_cast<const uni_color_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const mat2 * as_mat2(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat2>::hash:
				return &(static_cast<const uni_mat2 *>(value)->data);
			
			case typeof<const mat2 *>::hash:
				return static_cast<const uni_mat2_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const mat3 * as_mat3(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat3>::hash:
				return &(static_cast<const uni_mat3 *>(value)->data);
			
			case typeof<const mat3 *>::hash:
				return static_cast<const uni_mat3_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const mat4 * as_mat4(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat4>::hash:
				return &(static_cast<const uni_mat4 *>(value)->data);
			
			case typeof<const mat4 *>::hash:
				return static_cast<const uni_mat4_ptr *>(value)->data;
			}
			return nullptr;
		}

		static inline const Texture * as_sampler(const Uniform * value)
		{
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<const Texture *>::hash:
				return static_cast<const uni_sampler *>(value)->data;
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_