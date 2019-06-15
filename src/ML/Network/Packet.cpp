#include <ML/Network/Packet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Packet::Packet()
		: Packet(Host(), GUID(), 0, NULL)
	{
	}

	Packet::Packet(const Host & addr, const GUID & guid, uint32_t size, uint8_t * data)
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

	inline void Packet::serialize(OStream & out) const
	{
		out << addr << " "
			<< guid << " "
			<< size << " "
			<< data << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * */
}