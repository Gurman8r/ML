#ifndef _ML_I_EVENT_HPP_
#define _ML_I_EVENT_HPP_

#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Event
		: public ITrackable
		, public INonCopyable
	{
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

		virtual ~Event() {}

		/* * * * * * * * * * * * * * * * * * * * */

		virtual int32_t getEventID() const = 0;

		inline operator int32_t() const 
		{
			return getEventID();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline const T * as() const
		{
			return dynamic_cast<const T *>(this);
		}

		template <
			class T
		> inline T * as()
		{
			return dynamic_cast<T *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <int32_t _ID>
	struct IEvent : public Event
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum { ID = _ID };

		virtual ~IEvent() {}

		inline int32_t getEventID() const override
		{
			return _ID;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_EVENT_HPP_