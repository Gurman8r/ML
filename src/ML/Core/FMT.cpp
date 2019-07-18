#include <ML/Core/FMT.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ostream & FMT::operator()(Ostream & out) const
	{
#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			(((this->fg != FG::None) && (this->bg != BG::None))
				? ((uint16_t)this->fg | (uint16_t)this->bg) // Foreground and Background
				: ((this->fg != FG::None)
					? ((uint16_t)this->fg) // Foreground Only
					: ((this->bg != BG::None)
						? ((uint16_t)this->bg) // Background Only
						: ((uint16_t)FG::Normal | (uint16_t)BG::Black) // Default
						))));
#else 
		// Do the thing
#endif
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}