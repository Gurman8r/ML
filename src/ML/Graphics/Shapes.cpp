#include <ML/Graphics/Shapes.hpp>

namespace ml
{
	// Rect Quad
	/* * * * * * * * * * * * * * * * * * * * */
	Vertices Shapes::RectQuad::genQuadVerts(const FloatRect & r, const RectUVs & uv)
	{
		return
		{
		Vertex({ r.left(),  r.bottom()	},	Color::White,	{ uv[ 0], uv[ 1] }),
		Vertex({ r.left(),  r.top()		},	Color::White,	{ uv[ 2], uv[ 3] }),
		Vertex({ r.right(), r.top()		},	Color::White,	{ uv[ 4], uv[ 5] }),
		Vertex({ r.left(),  r.bottom()	},	Color::White,	{ uv[ 6], uv[ 7] }),
		Vertex({ r.right(), r.top()		},	Color::White,	{ uv[ 8], uv[ 9] }),
		Vertex({ r.right(), r.bottom()	},	Color::White,	{ uv[10], uv[11] }),
		};
	}

	Array<float, Shapes::RectQuad::Size> Shapes::RectQuad::genQuad(const FloatRect & r, const RectUVs & uv)
	{
		return
		{
			r.left(),  r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[ 0],	uv[ 1],
			r.left(),  r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[ 2],	uv[ 3],
			r.right(), r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[ 4],	uv[ 5],
			r.left(),  r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[ 6],	uv[ 7],
			r.right(), r.top(),		0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[ 8],	uv[ 9],
			r.right(), r.bottom(),	0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		uv[10], uv[11],
		};
	}

	Array<float, Shapes::RectQuad::Size> Shapes::RectQuad::genGlyphQuad(const FloatRect & r)
	{
		return genQuad(r, 
		{
			0.0f, 1.0f, // LB
			0.0f, 0.0f, // LT
			1.0f, 0.0f, // RT
			0.0f, 1.0f, // LB
			1.0f, 0.0f, // RT
			1.0f, 1.0f  // RB
		});
	}

	Array<float, Shapes::RectQuad::Size> Shapes::RectQuad::genSpriteQuad(const FloatRect & r)
	{
		return genQuad(r,
		{
			0.0f, 0.0f, // LB
			0.0f, 1.0f, // LT
			1.0f, 1.0f, // RT
			0.0f, 0.0f, // LB
			1.0f, 1.0f, // RT
			1.0f, 0.0f  // RB
		});
	}


	// Triangle
	/* * * * * * * * * * * * * * * * * * * * */
	const Vertices Shapes::Triangle::Vertices({
		Vertex({  0.0f,  0.5f, 0.0f }, Color::White, { 0.5f, 1.0f }), // top right
		Vertex({  0.5f, -0.5f, 0.0f }, Color::White, { 1.0f, 0.0f }), // bot right
		Vertex({ -0.5f, -0.5f, 0.0f }, Color::White, { 0.0f, 0.0f }), // bot left
	});
	const List<uint32_t> Shapes::Triangle::Indices({
		0, 1, 2
	});


	// Quad
	/* * * * * * * * * * * * * * * * * * * * */
	const Vertices Shapes::Quad::Vertices({
		Vertex({ +1.0f, +1.0f, 0.0f }, Color::White, { 1.0f, 1.0f }),
		Vertex({ +1.0f, -1.0f, 0.0f }, Color::White, { 1.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f, 0.0f }, Color::White, { 0.0f, 0.0f }),
		Vertex({ -1.0f, +1.0f, 0.0f }, Color::White, { 0.0f, 1.0f }),
	});
	const List<uint32_t> Shapes::Quad::Indices({
		0, 1, 3,
		1, 2, 3
	});


	// Cube
	/* * * * * * * * * * * * * * * * * * * * */
	const Vertices Shapes::Cube::Vertices({
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 1.0f, 1.0f }), // 0 front
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 1.0f, 0.0f }), // 1
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }), // 2
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }), // 3
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1.0f, 1.0f }), // 4 back
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1.0f, 0.0f }), // 5
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 0.0f, 0.0f }), // 6
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 0.0f, 1.0f }), // 7
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1.0f, 1.0f }), // 8 right
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1.0f, 0.0f }), // 9
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }), // 10
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }), // 11
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 1.0f, 1.0f }), // 12 left
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 1.0f, 0.0f }), // 13
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }), // 14
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }), // 15
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 1.0f, 1.0f }), // 16 top
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1.0f, 0.0f }), // 17
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }), // 18
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }), // 19
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 1.0f, 1.0f }), // 20 bottom
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1.0f, 0.0f }), // 21
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }), // 22
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }), // 23
	});
	const List<uint32_t> Shapes::Cube::Indices({
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
		20, 23, 22,
	});


	// Sky
	/* * * * * * * * * * * * * * * * * * * * */
	const List<float> Shapes::Sky::Vertices({ 
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
	});
}