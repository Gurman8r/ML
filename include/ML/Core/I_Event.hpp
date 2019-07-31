#ifndef _ML_I_EVENT_HPP_
#define _ML_I_EVENT_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <int32_t ID> struct I_Event : public Event
	{
		enum : int32_t { ID = ID };

		constexpr I_Event() : Event(ID) {}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_HPP_