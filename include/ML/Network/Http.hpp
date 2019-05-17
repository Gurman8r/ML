#ifndef _ML_HTTP_HPP_
#define _ML_HTTP_HPP_

#include <ML/Network/IpAddress.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	class ML_NETWORK_API Http final
		: public ITrackable
		, public INonCopyable
	{
	public:
		struct ML_NETWORK_API Request
		{
		};

		struct ML_NETWORK_API Response
		{

		};

	public:
		Http();
		Http(const IpAddress & host);
		~Http();

	public:
		Http & setHost(const IpAddress & value);

	private:
		IpAddress m_host;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HTTP_HPP_