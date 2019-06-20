#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

#include <ML/Window/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Cursor final
	{
		enum Mode : uint32_t
		{
			Normal = 0x34001,
			Hidden,
			Disabled,
		};

		enum Standard : uint32_t
		{
			Arrow = 0x36001, 
			IBeam, 
			Crosshair,
			Hand,
			HResize,
			VResize,
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CURSOR_HPP_