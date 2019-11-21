#ifndef _ML_SERVER_HPP_
#define _ML_SERVER_HPP_

#include <ML/Core/Singleton.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Network/NetInterface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetServer final : public Trackable, public NetInterface
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : int32_t { ID = 1 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NetServer();

		~NetServer() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		void onPacket(Packet const & value) override;

		bool start(Host const & host, uint32_t maxClients);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool running() const { return m_running; }

		inline auto maxClients() const -> uint32_t { return m_maxClients; }

		inline auto threadPriority() const -> int32_t { return m_threadPriority; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_running;
		uint32_t	m_maxClients;
		int32_t		m_threadPriority;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SERVER_HPP_