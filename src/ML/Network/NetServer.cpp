#include <ML/Network/NetServer.hpp>
#include <ML/Network/NetworkEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Network/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NetServer::NetServer()
		: m_running{ false }
	{
		ML_EventSystem.addListener<ServerRecievePacketEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void NetServer::onEvent(Event const & value)
	{
		switch (*value)
		{
		case ServerRecievePacketEvent::ID: if (auto ev{ value.as<ServerRecievePacketEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Debug::logInfo("SERVER -> {0}", ev.data);

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	void NetServer::onPacket(Packet const & value)
	{
		switch (value.data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			Debug::logInfo("Remote Disconnection");
			break;
	
		case ID_REMOTE_CONNECTION_LOST:
			Debug::logError("Remote Connection List");
			break;
	
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			Debug::logInfo("Remote New Incoming Connection");
			break;
	
		case ID_NEW_INCOMING_CONNECTION:
			Debug::logInfo("New Incoming Connection");
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
	
		case (ID_USER_PACKET_ENUM + NetServer::ID):
			RakNet::BitStream bitStream(value.data, value.size, false);
			RakNet::RakString str;
			if (bitStream.Read(str))
			{
				ML_EventSystem.fireEvent<ServerRecievePacketEvent>(str.C_String());
			}
			break;
		}
	}
	
	bool NetServer::start(Host const & host, uint32_t maxClients)
	{
		if (m_peer && (m_maxClients = maxClients))
		{
			RakNet::SocketDescriptor socket{ host.port, host.addr.str().c_str() };
			switch (ML_PEER(m_peer)->Startup(m_maxClients, &socket, 1))
			{
			case RakNet::RAKNET_STARTED:
				m_running = true;
				return Debug::logInfo("RAKNET_STARTED");

			case RakNet::RAKNET_ALREADY_STARTED:
				return Debug::logWarning("RAKNET_ALREADY_STARTED");

			case RakNet::INVALID_SOCKET_DESCRIPTORS:
				return Debug::logError("INVALID_SOCKET_DESCRIPTORS");

			case RakNet::INVALID_MAX_CONNECTIONS:
				return Debug::logError("INVALID_MAX_CONNECTIONS");

			case RakNet::SOCKET_FAMILY_NOT_SUPPORTED:
				return Debug::logError("SOCKET_FAMILY_NOT_SUPPORTED");

			case RakNet::SOCKET_PORT_ALREADY_IN_USE:
				return Debug::logError("SOCKET_PORT_ALREADY_IN_USE");

			case RakNet::SOCKET_FAILED_TO_BIND:
				return Debug::logError("SOCKET_FAILED_TO_BIND");

			case RakNet::SOCKET_FAILED_TEST_SEND:
				return Debug::logError("SOCKET_FAILED_TEST_SEND");

			case RakNet::PORT_CANNOT_BE_ZERO:
				return Debug::logError("PORT_CANNOT_BE_ZERO");

			case RakNet::FAILED_TO_CREATE_NETWORK_THREAD:
				return Debug::logError("FAILED_TO_CREATE_NETWORK_THREAD");

			case RakNet::COULD_NOT_GENERATE_GUID:
				return Debug::logError("COULD_NOT_GENERATE_GUID");

			case RakNet::STARTUP_OTHER_FAILURE:
			default:
				return Debug::logError("STARTUP_OTHER_FAILURE");
			}
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}