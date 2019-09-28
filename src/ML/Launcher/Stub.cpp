
#include <ML/Core/Matrix.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/TypeOf.hpp>
#include <ML/Core/Duration.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void stub()
	{
		// Type Tests
		{
			static_assert(typeof<int32_t>::name == "int");
			static_assert(typeof<uint32_t>::name == "unsigned int");
			static_assert(typeof<float_t>::name == "float");
			static_assert(typeof<C_String>::name == "const char*");
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

		// Color Tests
		{
			constexpr vec4f red		{ 1.0f, 0.0f, 0.0f, 1.0f };
			constexpr vec4f green	{ Color::green };
			constexpr vec4f blue	{ 0.0f, 0.0f, 1.0f, 1.0f };
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
	template <class T> struct bitset final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type	= typename detail::decay_t<T>;
		using bit	= typename bool;
		using byte	= typename uint8_t;
		using bits	= typename Array<bit, sizeof(type) * 8>;
		using bytes = typename Array<byte, sizeof(type)>;

		using pointer			= typename bit *;
		using reference			= typename bit &;
		using const_pointer		= typename const bit *;
		using const_reference	= typename const bit &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bitset() : m_bits { bits() } {}
		constexpr bitset(const bitset<type> & copy) : m_bits { copy.m_bits } {}
		constexpr bitset(const type & value) : m_bits { to_bits(value) } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr type get() const { return from_bits(m_bits); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr type from_bits(const bits & value)
		{
			type temp { 0 };
			for (size_t i = 0; i < sizeof(bits); i++)
			{
				bitWrite(temp, i, value[i]);
			}
			return temp;
		}

		static constexpr bits to_bits(const type & value)
		{
			bits temp { 0 };
			for (size_t i = 0; i < sizeof(bits); i++)
			{
				temp[i] = bitRead(value, i);
			}
			return temp;
		}

		static constexpr bytes to_bytes(const type & value)
		{
			bytes temp { 0 };
			for (size_t i = 0; i < sizeof(bytes); i++)
			{
				temp[i] = static_cast<bit>(value >> (i * 8));
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const { return m_bits[i]; }
		
		inline reference operator[](size_t i) { return m_bits[i]; }
		
		inline friend ML_SERIALIZE(std::ostream & out, const bitset<type> & value)
		{
			return out << value.m_bits;
		}

		inline friend ML_DESERIALIZE(std::istream & in, bitset<type> & value)
		{
			return in >> value.m_bits;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline		auto begin()			-> iterator			{ return &m_bits[0]; }
		constexpr	auto begin()	const	-> const_iterator	{ return &m_bits[0]; }
		constexpr	auto cbegin()	const	-> const_iterator	{ return begin(); }
		constexpr	auto cend()		const	-> const_iterator	{ return end(); }
		inline		auto end()				-> iterator			{ return &m_bits[sizeof(bits)]; }
		constexpr	auto end()		const	-> const_iterator	{ return &m_bits[sizeof(bits)]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: bits m_bits;
	};
}

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uni_name = typename C_String;

	enum class uni_type
	{
		Invalid = -1,
		Boolean, Float, Integer,
		Vector2, Vector3, Vector4, Color,
		Matrix3, Matrix4,
		Sampler
	};

	static constexpr uni_type uni_type_values[] = {
		uni_type::Boolean, uni_type::Float, uni_type::Integer,
		uni_type::Vector2, uni_type::Vector3, uni_type::Vector4, uni_type::Color,
		uni_type::Matrix3, uni_type::Matrix4,
		uni_type::Sampler
	};

	static constexpr uni_name uni_type_names[] = {
		"bool", "float", "int",
		"vec2", "vec3", "vec4", "color",
		"mat3", "mat4",
		"sampler"
	};

	inline ML_SERIALIZE(std::ostream & out, const uni_type & value)
	{
		return out << ((value > uni_type::Invalid) ? uni_type_names[(size_t)value] : "?");
	}

	using uni_info = typename std::pair<uni_name, uni_type>;

	using uni_data = typename std::variant<
		bool, int32_t, float_t, vec2, vec3, vec4, mat3, mat4, const Texture *
	>;

	using uni_pair = typename std::pair<uni_info, uni_data>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr int32_t uniform_id(const uni_data & value)
	{
		if (std::holds_alternative<bool>(value)) return 0;
		else if (std::holds_alternative<float_t>(value)) return 1;
		else if (std::holds_alternative<int32_t>(value)) return 2;
		else if (std::holds_alternative<vec2>(value)) return 3;
		else if (std::holds_alternative<vec3>(value)) return 4;
		else if (std::holds_alternative<vec4>(value)) return 5;
		//else if (std::holds_alternative<vec4>(value)) return 6; // color
		else if (std::holds_alternative<mat3>(value)) return 7;
		else if (std::holds_alternative<mat4>(value)) return 8;
		else if (std::holds_alternative<const Texture *>(value)) return 9;
		else return -1;
	}

	static constexpr int32_t uniform_id(const uni_pair & value)
	{
		return uniform_id(value.second);
	}

	template <class T> static constexpr int32_t uniform_id()
	{
		if (std::is_same_v<T, bool>) return 0;
		else if (std::is_same_v<T, float_t>) return 1;
		else if (std::is_same_v<T, int32_t>) return 2;
		else if (std::is_same_v<T, vec2>) return 3;
		else if (std::is_same_v<T, vec3>) return 4;
		else if (std::is_same_v<T, vec4>) return 5;
		//else if (std::is_same_v<T, vec4>) return 6; // color
		else if (std::is_same_v<T, mat3>) return 7;
		else if (std::is_same_v<T, mat4>) return 8;
		else if (std::is_same_v<T, const Texture *>) return 9;
		else return -1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr uni_pair make_uniform(const uni_info & info, const uni_data & data)
	{
		return std::make_pair(info, data);
	}

	static constexpr uni_pair make_uniform(const uni_name & name, const uni_type & type, const uni_data & data)
	{
		return make_uniform(std::make_pair(name, type), data);
	}

	static constexpr uni_pair make_uniform(const uni_name & name, const uni_data & data)
	{
		return make_uniform(std::make_pair(name, (uni_type)uniform_id(data)), data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> static constexpr const T * uniform_cast(const uni_pair & value)
	{
		return (std::holds_alternative<T>(value.second)
			? &std::get<T>(value.second)
			: nullptr
		);
	}

	template <
		class T
	> static constexpr T * uniform_cast(uni_pair & value)
	{
		return (std::holds_alternative<T>(value.second)
			? &std::get<T>(value.second)
			: nullptr
		);
	}

	template <
		class T
	> static constexpr const T & uniform_cast(const uni_pair & value, const T & dv)
	{
		const T * temp { uniform_cast<T>(value) };
		return (temp ? (*temp) : dv);
	}

	template <
		class T
	> static inline bool uniform_cast(const uni_pair & value, T & out, const T & dv)
	{
		const T * temp { uniform_cast<T>(value) };
		return (temp ? ML_TRUE_EXPR(out = (*temp)) : ML_FALSE_EXPR(out = dv));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct uniform_t;

	template <class T> struct uniform_t<T> final
	{
		using value_type = typename T;
		using pointer = typename value_type *;
		using reference = typename value_type &;
		using const_pointer = typename const value_type *;
		using const_reference = typename const value_type &;

		template <class U>
		static constexpr bool is_same { std::is_same_v<T, U> };
		static constexpr bool is_pointer { std::is_pointer_v<value_type> };
		static constexpr bool is_reference { std::is_reference_v<value_type> };
		static constexpr bool is_value { !is_pointer && !is_reference };
		static constexpr bool is_const_pointer { is_same<const_pointer> };
		static constexpr bool is_const_reference { is_same<const_reference> };

		static constexpr auto ID { (uni_type)uniform_id<value_type>() };

		constexpr uniform_t(const uniform_t<T> & copy)
			: m_base { copy.m_base }
		{
		}

		constexpr uniform_t(uni_name name, T data)
			: m_base { make_uniform(name, data) }
		{
		}

		constexpr auto base() const -> const uni_pair & { return m_base; }
		constexpr auto info() const -> const uni_info & { return base().first; }
		constexpr auto name() const -> const uni_name & { return info().first; }
		constexpr auto type() const -> const uni_type & { return info().second; }
		constexpr auto data() const -> const_pointer { return uniform_cast<T>(base()); }

		bool operator()(const Shader * shader) const;

	private: uni_pair m_base;
	};

	template <> struct uniform_t<> final
	{
		template <class T> constexpr uniform_t(const uniform_t<T> & copy)
			: m_base { copy.m_base }
		{
		}

		template <class T> constexpr uniform_t(uni_name name, T data)
			: m_base { make_uniform<T>(name, data) }
		{
		}

		constexpr auto base() const -> const uni_pair & { return m_base; }
		constexpr auto info() const -> const uni_info & { return base().first; }
		constexpr auto name() const -> const uni_name & { return info().first; }
		constexpr auto type() const -> const uni_type & { return info().second; }
		constexpr auto data() const -> const uni_data & { return base().second; }

		bool operator()(const Shader * shader) const;

	private: uni_pair m_base;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using uniform_bool = uniform_t<bool>;
	using uniform_float = uniform_t<float_t>;
	using uniform_int = uniform_t<int32_t>;
	using uniform_vec2 = uniform_t<vec2>;
	using uniform_vec3 = uniform_t<vec3>;
	using uniform_vec4 = uniform_t<vec4>;
	using uniform_color = uniform_t<vec4>;
	using uniform_mat3 = uniform_t<mat3>;
	using uniform_mat4 = uniform_t<mat4>;
	using uniform_sampler = uniform_t<const Texture *>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool uniform_t<>::operator()(const Shader * shader) const
	{
		if (!shader || !this->name()) { return false; }
		switch (this->type())
		{
		case uniform_bool::ID: if (auto temp { uniform_cast<bool>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_float::ID: if (auto temp { uniform_cast<float_t>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_int::ID: if (auto temp { uniform_cast<int32_t>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_vec2::ID: if (auto temp { uniform_cast<vec2>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_vec3::ID: if (auto temp { uniform_cast<vec3>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_vec4::ID: if (auto temp { uniform_cast<vec4>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_mat3::ID: if (auto temp { uniform_cast<mat3>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_mat4::ID: if (auto temp { uniform_cast<mat4>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		case uniform_sampler::ID: if (auto temp { uniform_cast<const Texture *>(this->base()) })
			return shader->setUniform(this->name(), (*temp));

		default: return false;
		}
	}

	template <class T> bool uniform_t<T>::operator()(const Shader * shader) const
	{
		return uniform_t<>(*this)(shader);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline auto test_uniforms()
	{
		{
			constexpr auto bb = make_uniform("a", true);
			constexpr auto ff = make_uniform("b", 1.0f);
			constexpr auto ii = make_uniform("c", 123);
			constexpr auto v2 = make_uniform("d", vec2 { 0, 1 });
			constexpr auto v3 = make_uniform("e", vec3 { 2, 3, 4 });
			constexpr auto v4 = make_uniform("f", vec4 { 5, 6, 7, 8 });
			constexpr auto m3 = make_uniform("g", mat3::identity());
			constexpr auto m4 = make_uniform("h", mat4::identity());
			constexpr auto t2 = make_uniform("i", (const Texture *)nullptr);
		}
		{
			uniform_t<const Texture *> tex("", nullptr);
			if (const Texture * const * temp { tex.data() }) {}

			uniform_t<float_t> flt("", 0.0f);
			if (const float_t * temp { flt.data() }) {}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}