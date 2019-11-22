#include <ML/Network/NetInterface.hpp>
#include <ML/Network/NetworkEvents.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Network/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NetInterface::NetInterface()
		: m_peer{ nullptr }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool NetInterface::setup()
	{
		return !m_peer && (m_peer = RakNet::RakPeerInterface::GetInstance());
	}

	bool NetInterface::dispose()
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			RakNet::RakPeerInterface::DestroyInstance(p);
			
			return true;
		}
		return false;
	}

	void * NetInterface::receive()
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			return p->Receive();
		}
		return nullptr;
	}

	void * NetInterface::receive(void * value)
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) }; p && value)
		{
			p->DeallocatePacket(static_cast<RakNet::Packet *>(value));
		}
		return this->receive();
	}

	void NetInterface::poll()
	{
		for (void * pkt{ this->receive() }; pkt; pkt = this->receive(pkt))
		{
			this->onPacket({ {
				static_cast<RakNet::Packet *>(pkt)->systemAddress.ToString(),
				static_cast<RakNet::Packet *>(pkt)->systemAddress.GetPort() },
				static_cast<RakNet::Packet *>(pkt)->guid.g,
				static_cast<RakNet::Packet *>(pkt)->length,
				static_cast<RakNet::Packet *>(pkt)->data
			});
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t NetInterface::send(GUID const & guid, String const & data, SendSettings const & settings)
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			RakNet::BitStream bitStream{};
			bitStream.Write(data.c_str());
			return p->Send(
				&bitStream,
				(PacketPriority)settings.priority,
				(PacketReliability)settings.reliability,
				settings.ordering,
				p->GetSystemAddressFromGuid(RakNet::RakNetGUID{ guid }),
				settings.broadcast,
				settings.receiptNumber
			);
		}
		return 0;
	}

	uint32_t NetInterface::send(Host const & host, String const & data, SendSettings const & settings)
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			RakNet::BitStream bitStream{};
			bitStream.Write(data.c_str());
			return p->Send(
				&bitStream,
				(PacketPriority)settings.priority,
				(PacketReliability)settings.reliability,
				settings.ordering,
				RakNet::SystemAddress{ host.addr.str().c_str(), host.port },
				settings.broadcast,
				settings.receiptNumber
			);
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GUID NetInterface::getMyGUID() const
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			return { p->GetMyGUID().g };
		}
		return GUID{};
	}

	GUID NetInterface::getGUIDFromAddress(Host const & value) const
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			return { p->GetGuidFromSystemAddress({ value.addr.str().c_str(), value.port }).g };
		}
		return GUID{};
	}

	Host NetInterface::getMyAddress() const
	{
		return getAddressFromGUID(getMyGUID());
	}

	Host NetInterface::getAddressFromGUID(GUID const & value) const
	{
		if (auto p{ static_cast<RakNet::RakPeerInterface *>(m_peer) })
		{
			const auto addr { p->GetSystemAddressFromGuid(RakNet::RakNetGUID(value)) };
			return { addr.ToString(), addr.GetPort() };
		}
		return Host{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}