#ifndef _ML_CUBE_MAP_HPP_
#define _ML_CUBE_MAP_HPP_

#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Placeholder
	struct ML_GRAPHICS_API CubeMap final
		: public I_Newable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CubeMap();
		CubeMap(const CubeMap & copy);
		~CubeMap();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		// todo...

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CUBE_MAP_HPP_