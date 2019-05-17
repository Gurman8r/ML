#include <ML/Network/IpAddress.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	IpAddress::IpAddress()
		: IpAddress(String(), 0)
	{
	}

	IpAddress::IpAddress(const String & addr)
		: IpAddress(addr, NULL)
	{
	}

	IpAddress::IpAddress(const String & addr, uint16_t port)
		: addr(addr)
		, port(port)
	{
	}

	IpAddress::IpAddress(const IpAddress & copy)
		: IpAddress(copy.addr, copy.port)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline IpAddress::operator bool() const
	{
		return (addr && port);
	}

	inline void IpAddress::serialize(std::ostream & out) const
	{
		out << addr;
	}

	inline bool IpAddress::equals(const IpAddress & copy) const
	{
		return
			addr == copy.addr &&
			port == copy.port;
	}

	inline bool IpAddress::lessThan(const IpAddress & copy) const
	{
		return
			addr < copy.addr &&
			port < copy.port;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}