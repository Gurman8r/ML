#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct ML_GRAPHICS_API Color final
		: public ITrackable
	{
		const static vec4 Clear;
		const static vec4 White;
		const static vec4 Black;
		const static vec4 Gray;

		const static vec4 Red;
		const static vec4 Blue;
		const static vec4 Green;
		
		const static vec4 Cyan;
		const static vec4 Yellow;
		const static vec4 LightYellow;
		const static vec4 Magenta;

		const static vec4 Violet;
		const static vec4 Lime;

		const static vec4 Orange;
		const static vec4 Fuchsia;

		const static vec4 Aqua;
		const static vec4 Azure;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_