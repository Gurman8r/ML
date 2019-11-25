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
			switch (typeof<detail::root_t<T>>::hash)
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

		virtual hash_t category() const = 0;

		virtual Uniform * clone() const = 0;

		virtual bool is_modifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual typeof<> get_base_type() const = 0;

		virtual typeof<> get_data_type() const = 0;

		virtual typeof<> get_root_type() const = 0;

		virtual typeof<> get_self_type() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t get_base_id() const { return this->get_base_type().hash; }

		inline hash_t get_data_id() const { return this->get_data_type().hash; }

		inline hash_t get_root_id() const { return this->get_root_type().hash; }

		inline hash_t get_self_id() const { return this->get_self_type().hash; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline String const & name() const { return m_name; }

		inline Uniform & rename(String const & value) { m_name = value; return (*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, Uniform const & value)
		{
			return out << Uniform::Type_names[value.category()];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: String m_name;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Uniform Implementation
	template <class Base, class Data> struct UniformImpl final : public Uniform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename Base;

		using data_type = typename Data;
		
		using root_type = typename detail::root_t<base_type>;

		using self_type = typename UniformImpl<base_type, data_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr hash_t ID{ Uniform::category<base_type>() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit UniformImpl(String const & name, data_type const & data)
			: Uniform { name }, m_data { data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t category() const override
		{
			return self_type::ID;
		}

		inline self_type * clone() const override
		{
			return new self_type{ this->name(), this->get() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline typeof<> get_base_type() const override
		{
			static constexpr auto temp{ typeof<base_type>{} };
			return temp;
		}

		inline typeof<> get_data_type() const override
		{
			static constexpr auto temp{ typeof<data_type>{} };
			return temp;
		}

		inline typeof<> get_root_type() const override
		{
			static constexpr auto temp{ typeof<root_type>{} };
			return temp;
		}

		inline typeof<> get_self_type() const override
		{
			static constexpr auto temp{ typeof<self_type>{} };
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool is_modifiable() const override
		{
			static const bool temp{ (
				(std::is_same_v<data_type, root_type>) ||
				(std::is_same_v<data_type, Texture const *>)) &&
				(get_data_type().name.str().find("function") == String::npos)
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline data_type const & get() const
		{
			return m_data;
		}

		inline self_type & set(data_type const & value)
		{
			m_data = value;
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: data_type m_data;

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

	template <class T> static inline T const * uniform_cast(Uniform const * value)
	{
		if (!value) { return nullptr; }
		static T temp{ 0 };
		switch (value->get_data_id())
		{
		case typeof<T>::hash:
		{
			const auto u{ static_cast<uni_value<T> const *>(value) };
			return &(temp = u->get());
		}
		case typeof<T const *>::hash:
		{
			const auto u{ static_cast<uni_pointer<T> const *>(value) };
			if (const auto ptr{ u->get() }) return &(temp = (*ptr));
		}
		case typeof<std::function<T()>>::hash:
		{
			const auto u{ static_cast<uni_function<T> const *>(value) };
			if (const auto fun{ u->get() }) return &(temp = fun());
		}
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_