#include <ML/Core/Meta.hpp>
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

		constexpr meta::mat4f mb = meta::alg::transpose(ma);

		constexpr meta::vec3f v3 { 1, 2, 3 };
		constexpr meta::mat3f m3 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr meta::mat4f m4 = v3;
		constexpr meta::mat4f m5 = m3;

		constexpr float angle	= 90.f * meta::type_t<float>::deg2rad;
		constexpr auto v		= meta::vec3f { 1.f, 1.f, 1.f } * (angle);
		constexpr auto rr		= meta::alg::rotationXYZ(v);
		constexpr auto rx		= meta::alg::rotationX(v[0]);
		constexpr auto ry		= meta::alg::rotationY(v[1]);
		constexpr auto rz		= meta::alg::rotationZ(v[2]);

		constexpr auto rebaseV3 = meta::alg::rebase(v3, m4);
		constexpr auto rebaseM3 = meta::alg::rebase(m3, m4);

		constexpr auto va		= meta::vec2 { 0.f, 0.f };
		constexpr auto vb		= meta::vec2 { -10.f, -10.f };
		constexpr auto vc		= meta::alg::lerp(va, vb, meta::vec2::type::half);

		constexpr auto hash1	= meta::alg::hash()("Here");
		constexpr auto hash2	= meta::c_string("Here").hash();
		constexpr auto hash3	= meta::mat4i::identity().hash();
		constexpr auto hash4	= meta::mat4f::identity().hash();

		constexpr auto eps		= meta::type_t<long double>::epsilon;
		constexpr auto sqr_mag	= meta::alg::sqr_magnitude(vb);
		constexpr auto mag		= meta::alg::magnitude(vb);
		constexpr auto norm		= meta::alg::normalize(vb);
		constexpr auto lerp		= meta::alg::lerp(va, vb, 0.5f);
		constexpr auto sqr		= meta::alg::sqrt<float>()(13.0f);
		constexpr auto det		= meta::alg::determinant(ma);
		constexpr auto inv		= meta::alg::inverse(ma);
		constexpr auto dot		= meta::alg::dot(ma, mb);
		constexpr auto pow		= meta::alg::pow(1.23, 10);
		constexpr auto fact		= meta::alg::fact(10);

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