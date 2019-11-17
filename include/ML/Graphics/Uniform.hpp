#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Trackable.hpp>
#include <ML/Core/BitMask.hpp>


namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Texture;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base Uniform
	struct Uniform : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : hash_t
		{
			ID_Boolean, ID_Integer, ID_Float,
			ID_Vector2, ID_Vector3, ID_Vector4, ID_Color,
			ID_Matrix2, ID_Matrix3, ID_Matrix4,
			ID_Sampler,
			MAX_UNIFORM_TYPE
		};

		static constexpr C_String Type_names[MAX_UNIFORM_TYPE] = 
		{
			"bool", "int", "float",
			"vec2", "vec3", "vec4", "color",
			"mat2", "mat3", "mat4",
			"sampler"
		};

		template <class T> static constexpr hash_t category()
		{
			switch (typeof<detail::decay_t<T>>::hash)
			{
			case typeof<bool>	::hash:	return ID_Boolean;
			case typeof<int32_t>::hash: return ID_Integer;
			case typeof<float_t>::hash: return ID_Float;
			case typeof<vec2>	::hash:	return ID_Vector2;
			case typeof<vec3>	::hash:	return ID_Vector3;
			case typeof<vec4>	::hash:	return ID_Vector4;
			case typeof<Color>	::hash:	return ID_Color;
			case typeof<mat2>	::hash:	return ID_Matrix2;
			case typeof<mat3>	::hash:	return ID_Matrix3;
			case typeof<mat4>	::hash:	return ID_Matrix4;
			case typeof<Texture>::hash: return ID_Sampler;
			}
			return MAX_UNIFORM_TYPE;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(const String & name) : m_name { name } {}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ptr_t<Uniform> clone() const = 0;
		
		virtual hash_t getID() const = 0;

		virtual typeof<> getInfo() const = 0;
		
		virtual bool isModifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getName() const -> const String & { return m_name; }

		inline auto setName(const String & value) -> const String & { return (m_name = value); }

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
			return ((lhs.getID() < rhs.getID()) || (lhs.getName() < rhs.getName()));
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

	private:
		String m_name;

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

		static constexpr hash_t ID{ Uniform::category<base_type>() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit UniformImpl(const String & name, value_type data)
			: Uniform { name }, m_data { data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type * clone() const override
		{
			return new self_type { getName(), getData() }; 
		}

		inline hash_t getID() const override { return self_type::ID; }

		inline typeof<> getInfo() const override { return typeof<value_type>{}; }

		inline bool isModifiable() const override
		{
			// uniform owns its value and is not a function
			static const bool is_fun { 
				getInfo().name.str().find("function") != String::npos
			};
			return (std::is_same_v<value_type, detail::decay_t<value_type>> 
				|| std::is_same_v<value_type, const_ptr_t<Texture>>)
				&& !is_fun;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const value_type & getData() const { return m_data; }

		inline const value_type & setData(value_type value) { return (m_data = value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		value_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_bool			= typename UniformImpl<bool,	bool>;
	using uni_int			= typename UniformImpl<int32_t, int32_t>;
	using uni_float			= typename UniformImpl<float_t, float_t>;
	using uni_vec2			= typename UniformImpl<vec2,	vec2>;
	using uni_vec3			= typename UniformImpl<vec3,	vec3>;
	using uni_vec4			= typename UniformImpl<vec4,	vec4>;
	using uni_color			= typename UniformImpl<Color,	Color>;
	using uni_mat2			= typename UniformImpl<mat2,	mat2>;
	using uni_mat3			= typename UniformImpl<mat3,	mat3>;
	using uni_mat4			= typename UniformImpl<mat4,	mat4>;
	using uni_sampler		= typename UniformImpl<Texture, const_ptr_t<Texture>>;

	using uni_bool_ptr		= typename UniformImpl<bool,	const_ptr_t<bool>>;
	using uni_int_ptr		= typename UniformImpl<int32_t, const_ptr_t<int32_t>>;
	using uni_float_ptr		= typename UniformImpl<float_t, const_ptr_t<float_t>>;
	using uni_vec2_ptr		= typename UniformImpl<vec2,	const_ptr_t<vec2>>;
	using uni_vec3_ptr		= typename UniformImpl<vec3,	const_ptr_t<vec3>>;
	using uni_vec4_ptr		= typename UniformImpl<vec4,	const_ptr_t<vec4>>;
	using uni_color_ptr		= typename UniformImpl<Color,	const_ptr_t<Color>>;
	using uni_mat2_ptr		= typename UniformImpl<mat2,	const_ptr_t<mat2>>;
	using uni_mat3_ptr		= typename UniformImpl<mat3,	const_ptr_t<mat3>>;
	using uni_mat4_ptr		= typename UniformImpl<mat4,	const_ptr_t<mat4>>;
	using uni_sampler_ptr	= typename UniformImpl<Texture, const_ptr_t<const_ptr_t<Texture>>>;

	using uni_bool_clbk		= typename UniformImpl<bool,	std::function<bool()>>;
	using uni_int_clbk		= typename UniformImpl<int32_t, std::function<int32_t()>>;
	using uni_float_clbk	= typename UniformImpl<float_t, std::function<float_t()>>;
	using uni_vec2_clbk		= typename UniformImpl<vec2,	std::function<vec2()>>;
	using uni_vec3_clbk		= typename UniformImpl<vec3,	std::function<vec3()>>;
	using uni_vec4_clbk		= typename UniformImpl<vec4,	std::function<vec4()>>;
	using uni_color_clbk	= typename UniformImpl<Color,	std::function<Color()>>;
	using uni_mat2_clbk		= typename UniformImpl<mat2,	std::function<mat2()>>;
	using uni_mat3_clbk		= typename UniformImpl<mat3,	std::function<mat3()>>;
	using uni_mat4_clbk		= typename UniformImpl<mat4,	std::function<mat4()>>;
	using uni_sampler_clbk	= typename UniformImpl<Texture, std::function<const_ptr_t<Texture>()>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline const_ptr_t<bool> as_bool(const_ptr_t<Uniform> value)
		{
			static bool temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<bool>::hash:
					return &(temp = static_cast<const uni_bool *>(value)->getData());

				case typeof<const_ptr_t<bool>>::hash:
					return &(temp = *(static_cast<const uni_bool_ptr *>(value)->getData()));

				case typeof<std::function<bool()>>::hash:
					return &(temp = static_cast<const uni_bool_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<int32_t> as_int(const_ptr_t<Uniform> value)
		{
			static int32_t temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<int32_t>::hash:
					return &(temp = static_cast<const uni_int *>(value)->getData());

				case typeof<const_ptr_t<int32_t>>::hash:
					return &(temp = *(static_cast<const uni_int_ptr *>(value)->getData()));

				case typeof<std::function<int32_t()>>::hash:
					return &(temp = static_cast<const uni_int_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<float_t> as_float(const_ptr_t<Uniform> value)
		{
			static float_t temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<float_t>::hash:
					return &(temp = static_cast<const uni_float *>(value)->getData());

				case typeof<const_ptr_t<float_t>>::hash:
					return &(temp = *(static_cast<const uni_float_ptr *>(value)->getData()));

				case typeof<std::function<float_t()>>::hash:
					return &(temp = static_cast<const uni_float_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<vec2> as_vec2(const_ptr_t<Uniform> value)
		{
			static vec2 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<vec2>::hash:
					return &(temp = static_cast<const uni_vec2 *>(value)->getData());

				case typeof<const_ptr_t<vec2>>::hash:
					return &(temp = *(static_cast<const uni_vec2_ptr *>(value)->getData()));

				case typeof<std::function<vec2()>>::hash:
					return &(temp = static_cast<const uni_vec2_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<vec3> as_vec3(const_ptr_t<Uniform> value)
		{
			static vec3 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<vec3>::hash:
					return &(temp = static_cast<const uni_vec3 *>(value)->getData());

				case typeof<const_ptr_t<vec3>>::hash:
					return &(temp = *(static_cast<const uni_vec3_ptr *>(value)->getData()));

				case typeof<std::function<vec3()>>::hash:
					return &(temp = static_cast<const uni_vec3_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<vec4> as_vec4(const_ptr_t<Uniform> value)
		{
			static vec4 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<vec4>::hash:
					return &(temp = static_cast<const uni_vec4 *>(value)->getData());

				case typeof<const_ptr_t<vec4>>::hash:
					return &(temp = *(static_cast<const uni_vec4_ptr *>(value)->getData()));

				case typeof<std::function<vec4()>>::hash:
					return &(temp = static_cast<const uni_vec4_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<Color> as_color(const_ptr_t<Uniform> value)
		{
			static Color temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<Color>::hash:
					return &(temp = static_cast<const uni_color *>(value)->getData());

				case typeof<const_ptr_t<Color>>::hash:
					return &(temp = *(static_cast<const uni_color_ptr *>(value)->getData()));

				case typeof<std::function<Color()>>::hash:
					return &(temp = static_cast<const uni_color_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<mat2> as_mat2(const_ptr_t<Uniform> value)
		{
			static mat2 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<mat2>::hash:
					return &(temp = static_cast<const uni_mat2 *>(value)->getData());

				case typeof<const_ptr_t<mat2>>::hash:
					return &(temp = *(static_cast<const uni_mat2_ptr *>(value)->getData()));

				case typeof<std::function<mat2()>>::hash:
					return &(temp = static_cast<const uni_mat2_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<mat3> as_mat3(const_ptr_t<Uniform> value)
		{
			static mat3 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<mat3>::hash:
					return &(temp = static_cast<const uni_mat3 *>(value)->getData());

				case typeof<const_ptr_t<mat3>>::hash:
					return &(temp = *(static_cast<const uni_mat3_ptr *>(value)->getData()));

				case typeof<std::function<mat3()>>::hash:
					return &(temp = static_cast<const uni_mat3_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<mat4> as_mat4(const_ptr_t<Uniform> value)
		{
			static mat4 temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<mat4>::hash:
					return &(temp = static_cast<const uni_mat4 *>(value)->getData());

				case typeof<const_ptr_t<mat4>>::hash:
					return &(temp = *(static_cast<const uni_mat4_ptr *>(value)->getData()));

				case typeof<std::function<mat4()>>::hash:
					return &(temp = static_cast<const uni_mat4_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		static inline const_ptr_t<const_ptr_t<Texture>> as_sampler(const_ptr_t<Uniform> value)
		{
			static const_ptr_t<Texture> temp{ 0 };
			if (!value) return nullptr;
			switch (value->getInfo().hash)
			{
				case typeof<const_ptr_t<Texture>>::hash:
					return &(temp = static_cast<const uni_sampler *>(value)->getData());

				case typeof<const_ptr_t<const_ptr_t<Texture>>>::hash:
					return &(temp = *(static_cast<const uni_sampler_ptr *>(value)->getData()));

				case typeof<std::function<const_ptr_t<Texture>()>>::hash:
					return &(temp = static_cast<const uni_sampler_clbk *>(value)->getData()());
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_