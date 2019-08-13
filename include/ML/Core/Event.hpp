#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Event : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			EV_SCRIPT	= (MAX_LIBRARY_EVENTS * 7),
			EV_WINDOW	= (MAX_LIBRARY_EVENTS * 8),
			EV_CUSTOM	= (MAX_LIBRARY_EVENTS * 9),
		};

		constexpr Event(const int32_t id) 
			: m_id(id) 
		{
		}

		constexpr const int32_t & operator*() const
		{ 
			return m_id;
		}

		template <class T> inline T * as()
		{
			return static_cast<T *>(this);
		}

		template <class T> inline const T * as() const
		{
			return static_cast<const T *>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: const int32_t m_id;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_