#include <ML/Core/FMT.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	Ostream & FMT::operator()(Ostream & out) const
	{
		const uint16_t value = 
			(((this->fg != FG::None) && (this->bg != BG::None))
				? ((uint16_t)this->fg | (uint16_t)this->bg)
				: ((this->fg != FG::None)
					? ((uint16_t)this->fg)
					: ((this->bg != BG::None)
						? ((uint16_t)this->bg)
						: ((uint16_t)FG::Normal | (uint16_t)BG::Black)
						)
					)
				);

#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
#else 

#endif
		return out;
	}
}