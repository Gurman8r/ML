#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/BitMask.hpp>

struct ml::Texture;

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base Uniform
	struct Uniform : public Newable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : hash_t
		{
			U_Boolean, U_Float, U_Integer,
			U_Vector2, U_Vector3, U_Vector4, U_Color,
			U_Matrix2, U_Matrix3, U_Matrix4,
			U_Sampler,
			MAX_UNIFORM_TYPE
		};

		static constexpr C_String Type_names[MAX_UNIFORM_TYPE] = 
		{
			"bool", "float", "int",
			"vec2", "vec3", "vec4", "color",
			"mat2", "mat3", "mat4",
			"sampler"
		};

		template <class T> static constexpr hash_t category()
		{
			switch (typeof<detail::decay_t<T>>::hash)
			{
			case typeof<bool>	::hash:	return U_Boolean;
			case typeof<float_t>::hash: return U_Float;
			case typeof<int32_t>::hash: return U_Integer;
			case typeof<vec2>	::hash:	return U_Vector2;
			case typeof<vec3>	::hash:	return U_Vector3;
			case typeof<vec4>	::hash:	return U_Vector4;
			case typeof<Color>	::hash:	return U_Color;
			case typeof<mat2>	::hash:	return U_Matrix2;
			case typeof<mat3>	::hash:	return U_Matrix3;
			case typeof<mat4>	::hash:	return U_Matrix4;
			case typeof<Texture>::hash: return U_Sampler;
			}
			return static_cast<hash_t>(-1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String name;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name) : name { name } {}

		virtual ~Uniform() {}

		virtual Uniform * clone() const = 0;
		
		virtual const hash_t & getID() const = 0;

		virtual const typeof<> & getType() const = 0;
		
		virtual bool isModifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> inline U * as() { return dynamic_cast<U *>(this); }

		template <
			class U
		> inline const U * as() const { return dynamic_cast<const U *>(this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const Uniform & value)
		{
			return out << Uniform::Type_names[value.getID()];
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

	// Uniform Implementation
	template <class Base, class Value> struct UniformImpl final : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename Base;

		using value_type = typename Value;
		
		using self_type = typename UniformImpl<base_type, value_type>;

		static constexpr hash_t ID { Uniform::category<base_type>() };

		static constexpr typeof<> type { typeof<value_type>() };

		value_type data;

		explicit UniformImpl(const String & name, value_type data)
			: Uniform { name }, data { data }
		{
		}

		inline self_type * clone() const override
		{
			return new self_type { name, data }; 
		}

		inline const hash_t & getID() const override
		{
			return self_type::ID;
		}

		inline const typeof<> & getType() const override
		{
			return self_type::type;
		}

		inline bool isModifiable() const override
		{
			// uniform owns its value and is not a function
			static const bool is_fun { 
				getType().name.str().find("function") != String::npos
			};
			return (std::is_same_v<value_type, detail::decay_t<value_type>> 
				|| std::is_same_v<value_type, const Texture *>)
				&& !is_fun;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_bool			= typename UniformImpl<bool,	bool>;
	using uni_float			= typename UniformImpl<float_t, float_t>;
	using uni_int			= typename UniformImpl<int32_t, int32_t>;
	using uni_vec2			= typename UniformImpl<vec2,	vec2>;
	using uni_vec3			= typename UniformImpl<vec3,	vec3>;
	using uni_vec4			= typename UniformImpl<vec4,	vec4>;
	using uni_color			= typename UniformImpl<Color,	Color>;
	using uni_mat2			= typename UniformImpl<mat2,	mat2>;
	using uni_mat3			= typename UniformImpl<mat3,	mat3>;
	using uni_mat4			= typename UniformImpl<mat4,	mat4>;
	using uni_sampler		= typename UniformImpl<Texture, const Texture *>;

	using uni_bool_ptr		= typename UniformImpl<bool,	const bool *>;
	using uni_float_ptr		= typename UniformImpl<float_t, const float_t *>;
	using uni_int_ptr		= typename UniformImpl<int32_t, const int32_t *>;
	using uni_vec2_ptr		= typename UniformImpl<vec2,	const vec2 *>;
	using uni_vec3_ptr		= typename UniformImpl<vec3,	const vec3 *>;
	using uni_vec4_ptr		= typename UniformImpl<vec4,	const vec4 *>;
	using uni_color_ptr		= typename UniformImpl<Color,	const Color *>;
	using uni_mat2_ptr		= typename UniformImpl<mat2,	const mat2 *>;
	using uni_mat3_ptr		= typename UniformImpl<mat3,	const mat3 *>;
	using uni_mat4_ptr		= typename UniformImpl<mat4,	const mat4 *>;
	using uni_sampler_ptr	= typename UniformImpl<Texture, const Texture * const *>;

	using uni_bool_clbk		= typename UniformImpl<bool,	std::function<bool()>>;
	using uni_float_clbk	= typename UniformImpl<float_t, std::function<float_t()>>;
	using uni_int_clbk		= typename UniformImpl<int32_t, std::function<int32_t()>>;
	using uni_vec2_clbk		= typename UniformImpl<vec2,	std::function<vec2()>>;
	using uni_vec3_clbk		= typename UniformImpl<vec3,	std::function<vec3()>>;
	using uni_vec4_clbk		= typename UniformImpl<vec4,	std::function<vec4()>>;
	using uni_color_clbk	= typename UniformImpl<Color,	std::function<Color()>>;
	using uni_mat2_clbk		= typename UniformImpl<mat2,	std::function<mat2()>>;
	using uni_mat3_clbk		= typename UniformImpl<mat3,	std::function<mat3()>>;
	using uni_mat4_clbk		= typename UniformImpl<mat4,	std::function<mat4()>>;
	using uni_sampler_clbk	= typename UniformImpl<Texture, std::function<const Texture *()>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline const bool * as_bool(const Uniform * value)
		{
			static bool temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<bool>::hash:
				return &(temp = static_cast<const uni_bool *>(value)->data);
			
			case typeof<const bool *>::hash:
				return &(temp = *(static_cast<const uni_bool_ptr *>(value)->data));

			case typeof<std::function<bool()>>::hash:
				return &(temp = static_cast<const uni_bool_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const float_t * as_float(const Uniform * value)
		{
			static float_t temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<float_t>::hash:
				return &(temp = static_cast<const uni_float *>(value)->data);
			
			case typeof<const float_t *>::hash:
				return &(temp = *(static_cast<const uni_float_ptr *>(value)->data));

			case typeof<std::function<float_t()>>::hash:
				return &(temp = static_cast<const uni_float_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const int32_t * as_int(const Uniform * value)
		{
			static int32_t temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<int32_t>::hash:
				return &(temp = static_cast<const uni_int *>(value)->data);

			case typeof<const int32_t *>::hash:
				return &(temp = *(static_cast<const uni_int_ptr *>(value)->data));

			case typeof<std::function<int32_t()>>::hash:
				return &(temp = static_cast<const uni_int_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const vec2 * as_vec2(const Uniform * value)
		{
			static vec2 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec2>::hash:
				return &(temp = static_cast<const uni_vec2 *>(value)->data);
			
			case typeof<const vec2 *>::hash:
				return &(temp = *(static_cast<const uni_vec2_ptr *>(value)->data));

			case typeof<std::function<vec2()>>::hash:
				return &(temp = static_cast<const uni_vec2_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const vec3 * as_vec3(const Uniform * value)
		{
			static vec3 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec3>::hash:
				return &(temp = static_cast<const uni_vec3 *>(value)->data);
			
			case typeof<const vec3 *>::hash:
				return &(temp = *(static_cast<const uni_vec3_ptr *>(value)->data));

			case typeof<std::function<vec3()>>::hash:
				return &(temp = static_cast<const uni_vec3_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const vec4 * as_vec4(const Uniform * value)
		{
			static vec4 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<vec4>::hash:
				return &(temp = static_cast<const uni_vec4 *>(value)->data);
			
			case typeof<const vec4 *>::hash:
				return &(temp = *(static_cast<const uni_vec4_ptr *>(value)->data));

			case typeof<std::function<vec4()>>::hash:
				return &(temp = static_cast<const uni_vec4_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const Color * as_color(const Uniform * value)
		{
			static Color temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<Color>::hash:
				return &(temp = static_cast<const uni_color *>(value)->data);
			
			case typeof<const Color *>::hash:
				return &(temp = *(static_cast<const uni_color_ptr *>(value)->data));

			case typeof<std::function<Color()>>::hash:
				return &(temp = static_cast<const uni_color_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const mat2 * as_mat2(const Uniform * value)
		{
			static mat2 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat2>::hash:
				return &(temp = static_cast<const uni_mat2 *>(value)->data);
			
			case typeof<const mat2 *>::hash:
				return &(temp = *(static_cast<const uni_mat2_ptr *>(value)->data));

			case typeof<std::function<mat2()>>::hash:
				return &(temp = static_cast<const uni_mat2_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const mat3 * as_mat3(const Uniform * value)
		{
			static mat3 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat3>::hash:
				return &(temp = static_cast<const uni_mat3 *>(value)->data);
			
			case typeof<const mat3 *>::hash:
				return &(temp = *(static_cast<const uni_mat3_ptr *>(value)->data));

			case typeof<std::function<mat3()>>::hash:
				return &(temp = static_cast<const uni_mat3_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const mat4 * as_mat4(const Uniform * value)
		{
			static mat4 temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<mat4>::hash:
				return &(temp = static_cast<const uni_mat4 *>(value)->data);
			
			case typeof<const mat4 *>::hash:
				return &(temp = *(static_cast<const uni_mat4_ptr *>(value)->data));

			case typeof<std::function<mat4()>>::hash:
				return &(temp = static_cast<const uni_mat4_clbk *>(value)->data());
			}
			return nullptr;
		}

		static inline const Texture * const * as_sampler(const Uniform * value)
		{
			static const Texture * temp { 0 };
			if (!value) return nullptr;
			switch (value->getType().hash)
			{
			case typeof<const Texture *>::hash:
				return &(temp = static_cast<const uni_sampler *>(value)->data);

			case typeof<const Texture * const *>::hash:
				return &(temp = *(static_cast<const uni_sampler_ptr *>(value)->data));

			case typeof<std::function<const Texture *()>>::hash:
				return &(temp = static_cast<const uni_sampler_clbk *>(value)->data());
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_