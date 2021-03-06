#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetworkEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Network/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NetClient::NetClient()
		: m_connected{ false }
	{
		ML_EventSystem.addListener<ClientRecievePacketEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void NetClient::onEvent(Event const & value)
	{
		switch (*value)
		{
		case ClientRecievePacketEvent::ID: if (auto ev{ value.as<ClientRecievePacketEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Debug::logInfo("CLIENT -> {0}", ev.data);

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	void NetClient::onPacket(Packet const & value)
	{
		switch (value.data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Debug::logInfo("Connection Request Accepted");
			m_connected = true;
			break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Debug::logError("No Free Incoming Connections");
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			Debug::logInfo("Disconnected");
			break;

		case ID_CONNECTION_LOST:
			Debug::logError("Connection Lost");
			break;

		case (ID_USER_PACKET_ENUM + NetClient::ID):
			RakNet::BitStream bitStream(value.data, value.size, false);
			if (RakNet::RakString str; bitStream.Read(str))
			{
				ML_EventSystem.fireEvent<ClientRecievePacketEvent>(str.C_String());
			}
			break;
		}
	}

	bool NetClient::connect(Host const & host, String const & password)
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			switch (p->Connect(
				host.addr.str().c_str(), 
				host.port, 
				password.c_str(), 
				(int32_t)password.size()
			))
			{
			case RakNet::CONNECTION_ATTEMPT_STARTED:
				return Debug::logInfo("CONNECTION_ATTEMPT_STARTED");

			case RakNet::INVALID_PARAMETER:
				return Debug::logError("INVALID_PARAMETER");

			case RakNet::CANNOT_RESOLVE_DOMAIN_NAME:
				return Debug::logError("CANNOT_RESOLVE_DOMAIN_NAME");

			case RakNet::ALREADY_CONNECTED_TO_ENDPOINT:
				return Debug::logError("ALREADY_CONNECTED_TO_ENDPOINT");

			case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
				return Debug::logError("CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS");

			case RakNet::SECURITY_INITIALIZATION_FAILED:
				return Debug::logError("SECURITY_INITIALIZATION_FAILED");
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}