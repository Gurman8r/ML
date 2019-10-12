#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <ML/Core/I_EventListener.hpp>
#include <ML/Core/I_Newable.hpp>

#define ML_EventSystem ::ml::EventSystem::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Event;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API EventSystem final : public I_Singleton<EventSystem>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using map_type	= typename std::multimap<int32_t, I_EventListener *>;
		using iterator	= typename map_type::iterator;
		using pair_type = typename std::pair<iterator, iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		I_EventListener * addListener(const int32_t type, I_EventListener * listener);

		template <class Ev> inline auto addListener(I_EventListener * listener)
		{
			return addListener(Ev::ID, listener);
		}
		
		bool fireEvent(const Event & value);

		template <class Ev, class ... Args> inline bool fireEvent(Args && ... args)
		{
			return fireEvent(Ev { std::forward<Args>(args)... });
		}

		bool removeListener(const int32_t & type, I_EventListener * listener);
		
		bool removeListenerFromAllEvents(I_EventListener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<EventSystem>;
		EventSystem() {}
		~EventSystem() {}
		map_type m_listeners;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_