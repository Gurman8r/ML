#ifndef _ML_FTP_HPP_
#define _ML_FTP_HPP_

#include <ML/Network/GUID.hpp>
#include <ML/Network/IpAddress.hpp>
#include <ML/Network/Packet.hpp>
#include <ML/Network/SendSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	class ML_NETWORK_API Ftp final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Ftp();
		~Ftp();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FTP_HPP_