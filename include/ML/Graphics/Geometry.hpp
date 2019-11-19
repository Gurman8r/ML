#ifndef _ML_GEOMETRY_HPP_
#define _ML_GEOMETRY_HPP_

#include <ML/Core/Rect.hpp>
#include <ML/Graphics/Vertex.hpp>

// Default meshes and rect quad generator functions
// Used to create builtin geometry

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace geo
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Rect-Quad
		
		using rect_quad = typename Array<float_t, Vertex::Size * 6>;
		
		using rect_uvs	= typename Array<float_t, rect_quad::Size * 2>;

		static constexpr rect_quad rectQuad(FloatRect const & rq, rect_uvs const & uv)
		{
			return
			{
				// Position						// Normal					// Texcoord
				rq.left(),  rq.bot(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[ 0],	uv[ 1],
				rq.left(),  rq.top(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[ 2],	uv[ 3],
				rq.right(), rq.top(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[ 4],	uv[ 5],
				rq.left(),  rq.bot(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[ 6],	uv[ 7],
				rq.right(), rq.top(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[ 8],	uv[ 9],
				rq.right(), rq.bot(), 0.f,		1.0f, 1.0f, 1.0f, 1.0f,		uv[10], uv[11],
			};
		}

		static constexpr rect_quad glyphQuad(FloatRect const & rq)
		{
			return rectQuad(rq,
			{
				0.0f, 1.0f, // Left  Bottom
				0.0f, 0.0f, // Left  Top
				1.0f, 0.0f, // Right Top
				0.0f, 1.0f, // Left  Bottom
				1.0f, 0.0f, // Right Top
				1.0f, 1.0f  // Right Bottom
			});
		}

		static constexpr rect_quad spriteQuad(FloatRect const & rq)
		{
			return rectQuad(rq,
			{
				0.0f, 0.0f, // Left  Bottom
				0.0f, 1.0f, // Left  Top
				1.0f, 1.0f, // Right Top
				0.0f, 0.0f, // Left  Bottom
				1.0f, 1.0f, // Right Top
				1.0f, 0.0f  // Right Bottom
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		namespace impl
		{
			template <size_t V, size_t I> struct static_mesh
			{
				using vertices_t	= typename Array<Vertex, V>;
				using indices_t		= typename Array<uint32_t, I>;
				using contiguous_t	= typename Array<float_t, V * Vertex::Size>;

				static constexpr contiguous_t make_contiguous(vertices_t const & value)
				{
					contiguous_t temp { 0 };
					for (size_t i = 0; i < temp.size(); i++)
					{
						temp[i] = (value[i / Vertex::Size])[i % Vertex::Size];
					}
					return temp;
				}
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct triangle_static final : public impl::static_mesh<3, 3>
		{
			static constexpr vertices_t vertices
			{
				Vertex { {  0.0f,  0.5f, 0.0f }, Colors::white, { 0.5f, 1.0f } },
				Vertex { {  0.5f, -0.5f, 0.0f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, 0.0f }, Colors::white, { 0.0f, 0.0f } },
			};
			static constexpr indices_t indices
			{
				0, 1, 2
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct quad_static final : public impl::static_mesh<4, 6>
		{
			static constexpr vertices_t vertices
			{
				Vertex { { +1.0f, +1.0f, 0.0f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +1.0f, -1.0f, 0.0f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f, 0.0f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -1.0f, +1.0f, 0.0f }, Colors::white, { 0.0f, 1.0f } },
			};
			static constexpr indices_t indices
			{
				0, 1, 3,
				1, 2, 3
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct cube_static final : public impl::static_mesh<24, 36>
		{
			static constexpr vertices_t vertices
			{
				Vertex { { +0.5f, +0.5f, -0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, Colors::white, { 0.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, Colors::white, { 0.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { +0.5f, +0.5f, -0.5f }, Colors::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, Colors::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, +0.5f, -0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, Colors::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, Colors::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, Colors::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, Colors::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, Colors::white, { 0.0f, 1.0f } },
			};
			static constexpr indices_t indices
			{
				0, 1, 3,
				1, 2, 3,
				7, 5, 4,
				7, 6, 5,
				8, 9, 11,
				9, 10, 11,
				15, 13, 12,
				15, 14, 13,
				17, 18, 16,
				18, 19, 16,
				22, 21, 20,
				20, 23, 22
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct skybox_static final : public impl::static_mesh<36, 0>
		{
			static constexpr vertices_t vertices
			{
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f,  1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f, -1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
				Vertex { {  1.0f, -1.0f,  1.0f }, Colors::white, vec2 { 0.0f, 0.0f } },
			};
			static constexpr indices_t indices {};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GEOMETRY_HPP_