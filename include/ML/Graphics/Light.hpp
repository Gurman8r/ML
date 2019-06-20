#ifndef _ML_LIGHT_HPP_
#define _ML_LIGHT_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// Placeholder
	class ML_GRAPHICS_API Light final
		: public INewable
	{
	public:
		Light();
		Light(const vec4 & color);
		Light(const Light & copy);
		~Light();

	public:
		vec4 color;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIGHT_HPP_