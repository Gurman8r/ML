#include <ML/Network/Http.hpp>
#include <ML/Core/Debug.hpp>

#define ML_HTTP_PORT 80

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Http::Http()
		: m_host()
	{
	}

	Http::Http(const IpAddress & host)
		: m_host(host)
	{
	}

	Http::~Http()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Http & Http::setHost(const IpAddress & value)
	{
		if (value.addr.substr(0, 7) == "http://")
		{
			m_host.addr = value.addr.substr(7);
			m_host.port = (value.port != 0 ? value.port : ML_HTTP_PORT);
		}
		else if (value.addr.substr(0, 8) == "https://")
		{
			Debug::logError("HTTPS protocol is not supported");

			m_host.addr = "";
			m_host.port = 0;
		}
		else
		{
			m_host.addr = value.addr;
			m_host.port = (value.port != 0 ? value.port : ML_HTTP_PORT);
		}

		// Remove any trailing '/' from the host name
		if (m_host.addr && (*m_host.addr.rbegin() == '/'))
			m_host.addr.erase(m_host.addr.size() - 1);

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}