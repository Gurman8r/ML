#ifndef _ML_I_EVENT_HPP_
#define _ML_I_EVENT_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Event : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			MAX_LIBRARY_EVENTS = 64,

			EV_INVALID	= -1,
			EV_AUDIO	= (MAX_LIBRARY_EVENTS * 1),
			EV_CORE		= (MAX_LIBRARY_EVENTS * 2),
			EV_EDITOR	= (MAX_LIBRARY_EVENTS * 3),
			EV_ENGINE	= (MAX_LIBRARY_EVENTS * 4),
			EV_GRAPHICS = (MAX_LIBRARY_EVENTS * 5),
			EV_NETWORK	= (MAX_LIBRARY_EVENTS * 6),
			EV_SCRIPT	= (MAX_LIBRARY_EVENTS * 7),
			EV_WINDOW	= (MAX_LIBRARY_EVENTS * 8),

			MAX_EVENT_ID
		};

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr Event(const int32_t id) : m_id(id) {}

		constexpr operator int32_t() const { return m_id; }

		/* * * * * * * * * * * * * * * * * * * * */

		template <class Derived> 
		inline const Derived * as() const
		{
			return static_cast<const Derived *>(this);
		}

		template <class Derived>
		inline Derived * as()
		{
			return static_cast<Derived *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private: const int32_t m_id;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		int32_t EV_ID
	> struct I_Event : public Event
	{
		enum : int32_t { ID = EV_ID };

		constexpr I_Event() : Event(ID) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_HPP_