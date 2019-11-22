#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/MemoryTracker.hpp>
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

		explicit Uniform(String const & name) : m_name { name } {}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Uniform * clone() const = 0;
		
		virtual hash_t getID() const = 0;

		virtual typeof<> getType() const = 0;

		virtual hash_t getHash() const = 0;

		virtual bool isModifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getName() const -> String const & { return m_name; }

		inline auto setName(String const & value) -> String const & { return (m_name = value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, Uniform const & value)
		{
			return out << Uniform::Type_names[value.getID()];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: String m_name;

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr hash_t ID{ Uniform::category<base_type>() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit UniformImpl(String const & name, value_type data)
			: Uniform { name }, m_data { data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type * clone() const override { return new self_type { getName(), getData() };  }

		inline hash_t getID() const override { return self_type::ID; }

		inline typeof<> getType() const override { return typeof<value_type>{}; }

		inline hash_t getHash() const override { return getType().hash; }

		inline bool isModifiable() const override
		{
			// uniform owns its value and is not a function
			static const bool temp{
				(std::is_same_v<value_type, detail::decay_t<value_type>> || 
				std::is_same_v<value_type, Texture const *>)
				&& (typeof<value_type>::name.str().find("function") == String::npos)
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type const & getData() const { return m_data; }

		inline value_type const & setData(value_type value) { return (m_data = value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: value_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING_X	uni_value			= typename UniformImpl	<X, X>;
	ML_USING_X	uni_pointer			= typename UniformImpl	<X, X const *>;
	ML_USING_X	uni_function		= typename UniformImpl	<X, std::function<X()>>;

	ML_USING	uni_bool			= typename uni_value	<bool>;
	ML_USING	uni_int				= typename uni_value	<int32_t>;
	ML_USING	uni_float			= typename uni_value	<float_t>;
	ML_USING	uni_vec2			= typename uni_value	<vec2>;
	ML_USING	uni_vec3			= typename uni_value	<vec3>;
	ML_USING	uni_vec4			= typename uni_value	<vec4>;
	ML_USING	uni_color			= typename uni_value	<Color>;
	ML_USING	uni_mat2			= typename uni_value	<mat2>;
	ML_USING	uni_mat3			= typename uni_value	<mat3>;
	ML_USING	uni_mat4			= typename uni_value	<mat4>;
	ML_USING	uni_sampler			= typename uni_value	<Texture const *>;

	ML_USING	uni_bool_ptr		= typename uni_pointer	<bool>;
	ML_USING	uni_int_ptr			= typename uni_pointer	<int32_t>;
	ML_USING	uni_float_ptr		= typename uni_pointer	<float_t>;
	ML_USING	uni_vec2_ptr		= typename uni_pointer	<vec2>;
	ML_USING	uni_vec3_ptr		= typename uni_pointer	<vec3>;
	ML_USING	uni_vec4_ptr		= typename uni_pointer	<vec4>;
	ML_USING	uni_color_ptr		= typename uni_pointer	<Color>;
	ML_USING	uni_mat2_ptr		= typename uni_pointer	<mat2>;
	ML_USING	uni_mat3_ptr		= typename uni_pointer	<mat3>;
	ML_USING	uni_mat4_ptr		= typename uni_pointer	<mat4>;
	ML_USING	uni_sampler_ptr		= typename uni_pointer	<Texture const *>;

	ML_USING	uni_bool_func		= typename uni_function	<bool>;
	ML_USING	uni_int_func		= typename uni_function	<int32_t>;
	ML_USING	uni_float_func		= typename uni_function	<float_t>;
	ML_USING	uni_vec2_func		= typename uni_function	<vec2>;
	ML_USING	uni_vec3_func		= typename uni_function	<vec3>;
	ML_USING	uni_vec4_func		= typename uni_function	<vec4>;
	ML_USING	uni_color_func		= typename uni_function	<Color>;
	ML_USING	uni_mat2_func		= typename uni_function	<mat2>;
	ML_USING	uni_mat3_func		= typename uni_function	<mat3>;
	ML_USING	uni_mat4_func		= typename uni_function	<mat4>;
	ML_USING	uni_sampler_func	= typename uni_function	<Texture const *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline T const * uni_cast(Uniform const * value)
		{
			if (!value) { return nullptr; }
			static T temp{ 0 };
			switch (value->getHash())
			{
			case typeof<T>::hash:
				if (auto u{ static_cast<uni_value<T> const *>(value) })
				{
					return &(temp = u->getData());
				}

			case typeof<T const *>::hash:
				if (auto u{ static_cast<uni_pointer<T> const *>(value) }; u && u->getData())
				{
					return &(temp = (*u->getData()));
				}

			case typeof<std::function<T()>>::hash:
				if (auto u{ static_cast<uni_function<T> const *>(value) }; u && u->getData())
				{
					return &(temp = u->getData()());
				}
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_