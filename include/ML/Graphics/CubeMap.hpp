#ifndef _ML_CUBE_MAP_HPP_
#define _ML_CUBE_MAP_HPP_

#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Placeholder
	class ML_GRAPHICS_API CubeMap final
		: public I_Newable
	{
	public:
		CubeMap();
		CubeMap(const CubeMap & copy);
		~CubeMap();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CUBE_MAP_HPP_