#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/Singleton.hpp>

#define ML_EventSystem ::ml::EventSystem::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API EventSystem final : public Singleton<EventSystem>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		EventListener * addListener(const int32_t type, EventListener * listener);
		
		bool fireEvent(Event const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool removeListener(int32_t const & type, EventListener * listener);
		
		bool removeListenerFromAllEvents(EventListener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev> inline auto addListener(EventListener * listener)
		{
			return addListener(Ev::ID, listener);
		}

		template <class Ev, class ... Args> inline bool fireEvent(Args && ... args)
		{
			return fireEvent(Ev{ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<EventSystem>;
		EventSystem() {}
		~EventSystem() {}
		std::multimap<int32_t, EventListener *> m_listeners;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_