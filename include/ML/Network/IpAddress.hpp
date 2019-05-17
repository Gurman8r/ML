#ifndef _ML_IP_ADDRESS_HPP_
#define _ML_IP_ADDRESS_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API IpAddress final
		: public ITrackable
		, public IComparable<IpAddress>
	{
		String	 addr;
		uint16_t port;

		IpAddress();

		IpAddress(const String & addr);

		IpAddress(const String & addr, uint16_t port);

		IpAddress(const IpAddress & copy);

		operator bool() const;

		void serialize(std::ostream & out) const override;

		bool equals(const IpAddress & copy) const override;

		bool lessThan(const IpAddress & copy) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IP_ADDRESS_HPP_