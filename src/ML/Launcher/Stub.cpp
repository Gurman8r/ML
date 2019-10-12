
#include <ML/Core/Matrix.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/TypeOf.hpp>
#include <ML/Core/Duration.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Core/Bitset.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void stub()
	{
		// Type Tests
		{
			static_assert(typeof<uint32_t>::name == "unsigned int");
			static_assert(typeof<float_t>::name == "float");
			static_assert(typeof<Debug>::name == "ml::Debug");
			static_assert(typeof<std::string>::name == "std::string");
			static_assert(typeof<String>::name == "ml::BasicString<char>");
		}

		// Matrix Tests
		{
			constexpr mat4f ma {
				0.f,	1.f,	2.f,	3.f,
				4.f,	5.f,	6.f,	7.f,
				8.f,	9.f,	10.f,	11.f,
				12.f,	13.f,	14.f,	15.f
			};

			constexpr mat4f mb = alg::transpose(ma);

			constexpr vec3f v3 { 1, 2, 3 };
			constexpr mat3f m3 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			constexpr mat4f m4 = v3;
			constexpr mat4f m5 = m3;

			constexpr auto rebaseV3 = alg::rebase(v3, m4);
			constexpr auto rebaseM3 = alg::rebase(m3, m4);

			constexpr auto va		= vec2 { 0.f, 0.f }; 
			constexpr auto vb		= vec2 { -10.f, -10.f };
			constexpr auto vc		= alg::lerp(va, vb, 0.5f);

			constexpr auto arr1 = Array<char, 3> { 'a', 'b', 'c' };
			constexpr auto arr2 = Array<char, 3> { 'a', 'b', 'c' };
			static_assert(arr1 == arr2, "What?");

			static_assert(StringView("Here") == StringView("Here"), "What?");

			constexpr hash_t hash1	= Hash("Here");
			constexpr hash_t hash2	= StringView("Here").hash();
			constexpr hash_t hash3	= mat4i::identity().hash();
			constexpr hash_t hash4	= mat4f::identity().hash();

			constexpr auto eps1		= constant_t<float64_t>::epsilon;
			constexpr auto eps2		= constant_t<float80_t>::epsilon;
			constexpr auto sqr_mag	= alg::sqr_magnitude(vb);
			constexpr auto mag		= alg::magnitude(vb);
			constexpr auto norm		= alg::normalize(vb);
			constexpr auto lerp		= alg::lerp(va, vb, 0.5f);
			constexpr auto sqr		= alg::sqrt<float_t>()(13.0f);
			constexpr auto det		= alg::determinant(ma);
			constexpr auto inv		= alg::inverse(ma);
			constexpr auto dot		= alg::dot(ma, mb);
			constexpr auto pow		= alg::pow(1.23, 10);
			constexpr auto fact		= alg::fact(10);
			constexpr auto cross	= alg::cross(vec3 { 1, 2, 3 }, vec3 { 4, 5, 6 });
		}

		// Quaternion Tests
		{
			constexpr quat		q			{ 1.0f, 2.0f, 3.0f, 4.0f };
			constexpr vec3		q_complex	= q.complex();
			constexpr float_t	q_real		= q.real();
			constexpr vec3		q_axis		= q.axis();
			constexpr quat		q_angAxis	= quat::angleAxis(q_real, q_complex);
			constexpr float_t	q_pitch		= q.pitch();
			constexpr float_t	q_roll		= q.roll();
			constexpr float_t	q_yaw		= q.yaw();
			constexpr vec3		q_euler		= q.eulerAngles();
			constexpr mat3		q_toMat3	= q.as_mat3();
			constexpr mat4		q_toMat4	= q.as_mat4();
		}

		// Geometry Tests
		{
			constexpr auto tri		= geo::tri::contiguous;
			constexpr auto quad		= geo::quad::contiguous;
			constexpr auto cube		= geo::cube::contiguous;
			constexpr auto sky		= geo::sky::contiguous;
		}

		// Camera Tests
		{
			constexpr auto res		= vec2 { 1280, 720 };
			constexpr auto fov		= 45.f;
			constexpr auto aspect	= (res[0] / res[1]);
			constexpr auto zNear	= 0.1f;
			constexpr auto zFar		= 1000.f;
			//constexpr auto persp	= mat4::persp(fov, aspect, zNear, zFar);
			//constexpr auto ortho	= mat4::ortho(0, res[0], res[1], 0);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_name = typename StringView;

	enum uni_type : int32_t
	{
		U_Invalid = -1,
		U_Boolean, U_Float, U_Integer,
		U_Vector2, U_Vector3, U_Vector4, U_Color,
		U_Matrix2, U_Matrix3, U_Matrix4,
		U_Sampler
	};

	using uni_data = typename std::variant<
		bool, int32_t, float_t,
		vec2, vec3, vec4, Color,
		mat2, mat3, mat4,
		const Texture *
	>;

	using uni_base = typename std::tuple<uni_name, uni_type, uni_data>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr uni_type uni_type_values[] = {
		U_Boolean, U_Float, U_Integer,
		U_Vector2, U_Vector3, U_Vector4, U_Color,
		U_Matrix2, U_Matrix3, U_Matrix4,
		U_Sampler
	};

	static constexpr uni_name uni_type_names[] = {
		"bool", "float", "int",
		"vec2", "vec3", "vec4", "color",
		"mat2", "mat3", "mat4",
		"sampler"
	};

	namespace alg
	{
		inline String to_string(uni_type value)
		{
			return ((value > U_Invalid) ? uni_type_names[(size_t)value] : uni_name { "" });
		}
	}

	inline ML_SERIALIZE(std::ostream & out, const uni_type & value)
	{
		return out << util::to_string(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr int32_t uniform_type(const uni_data & value)
	{
		if (std::holds_alternative<bool>(value)) return U_Boolean;
		else if (std::holds_alternative<float_t>(value)) return U_Float;
		else if (std::holds_alternative<int32_t>(value)) return U_Integer;
		else if (std::holds_alternative<vec2>(value)) return U_Vector2;
		else if (std::holds_alternative<vec3>(value)) return U_Vector3;
		else if (std::holds_alternative<vec4>(value)) return U_Vector4;
		else if (std::holds_alternative<Color>(value)) return U_Color;
		else if (std::holds_alternative<mat2>(value)) return U_Matrix2;
		else if (std::holds_alternative<mat3>(value)) return U_Matrix3;
		else if (std::holds_alternative<mat4>(value)) return U_Matrix4;
		else if (std::holds_alternative<const Texture *>(value)) return U_Sampler;
		else return U_Invalid;
	}

	static constexpr int32_t uniform_type(const uni_base & value)
	{
		return uniform_type(std::get<2>(value));
	}

	template <class T> static constexpr int32_t uniform_type(const typeof<T> &)
	{
		switch (typeof<T>::hash)
		{
		case typeof<bool>::hash: return U_Boolean;
		case typeof<float_t>::hash: return U_Float;
		case typeof<int32_t>::hash: return U_Integer;
		case typeof<vec2>::hash: return U_Vector2;
		case typeof<vec3>::hash: return U_Vector3;
		case typeof<vec4>::hash: return U_Vector4;
		case typeof<Color>::hash: return U_Color;
		case typeof<mat2>::hash: return U_Matrix2;
		case typeof<mat3>::hash: return U_Matrix3;
		case typeof<mat4>::hash: return U_Matrix4;
		case typeof<const Texture *>::hash: return U_Sampler;
		default: return U_Invalid;
		}
	}

	template <class T> static constexpr int32_t uniform_type()
	{
		return uniform_type(typeof<T>());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr uni_base make_uniform(const uni_name & name, const uni_type & type, const uni_data & data)
	{
		return std::tie(name, type, data);
	}

	static constexpr uni_base make_uniform(const uni_name & name, const uni_data & data)
	{
		return make_uniform(name, (uni_type)uniform_type(data), data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> static constexpr const T * uniform_cast(const uni_base & value)
	{
		return (std::holds_alternative<T>(std::get<2>(value))
			? &std::get<T>(std::get<2>(value))
			: nullptr
		);
	}

	template <
		class T
	> static constexpr const T & uniform_cast(const uni_base & value, const T & dv)
	{
		const T * temp { uniform_cast<T>(value) };
		return (temp ? (*temp) : dv);
	}

	template <
		class T
	> static inline bool uniform_cast(const uni_base & value, T & out, const T & dv)
	{
		const T * temp { uniform_cast<T>(value) };
		return (temp ? ML_TRUE_EXPR(out = (*temp)) : ML_FALSE_EXPR(out = dv));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct uniform_t;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct uniform_t<T> final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { ID = uniform_type<T>() };

		constexpr uniform_t(const uniform_t<T> & copy)
			: m_base { copy.base() }
		{
		}

		constexpr uniform_t(uni_name name, T data)
			: m_base { make_uniform(name, data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool apply(const Shader * shader) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto base() const -> const uni_base & { return m_base; }

		constexpr auto name() const -> const uni_name & { return std::get<0>(m_base); }

		constexpr auto type() const -> const uni_type & { return std::get<1>(m_base); }

		constexpr auto data() const -> const T * { return uniform_cast<T>(m_base); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const uni_name & rename(const uni_name & value)
		{
			return (std::get<0>(m_base) = value);
		}

		inline const uni_data & update(const T & value)
		{
			return (std::get<2>(m_base) = value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: uni_base m_base;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct uniform_t<> final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr uniform_t(const uniform_t<T> & copy)
			: m_base { copy.base() }
		{
		}

		template <class T> constexpr uniform_t(uni_name name, T data)
			: m_base { make_uniform(name, data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool apply(const Shader * shader) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto base() const -> const uni_base & { return m_base; }

		constexpr auto name() const -> const uni_name & { return std::get<0>(m_base); }

		constexpr auto type() const -> const uni_type & { return std::get<1>(m_base); }

		template <class T>
		constexpr auto data() const -> const T * { return uniform_cast<T>(m_base); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const uni_name & rename(const uni_name & value)
		{
			return (std::get<0>(m_base) = value);
		}

		inline const uni_data & update(const uni_data & value)
		{
			return (std::get<2>(m_base) = value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: uni_base m_base;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uniform			= typename uniform_t<>;
	using uniform_bool		= typename uniform_t<bool>;
	using uniform_float		= typename uniform_t<float_t>;
	using uniform_int		= typename uniform_t<int32_t>;
	using uniform_vec2		= typename uniform_t<vec2>;
	using uniform_vec3		= typename uniform_t<vec3>;
	using uniform_vec4		= typename uniform_t<vec4>;
	using uniform_color		= typename uniform_t<Color>;
	using uniform_mat2		= typename uniform_t<mat2>;
	using uniform_mat3		= typename uniform_t<mat3>;
	using uniform_mat4		= typename uniform_t<mat4>;
	using uniform_sampler	= typename uniform_t<const Texture *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		static constexpr auto as_bool(const uniform * value)
			-> const bool *
		{
			return ((value && value->type() == uni_type::U_Boolean)
				? value->data<bool>()
				: nullptr
			);
		}

		static constexpr auto as_float(const uniform * value)
			-> const float_t *
		{
			return ((value && value->type() == uni_type::U_Float)
				? value->data<float_t>()
				: nullptr
			);
		}

		static constexpr auto as_int(const uniform * value)
			-> const int32_t *
		{
			return ((value && value->type() == uni_type::U_Integer)
				? value->data<int32_t>()
				: nullptr
			);
		}

		static constexpr auto as_vec2(const uniform * value)
			-> const vec2 *
		{
			return ((value && value->type() == uni_type::U_Vector2)
				? value->data<vec2>()
				: nullptr
			);
		}

		static constexpr auto as_vec3(const uniform * value)
			-> const vec3 *
		{
			return ((value && value->type() == uni_type::U_Vector3)
				? value->data<vec3>()
				: nullptr
			);
		}

		static constexpr auto as_vec4(const uniform * value)
			-> const vec4 *
		{
			return ((value && value->type() == uni_type::U_Vector4)
				? value->data<vec4>()
				: nullptr
			);
		}

		static constexpr auto as_color(const uniform * value)
			-> const Color *
		{
			return ((value && value->type() == uni_type::U_Vector4)
				? value->data<Color>()
				: nullptr
			);
		}

		static constexpr auto as_mat2(const uniform * value)
			-> const mat2 *
		{
			return ((value && value->type() == uni_type::U_Vector2)
				? value->data<mat2>()
				: nullptr
			);
		}

		static constexpr auto as_mat3(const uniform * value)
			-> const mat3 *
		{
			return ((value && value->type() == uni_type::U_Vector3)
				? value->data<mat3>()
				: nullptr
			);
		}

		static constexpr auto as_mat4(const uniform * value)
			-> const mat4 *
		{
			return ((value && value->type() == uni_type::U_Vector4)
				? value->data<mat4>()
				: nullptr
			);
		}

		static constexpr auto as_sampler(const uniform * value)
			-> const Texture * const *
		{
			return ((value && value->type() == uni_type::U_Sampler)
				? value->data<const Texture *>()
				: nullptr
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool uniform_t<>::apply(const Shader * shader) const
	{
		if (!shader || !name()) { return false; }
		switch (type())
		{
		case uniform_bool::ID	: return shader->setUniform(name(), *data<bool>());
		case uniform_float::ID	: return shader->setUniform(name(), *data<float_t>());
		case uniform_int::ID	: return shader->setUniform(name(), *data<int32_t>());
		case uniform_vec2::ID	: return shader->setUniform(name(), *data<vec2>());
		case uniform_vec3::ID	: return shader->setUniform(name(), *data<vec3>());
		case uniform_vec4::ID	: return shader->setUniform(name(), *data<vec4>());
		case uniform_color::ID	: return shader->setUniform(name(), *data<Color>());
		case uniform_mat2::ID	: return shader->setUniform(name(), *data<mat2>());
		case uniform_mat3::ID	: return shader->setUniform(name(), *data<mat3>());
		case uniform_mat4::ID	: return shader->setUniform(name(), *data<mat4>());
		case uniform_sampler::ID: return shader->setUniform(name(), *data<const Texture *>());
		}
		return false;
	}

	inline bool uniform_t<bool>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<int32_t>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<float_t>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<vec2>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<vec3>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<vec4>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<Color>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<mat2>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<mat3>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<mat4>::apply(const Shader * shader) const
	{
		return shader && name() && shader->setUniform(name(), (*data()));
	}

	inline bool uniform_t<const Texture *>::apply(const Shader * shader) const
	{
		return shader && name() && (*data()) && shader->setUniform(name(), (**data()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline auto test_uniforms()
	{
		{
			constexpr uniform_float f1("f1", 0.0f);
			if (const float_t * temp { f1.data() }) {}

			constexpr uniform_color c4("c4", Colors::white);
			if (const Color * temp { c4.data() }) {}

			constexpr uniform_mat4 m4("m4", mat4::identity());
			if (const mat4 * temp { m4.data() }) {}

			{
				constexpr uniform_sampler t2("t2", nullptr);
				if (const Texture * const * temp { t2.data() }) {}
			}
			{
				constexpr uniform t2("t2", nullptr);
				if (const Texture * const * temp { detail::as_sampler(&t2) }) {}
			}
			{
				constexpr uniform t2("t2", nullptr);
				if (auto temp { t2.data<const Texture *>() }) {}
			}
		}
		{
			uniform u = uniform_mat4("", mat4 {});
			u.update(mat4::identity());
			Shader s;
			u.apply(&s);
		}
		{
			List<uniform> u;
			u.push_back(uniform("b", true));
			u.push_back(uniform("f", 1.0f));
			u.push_back(uniform("i", 123));
			u.push_back(uniform("v2", vec2 { }));

			u[0].update(false);
			u[1].update(2.0f);
			u[2].update(456);
			u[3].update(nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		template <class, class> class Base, class K, class V
	> struct DenseTable
	{
		using key_type		= typename K;
		using value_type	= typename V;
		using pair_type		= typename std::pair<K, V>;
		using base_type		= typename Base<K, V>;
		using list_type		= typename List<pair_type>;

		DenseTable()
			: m_data { }
		{
		}

		~DenseTable() {}

	private: list_type m_data;
	};

	template <class K, class V>
	using DenseHashMap = DenseTable<HashMap, K, V>;

	template <class K, class V>
	using DenseTreeMap = DenseTable<Tree, K, V>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}