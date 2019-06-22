#ifndef _ML_SHAPES_HPP_
#define _ML_SHAPES_HPP_

#include <ML/Core/Rect.hpp>
#include <ML/Graphics/Vertices.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Shapes final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API RectQuad final
		{
			enum 
			{ 
				Size	= (Vertex::Size * 6), 
				NumUV	= (Size * 2),
			};

			using RectUVs = typename Array<float, NumUV>;

			static Vertices genQuadVerts(const FloatRect & r, const RectUVs & uv);

			static Array<float, Size> genQuad(const FloatRect & r, const RectUVs & uv);
			static Array<float, Size> genGlyphQuad(const FloatRect & r);
			static Array<float, Size> genSpriteQuad(const FloatRect & r);
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API Triangle final
		{
			const static Vertices	Vertices;
			const static List<uint32_t>	Indices;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API Quad final
		{
			const static Vertices	Vertices;
			const static List<uint32_t>	Indices;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API Cube final
		{
			const static Vertices	Vertices;
			const static List<uint32_t>	Indices;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API Sky final
		{
			const static List<float> Vertices;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHAPES_HPP_