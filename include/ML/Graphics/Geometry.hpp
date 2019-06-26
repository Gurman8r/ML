#ifndef _ML_GEOMETRY_HPP_
#define _ML_GEOMETRY_HPP_

#include <ML/Core/Rect.hpp>
#include <ML/Graphics/Vertices.hpp>

// Default meshes and rect quad generator functions
// Used to create builtin geometry

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace geo
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct rect_quad final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using contiguous_t	= typename Array<float, Vertex::Size * 6>;
			using texcoord_t	= typename Array<float, (contiguous_t::Size * 2)>;

			/* * * * * * * * * * * * * * * * * * * * */

			static constexpr contiguous_t create(const FloatRect & r, const texcoord_t & uv)
			{
				return
				{
					r.left(),  r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[ 0],	uv[ 1],
					r.left(),  r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[ 2],	uv[ 3],
					r.right(), r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[ 4],	uv[ 5],
					r.left(),  r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[ 6],	uv[ 7],
					r.right(), r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[ 8],	uv[ 9],
					r.right(), r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,	uv[10], uv[11],
				};
			}

			static constexpr contiguous_t glyphQuad(const FloatRect & r)
			{
				return create(r,
				{
					0.0f, 1.0f, // LB
					0.0f, 0.0f, // LT
					1.0f, 0.0f, // RT
					0.0f, 1.0f, // LB
					1.0f, 0.0f, // RT
					1.0f, 1.0f  // RB
				});
			}

			static constexpr contiguous_t spriteQuad(const FloatRect & r)
			{
				return create(r,
				{
					0.0f, 0.0f, // LB
					0.0f, 1.0f, // LT
					1.0f, 1.0f, // RT
					0.0f, 0.0f, // LB
					1.0f, 1.0f, // RT
					1.0f, 0.0f  // RB
				});
			}

			/* * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		namespace impl
		{
			template <
				size_t V, size_t I
			> struct static_mesh
			{
				struct num final
				{
					static constexpr size_t vertices	{ V };
					static constexpr size_t indices		{ I };
					static constexpr size_t contiguous	{ vertices * Vertex::Size };
				};

				using vertices_t	= typename Array<Vertex, num::vertices>;
				using indices_t		= typename Array<uint32_t, num::indices>;
				using contiguous_t	= typename Array<float, num::contiguous>;

			protected:
				static constexpr contiguous_t Contiguous(const vertices_t & value)
				{
					contiguous_t temp { meta::uninit };
					for (size_t i = 0; i < temp.size(); i++)
					{
						temp[i] = (value[i / Vertex::Size])[i % Vertex::Size];
					}
					return temp;
				}
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct tri final : public impl::static_mesh<3, 3>
		{
			static constexpr vertices_t vertices
			{
				Vertex { {  0.0f,  0.5f, 0.0f }, color::white, { 0.5f, 1.0f } },
				Vertex { {  0.5f, -0.5f, 0.0f }, color::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, 0.0f }, color::white, { 0.0f, 0.0f } },
			};
			static constexpr indices_t indices
			{
				0, 1, 2
			};
			static constexpr contiguous_t contiguous
			{
				static_mesh::Contiguous(vertices)
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct quad final : public impl::static_mesh<4, 6>
		{
			enum
			{
				Size = num::contiguous
			};

			static constexpr vertices_t vertices
			{
				Vertex { { +1.0f, +1.0f, 0.0f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +1.0f, -1.0f, 0.0f }, color::white, { 1.0f, 0.0f } },
				Vertex { { -1.0f, -1.0f, 0.0f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -1.0f, +1.0f, 0.0f }, color::white, { 0.0f, 1.0f } },
			};
			static constexpr indices_t indices
			{
				0, 1, 3,
				1, 2, 3
			};
			static constexpr contiguous_t contiguous
			{
				static_mesh::Contiguous(vertices)
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct cube final : public impl::static_mesh<24, 36>
		{
			static constexpr vertices_t vertices
			{
				Vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 0.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
				Vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
				Vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
				Vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
				Vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
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
			static constexpr contiguous_t contiguous
			{
				static_mesh::Contiguous(vertices)
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct sky final
		{
			static constexpr Array<float, 108> contiguous
			{
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GEOMETRY_HPP_