#include <ML/Network/NetInterface.hpp>
#include <ML/Network/NetworkEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Network/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	NetInterface::NetInterface(EventSystem & eventSystem)
		: EventListener(eventSystem)
		, m_peer(NULL)
	{
	}
	
	NetInterface::~NetInterface()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool NetInterface::setup()
	{
		return ((!m_peer) && (m_peer = ML_PEER(RakNet::RakPeerInterface::GetInstance())));
	}

	bool NetInterface::dispose()
	{
		if (m_peer)
		{
			RakNet::RakPeerInterface::DestroyInstance(ML_PEER(m_peer));
		}
		return (!m_peer);
	}

	void NetInterface::poll()
	{
		for (void * packet = ML_PEER(m_peer)->Receive();
			(packet != NULL);
			(ML_PEER(m_peer)->DeallocatePacket(ML_PACKET(packet))),
			(packet = ML_PEER(m_peer)->Receive()))
		{
			onPacket(Packet({
					ML_PACKET(packet)->systemAddress.ToString(),
					ML_PACKET(packet)->systemAddress.GetPort()
				},
				ML_PACKET(packet)->guid.g,
				ML_PACKET(packet)->length,
				ML_PACKET(packet)->data
			));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t NetInterface::send(const GUID & guid, const String & data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data.c_str());
		return ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			ML_PEER(m_peer)->GetSystemAddressFromGuid(RakNet::RakNetGUID(guid)),
			settings.broadcast,
			settings.receiptNumber);
	}

	uint32_t NetInterface::send(const Host & addr, const String & data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data.c_str());
		return ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			RakNet::SystemAddress(addr.addr.c_str(), addr.port),
			settings.broadcast,
			settings.receiptNumber);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	GUID NetInterface::getMyGUID() const
	{
		return GUID(
			ML_PEER(m_peer)->GetMyGUID().g
		);
	}

	GUID NetInterface::getGUIDFromAddress(const Host & value) const
	{
		return GUID(
			ML_PEER(m_peer)->GetGuidFromSystemAddress(
				RakNet::SystemAddress(value.addr.c_str(), value.port)
			).g
		);
	}

	Host NetInterface::getMyAddress() const
	{
		return getAddressFromGUID(getMyGUID());
	}

	Host NetInterface::getAddressFromGUID(const GUID & value) const
	{
		const RakNet::SystemAddress addr = ML_PEER(m_peer)->GetSystemAddressFromGuid(
			RakNet::RakNetGUID(value)
		);
		return Host(
			addr.ToString(),
			addr.GetPort()
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}