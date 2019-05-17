#ifndef _ML_CLIENT_HPP_
#define _ML_CLIENT_HPP_

#include <ML/Core/ISingleton.hpp>
#include <ML/Network/NetInterface.hpp>

#define ML_NetClient ml::NetClient::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetClient final
		: public ITrackable
		, public NetInterface
		, public ISingleton<NetClient>
	{
		friend class ISingleton<NetClient>;

	private:
		NetClient();
		~NetClient();

	public:
		void onEvent(const IEvent * value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const IpAddress & addr, const String & pass = String());

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_HPP_