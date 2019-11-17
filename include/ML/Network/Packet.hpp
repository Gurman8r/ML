#ifndef _ML_PACKET_HPP_
#define _ML_PACKET_HPP_

#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Packet final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Host		host;
		GUID		guid;
		uint32_t	size;
		uint8_t *	data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Packet(const Host & addr, const GUID & guid, uint32_t size, uint8_t * data)
			: host(addr)
			, guid(guid)
			, size(size)
			, data(data)
		{
		}

		constexpr Packet(const Packet & copy)
			: Packet{ copy.host, copy.guid, copy.size, copy.data }
		{
		}

		constexpr Packet()
			: Packet{ {}, {}, 0, nullptr }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const Packet & value)
		{
			return out
				<< value.host << " "
				<< value.guid << " "
				<< value.size << " "
				<< value.data << " ";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const Packet & other)
		{

			return
				this->host == other.host &&
				this->guid == other.guid &&
				this->size == other.size &&
				this->data == other.data;
		}

		constexpr bool operator!=(const Packet & other)
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const Packet & other)
		{
			return
				this->host < other.host &&
				this->guid < other.guid &&
				this->size < other.size &&
				this->data < other.data;
		}

		constexpr bool operator>(const Packet & other)
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(const Packet & other)
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(const Packet & other)
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PACKET_HPP_