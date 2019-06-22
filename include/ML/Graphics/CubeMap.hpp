#ifndef _ML_CUBE_MAP_HPP_
#define _ML_CUBE_MAP_HPP_

#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Placeholder
	struct ML_GRAPHICS_API CubeMap final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CubeMap();
		CubeMap(const CubeMap & copy);
		~CubeMap();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		// todo...

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CUBE_MAP_HPP_