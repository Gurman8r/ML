#ifndef _ML_GUID_HPP_
#define _ML_GUID_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/IObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API GUID final
		: public IObject
		, public IComparable<uint64_t>
		, public IComparable<GUID>
	{
		uint64_t id;

		GUID();
		GUID(const uint64_t id);
		GUID(const GUID & copy);

		operator uint64_t() const;

		void serialize(std::ostream & out) const override;

		bool equals(const uint64_t & value) const override;
		bool equals(const GUID & value) const override;

		bool lessThan(const uint64_t & value) const override;
		bool lessThan(const GUID & value) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUID_HPP_