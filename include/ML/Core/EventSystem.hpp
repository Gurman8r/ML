#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/INewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API EventSystem final
		: public INewable
		, public INonCopyable
	{
	public:
		using map_type = typename MultiMap<int32_t, EventListener *>;
		using iterator = typename map_type::iterator;

	public:
		EventSystem();
		~EventSystem();

	public:
		EventListener * addListener(const int32_t type, EventListener * listener);
		bool fireEvent(const Event & ev);

	public:
		bool removeListener(const int32_t & type, EventListener * listener);
		bool removeListenerFromAllEvents(EventListener * listener);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		map_type m_listeners;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_