#ifndef _ML_I_EVENT_HPP_
#define _ML_I_EVENT_HPP_

#include <ML/Core/ITrackable.hpp>
#include <ML/Core/Preprocessor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Event : public ITrackable, public INonCopyable
	{
		const int32_t m_id;

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			MAX_LIBRARY_EVENTS = 64,

			EV_INVALID	= -1,
			EV_AUDIO	= (MAX_LIBRARY_EVENTS * 1),
			EV_EDITOR	= (MAX_LIBRARY_EVENTS * 2),
			EV_ENGINE	= (MAX_LIBRARY_EVENTS * 3),
			EV_GRAPHICS = (MAX_LIBRARY_EVENTS * 4),
			EV_NETWORK	= (MAX_LIBRARY_EVENTS * 5),
			EV_SCRIPT	= (MAX_LIBRARY_EVENTS * 6),
			EV_WINDOW	= (MAX_LIBRARY_EVENTS * 7),

			MAX_EVENT_ID
		};

		/* * * * * * * * * * * * * * * * * * * * */

		Event(const int32_t id) : m_id(id) {}

		inline operator int32_t() const
		{
			return m_id;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <class Derived> 
		inline const Derived * as() const
		{
			ML_assert_is_base_of(Event, Derived);
			return dynamic_cast<const Derived *>(this);
		}

		template <class Derived>
		inline Derived * as()
		{
			ML_assert_is_base_of(Event, Derived);
			return dynamic_cast<Derived *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <int32_t _ID>
	struct IEvent : public Event
	{
		enum { ID = _ID };

		IEvent() : Event(_ID) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_HPP_