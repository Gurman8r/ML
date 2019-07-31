#ifndef _ML_SERVER_HPP_
#define _ML_SERVER_HPP_

#include <ML/Core/I_Singleton.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Network/NetInterface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetServer final
		: public I_Newable
		, public NetInterface
	{
	public:
		enum { ID = 1 };

	public:
		explicit NetServer(EventSystem & eventSystem);
		~NetServer();

	public:
		void onEvent(const Event & value) override;

		void onPacket(const Packet & value) override;

		bool start(const Host & host, uint32_t maxClients);

	public:
		inline bool		running()		const { return m_running; }
		inline uint32_t maxClients()	const { return m_maxClients; }

	private:
		bool		m_running;
		uint32_t	m_maxClients;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SERVER_HPP_