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
		
		ptr_t<EventListener> addListener(const int32_t type, ptr_t<EventListener> listener);
		
		bool fireEvent(const Event & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool removeListener(const int32_t & type, ptr_t<EventListener> listener);
		
		bool removeListenerFromAllEvents(ptr_t<EventListener> listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev> inline auto addListener(ptr_t<EventListener> listener)
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
		std::multimap<int32_t, ptr_t<EventListener>> m_listeners;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_