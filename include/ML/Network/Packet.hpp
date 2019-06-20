#ifndef _ML_PACKET_HPP_
#define _ML_PACKET_HPP_

#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Packet final
		: public INewable
	{
		Host	addr;
		GUID		guid;
		uint32_t	size;
		uint8_t *	data;

		Packet();

		Packet(const Host & addr, const GUID & guid, uint32_t size, uint8_t * data);

		Packet(const Packet & copy);

		void serialize(OStream & out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PACKET_HPP_