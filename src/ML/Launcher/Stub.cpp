#include <ML/Core/Matrix.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	inline static int32_t stub()
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
		constexpr auto vc		= alg::lerp(va, vb, vec2::type::half);

		constexpr auto arr1 = Array<char, 3> { 'a', 'b', 'c' };
		constexpr auto arr2 = Array<char, 3> { 'a', 'b', 'c' };
		static_assert(arr1 == arr2, "What?");

		static_assert(X_String("Here") == X_String("Here"), "What?");

		constexpr auto hash1	= Hash()("Here");
		constexpr auto hash2	= X_String("Here").hash();
		constexpr auto hash3	= mat4i::identity().hash();
		constexpr auto hash4	= mat4f::identity().hash();

		constexpr auto eps1		= type_t<float64_t>::epsilon;
		constexpr auto eps2		= type_t<float80_t>::epsilon;
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

		constexpr auto tri		= geo::tri::contiguous;
		constexpr auto quat		= geo::quad::contiguous;
		constexpr auto cube		= geo::cube::contiguous;

		constexpr auto res		= vec2 { 1280, 720 };
		constexpr auto fov		= 45.f;
		constexpr auto aspect	= (res[0] / res[1]);
		constexpr auto zNear	= 0.1f;
		constexpr auto zFar		= 1000.f;
		constexpr auto ortho	= mat4::ortho(0, res[0], res[1], 0);
		constexpr auto persp	= mat4::persp(fov, aspect, zNear, zFar);

		return Debug::pause(0);
	}
}