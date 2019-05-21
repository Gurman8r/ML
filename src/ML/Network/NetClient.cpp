#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetworkEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Network/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	NetClient::NetClient(EventSystem & eventSystem)
		: NetInterface(eventSystem)
		, m_connected(false)
	{
		eventSystem.addListener(NetworkEvent::EV_ClientRecievePacket, this);
	}

	NetClient::~NetClient()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	void NetClient::onEvent(const Event * value)
	{
		switch (*value)
		{
		case NetworkEvent::EV_ClientRecievePacket:
			if (auto ev = value->as<ClientRecievePacketEvent>())
			{
				Debug::log("CLIENT -> {0}", (*ev));
			}
			break;
		}
	}

	void NetClient::onPacket(const Packet & value)
	{
		switch (value.data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Debug::log("Connection Request Accepted");
			m_connected = true;
			break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Debug::logError("No Free Incoming Connections");
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			Debug::log("Disconnected");
			break;

		case ID_CONNECTION_LOST:
			Debug::logError("Connection Lost");
			break;

		case ML_CLIENT_RECIEVE:
			RakNet::BitStream bitStream(value.data, value.size, false);
			RakNet::RakString str;
			if (bitStream.Read(str))
			{
				eventSystem().fireEvent(ClientRecievePacketEvent(str.C_String()));
			}
			break;
		}
	}

	bool NetClient::connect(const IpAddress & addr, const String & pass)
	{
		if (m_peer)
		{
			switch (ML_PEER(m_peer)->Connect(
				addr.ToCString(),
				addr.port,
				pass.c_str(),
				(int32_t)pass.size()))
			{
			case RakNet::CONNECTION_ATTEMPT_STARTED:
				return Debug::log("CONNECTION_ATTEMPT_STARTED");

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

	/* * * * * * * * * * * * * * * * * * * * */
}