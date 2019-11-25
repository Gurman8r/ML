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

		enum class Type : hash_t
		{
			ID_bool, ID_int, ID_float,
			ID_vec2, ID_vec3, ID_vec4, ID_color,
			ID_mat2, ID_mat3, ID_mat4,
			ID_sampler,
			ID_MAX
		};

		static constexpr C_String Type_names[(size_t)Type::ID_MAX] =
		{
			"bool", "int", "float",
			"vec2", "vec3", "vec4", "color",
			"mat2", "mat3", "mat4",
			"sampler"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr hash_t category_of(hash_t code)
		{
			switch (code)
			{
			case typeof<bool>	::hash:	return (hash_t)Type::ID_bool;
			case typeof<int32_t>::hash: return (hash_t)Type::ID_int;
			case typeof<float_t>::hash: return (hash_t)Type::ID_float;
			case typeof<vec2>	::hash:	return (hash_t)Type::ID_vec2;
			case typeof<vec3>	::hash:	return (hash_t)Type::ID_vec3;
			case typeof<vec4>	::hash:	return (hash_t)Type::ID_vec4;
			case typeof<Color>	::hash:	return (hash_t)Type::ID_color;
			case typeof<mat2>	::hash:	return (hash_t)Type::ID_mat2;
			case typeof<mat3>	::hash:	return (hash_t)Type::ID_mat3;
			case typeof<mat4>	::hash:	return (hash_t)Type::ID_mat4;
			case typeof<Texture>::hash: return (hash_t)Type::ID_sampler;
			}
			return (hash_t)Type::ID_MAX;
		}

		template <class T> static constexpr hash_t category_of()
		{
			return Uniform::category_of(typeof<detail::root_t<T>>::hash);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(String const & name) : m_name{ name } {}

		virtual ~Uniform() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual hash_t category() const = 0;

		virtual Uniform * clone() const = 0;

		virtual bool is_modifiable() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual typeof<> const & get_base_type() const = 0;

		virtual typeof<> const & get_data_type() const = 0;

		virtual typeof<> const & get_root_type() const = 0;

		virtual typeof<> const & get_self_type() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t get_base_id() const { return this->get_base_type().hash; }

		inline hash_t get_data_id() const { return this->get_data_type().hash; }

		inline hash_t get_root_id() const { return this->get_root_type().hash; }

		inline hash_t get_self_id() const { return this->get_self_type().hash; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline String const & name() const
		{
			return m_name;
		}

		inline Uniform & rename(String const & value)
		{
			m_name = value;
			return (*this);
		}

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

		explicit UniformImpl(String const & name, data_type const & data)
			: Uniform { name }, m_data { data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t category() const override
		{
			static constexpr auto temp{ Uniform::category_of<base_type>() };
			return temp;
		}

		inline self_type * clone() const override
		{
			return new self_type{ this->name(), this->get() };
		}

		inline bool is_modifiable() const override
		{
			static constexpr bool temp{ (
				std::is_same_v<base_type, root_type> ||
				std::is_same_v<base_type, Texture const *>) &&
				!std::is_same_v<data_type, std::function<base_type()>>
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline typeof<> const & get_base_type() const override
		{
			static constexpr auto temp{ typeof<base_type>{} };
			return temp;
		}

		inline typeof<> const & get_data_type() const override
		{
			static constexpr auto temp{ typeof<data_type>{} };
			return temp;
		}

		inline typeof<> const & get_root_type() const override
		{
			static constexpr auto temp{ typeof<root_type>{} };
			return temp;
		}

		inline typeof<> const & get_self_type() const override
		{
			static constexpr auto temp{ typeof<self_type>{} };
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline data_type const & get() const
		{
			return m_data;
		}

		inline self_type & set(data_type const & value)
		{
			m_data = value; return (*this);
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
	ML_USING	uni_vec2			= typename uni_value	<vec2f>;
	ML_USING	uni_vec3			= typename uni_value	<vec3f>;
	ML_USING	uni_vec4			= typename uni_value	<vec4f>;
	ML_USING	uni_color			= typename uni_value	<Color>;
	ML_USING	uni_mat2			= typename uni_value	<mat2f>;
	ML_USING	uni_mat3			= typename uni_value	<mat3f>;
	ML_USING	uni_mat4			= typename uni_value	<mat4f>;
	ML_USING	uni_sampler			= typename uni_value	<Texture const *>;

	ML_USING	uni_bool_ptr		= typename uni_pointer	<bool>;
	ML_USING	uni_int_ptr			= typename uni_pointer	<int32_t>;
	ML_USING	uni_float_ptr		= typename uni_pointer	<float_t>;
	ML_USING	uni_vec2_ptr		= typename uni_pointer	<vec2f>;
	ML_USING	uni_vec3_ptr		= typename uni_pointer	<vec3f>;
	ML_USING	uni_vec4_ptr		= typename uni_pointer	<vec4f>;
	ML_USING	uni_color_ptr		= typename uni_pointer	<Color>;
	ML_USING	uni_mat2_ptr		= typename uni_pointer	<mat2f>;
	ML_USING	uni_mat3_ptr		= typename uni_pointer	<mat3f>;
	ML_USING	uni_mat4_ptr		= typename uni_pointer	<mat4f>;
	ML_USING	uni_sampler_ptr		= typename uni_pointer	<Texture const *>;

	ML_USING	uni_bool_func		= typename uni_function	<bool>;
	ML_USING	uni_int_func		= typename uni_function	<int32_t>;
	ML_USING	uni_float_func		= typename uni_function	<float_t>;
	ML_USING	uni_vec2_func		= typename uni_function	<vec2f>;
	ML_USING	uni_vec3_func		= typename uni_function	<vec3f>;
	ML_USING	uni_vec4_func		= typename uni_function	<vec4f>;
	ML_USING	uni_color_func		= typename uni_function	<Color>;
	ML_USING	uni_mat2_func		= typename uni_function	<mat2f>;
	ML_USING	uni_mat3_func		= typename uni_function	<mat3f>;
	ML_USING	uni_mat4_func		= typename uni_function	<mat4f>;
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
			return &(temp = static_cast<uni_value<T> const *>(value)->get());
		}
		case typeof<T const *>::hash:
		{
			if (auto const & ptr{ static_cast<uni_pointer<T> const *>(value)->get() })
			{
				return &(temp = (*ptr));
			}
		}
		case typeof<std::function<T()>>::hash:
		{
			if (auto const & fun{ static_cast<uni_function<T> const *>(value)->get() })
			{
				return &(temp = fun());
			}
		}
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_