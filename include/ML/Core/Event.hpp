#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			MAX_LIBRARY_EVENTS = 64, // The number of events reserved for each library
			
			EV_INVALID	= (-1),
			EV_AUDIO	= (MAX_LIBRARY_EVENTS * 1),
			EV_CORE		= (MAX_LIBRARY_EVENTS * 2),
			EV_EDITOR	= (MAX_LIBRARY_EVENTS * 3),
			EV_ENGINE	= (MAX_LIBRARY_EVENTS * 4),
			EV_GRAPHICS = (MAX_LIBRARY_EVENTS * 5),
			EV_NETWORK	= (MAX_LIBRARY_EVENTS * 6),
			EV_WINDOW	= (MAX_LIBRARY_EVENTS * 7),
			EV_CUSTOM	= (MAX_LIBRARY_EVENTS * 8),
		};

		constexpr Event(int32_t value) : m_id { value } {}

		constexpr const int32_t & operator*() const { return m_id; }

		constexpr operator bool() const { return (m_id > EV_INVALID); }

		template <class T> inline auto as() { return static_cast<T *>(this); }

		template <class T> inline auto as() const { return static_cast<const T *>(this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: const int32_t m_id;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <int32_t _ID> struct I_Event : public Event
	{
		enum : int32_t { ID = _ID };

		static constexpr int32_t id { _ID };

		constexpr I_Event() : Event { _ID } {}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_