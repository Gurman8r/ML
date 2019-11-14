#include <ML/Core/Matrix.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/TypeOf.hpp>
#include <ML/Core/Duration.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Core/BitMask.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void test_stub()
	{
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

			constexpr hash_t hash1	= Hash("Here");
			constexpr hash_t hash2	= StringView("Here").hash();
			constexpr hash_t hash3	= mat4i::identity().hash();
			constexpr hash_t hash4	= mat4f::identity().hash();

			constexpr auto eps1		= limits<float64_t>::epsilon;
			constexpr auto eps2		= limits<float80_t>::epsilon;
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Base
	{
	};

	struct Allocable
	{
		virtual ~Allocable() {}

		inline ptr_t<void> operator new(size_t size) { return std::malloc(size); }
		inline ptr_t<void> operator new[](size_t size) { return std::malloc(size); }
		inline void	  operator delete(ptr_t<void> ptr) { return std::free(ptr); }
		inline void	  operator delete[](ptr_t<void> ptr) { return std::free(ptr); }
	};

	struct Serializable
	{
		virtual ~Serializable() {}

		inline virtual std::ostream & serialize(std::ostream & out) const = 0;

		inline virtual std::istream & deserialize(std::istream & in) = 0;

		inline friend ML_SERIALIZE(std::ostream & out, const Serializable & value)
		{
			return value.serialize(out);
		}

		inline friend ML_DESERIALIZE(std::istream & in, Serializable & value)
		{
			return value.deserialize(in);
		}
	};

	struct Child final
		: public Base
		, public Allocable
		, public Serializable
	{
		String name;

		Child()
			: name{}
		{
		}

		Child(const String & name)
			: name{ name }
		{
		}

		Child(const Child & copy)
			: name{ copy.name }
		{
		}

		std::ostream & serialize(std::ostream & out) const override
		{
			return out << this->name;
		}

		std::istream & deserialize(std::istream & in) override
		{
			return in >> this->name;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */