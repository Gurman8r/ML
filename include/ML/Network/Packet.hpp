#ifndef _ML_PACKET_HPP_
#define _ML_PACKET_HPP_

#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Packet final : public NonNewable
	{
		Host		host;
		GUID		guid;
		uint32_t	size;
		uint8_t *	data;

		constexpr Packet(const Host & addr, const GUID & guid, uint32_t size, uint8_t * data)
			: host(addr)
			, guid(guid)
			, size(size)
			, data(data)
		{
		}

		constexpr Packet(const Packet & copy)
			: Packet(copy.host, copy.guid, copy.size, copy.data)
		{
		}

		constexpr Packet()
			: Packet(Host(), GUID(), 0, nullptr)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Packet & value)
	{
		return out 
			<< value.host << " "
			<< value.guid << " "
			<< value.size << " "
			<< value.data << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const Packet & lhs, const Packet & rhs)
	{
		return
			lhs.host == rhs.host &&
			lhs.guid == rhs.guid &&
			lhs.size == rhs.size &&
			lhs.data == rhs.data;
	}

	constexpr bool operator!=(const Packet & lhs, const Packet & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const Packet & lhs, const Packet & rhs)
	{
		return
			lhs.host < rhs.host &&
			lhs.guid < rhs.guid &&
			lhs.size < rhs.size &&
			lhs.data < rhs.data;
	}

	constexpr bool operator>(const Packet & lhs, const Packet & rhs)
	{
		return !(lhs < rhs);
	}

	constexpr bool operator<=(const Packet & lhs, const Packet & rhs)
	{
		return (lhs == rhs) || (lhs < rhs);
	}

	constexpr bool operator>=(const Packet & lhs, const Packet & rhs)
	{
		return (lhs == rhs) || (lhs > rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PACKET_HPP_