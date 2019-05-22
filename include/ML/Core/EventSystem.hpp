#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <ML/Core/IEventListener.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API EventSystem final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using multimap_type = typename MultiMap<int32_t, IEventListener *>;
		using iterator = typename multimap_type::iterator;

	public:
		EventSystem();
		~EventSystem();

	public:
		void addListener(const int32_t type, IEventListener * listener);
		void fireEvent(const Event & ev);

	public:
		void removeListener(const int32_t & type, IEventListener * listener);
		void removeListenerFromAllEvents(IEventListener * listener);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		multimap_type m_listeners;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_