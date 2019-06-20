#ifndef _ML_HOST_HPP_
#define _ML_HOST_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/INewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Host final
		: public INewable
		, public IComparable<Host>
	{
		String	 addr;
		uint16_t port;

		Host();
		Host(const String & addr);
		Host(const String & addr, uint16_t port);
		Host(const Host & copy);

		operator bool() const;

		void serialize(ostream & out) const override;

		bool equals(const Host & copy) const override;

		bool lessThan(const Host & copy) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HOST_HPP_