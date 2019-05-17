#include <ML/Network/Packet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Packet::Packet()
		: Packet(IpAddress(), GUID(), 0, NULL)
	{
	}

	Packet::Packet(const IpAddress & addr, const GUID & guid, uint32_t size, uint8_t * data)
		: addr(addr)
		, guid(guid)
		, size(size)
		, data(data)
	{
	}

	Packet::Packet(const Packet & copy)
		: Packet(copy.addr, copy.guid, copy.size, copy.data)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline void Packet::serialize(std::ostream & out) const
	{
		out << addr << " "
			<< guid << " "
			<< size << " "
			<< data << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * */
}