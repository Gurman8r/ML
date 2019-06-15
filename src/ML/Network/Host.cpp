#include <ML/Network/Host.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Host::Host()
		: Host(String(), 0)
	{
	}

	Host::Host(const String & addr)
		: Host(addr, NULL)
	{
	}

	Host::Host(const String & addr, uint16_t port)
		: addr(addr)
		, port(port)
	{
	}

	Host::Host(const Host & copy)
		: Host(copy.addr, copy.port)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline Host::operator bool() const
	{
		return (addr && port);
	}

	inline void Host::serialize(OStream & out) const
	{
		out << addr;
	}

	inline bool Host::equals(const Host & copy) const
	{
		return
			addr == copy.addr &&
			port == copy.port;
	}

	inline bool Host::lessThan(const Host & copy) const
	{
		return
			addr < copy.addr &&
			port < copy.port;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}