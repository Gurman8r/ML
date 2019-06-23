#include <ML/Core/MetaMath.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	inline static int32_t meta_tests()
	{
		constexpr meta::mat4f ma {
			0.f,	1.f,	2.f,	3.f,
			4.f,	5.f,	6.f,	7.f,
			8.f,	9.f,	10.f,	11.f,
			12.f,	13.f,	14.f,	15.f
		};

		constexpr meta::mat4f mb = alg::transpose(ma);

		constexpr meta::vec3f v3 { 1, 2, 3 };
		constexpr meta::mat3f m3 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr meta::mat4f m4 = v3;
		constexpr meta::mat4f m5 = m3;

		constexpr auto rebaseV3 = alg::rebase(v3, m4);
		constexpr auto rebaseM3 = alg::rebase(m3, m4);

		constexpr auto va		= meta::vec2 { 0.f, 0.f };
		constexpr auto vb		= meta::vec2 { -10.f, -10.f };
		constexpr auto vc		= alg::lerp(va, vb, meta::vec2::type::half);

		constexpr auto arr1 = meta::array_t<char, 3> { 'a', 'b', 'c' };
		constexpr auto arr2 = meta::array_t<char, 3> { 'a', 'b', 'c' };
		static_assert(arr1 == arr2, "What?");

		static_assert(meta::c_string("Here") == meta::c_string("Here"), "What?");

		constexpr auto hash1	= ml::hash()("Here");
		constexpr auto hash2	= meta::c_string("Here").hash();
		constexpr auto hash3	= meta::mat4i::identity().hash();
		constexpr auto hash4	= meta::mat4f::identity().hash();

		constexpr auto eps		= meta::type_t<long double>::epsilon;
		constexpr auto sqr_mag	= alg::sqr_magnitude(vb);
		constexpr auto mag		= alg::magnitude(vb);
		constexpr auto norm		= alg::normalize(vb);
		constexpr auto lerp		= alg::lerp(va, vb, 0.5f);
		constexpr auto sqr		= alg::sqrt<float>()(13.0f);
		constexpr auto det		= alg::determinant(ma);
		constexpr auto inv		= alg::inverse(ma);
		constexpr auto dot		= alg::dot(ma, mb);
		constexpr auto pow		= alg::pow(1.23, 10);
		constexpr auto fact		= alg::fact(10);

		constexpr auto tri		= meta::geometry::tri::contiguous;
		constexpr auto quat		= meta::geometry::quad::contiguous;
		constexpr auto cube		= meta::geometry::cube::contiguous;

		constexpr auto res		= meta::vec2 { 1280, 720 };
		constexpr auto fov		= 45.f;
		constexpr auto aspect	= (res[0] / res[1]);
		constexpr auto zNear	= 0.1f;
		constexpr auto zFar		= 1000.f;
		constexpr auto ortho	= meta::mat4::ortho(0, res[0], res[1], 0);
		constexpr auto persp	= meta::mat4::persp(fov, aspect, zNear, zFar);

		return Debug::pause(0);
	}
}