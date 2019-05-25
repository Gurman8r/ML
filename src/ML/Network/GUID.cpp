#include <ML/Network/GUID.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GUID::GUID()
		: GUID(0)
	{
	}

	GUID::GUID(const uint64_t id)
		: id(id)
	{
	}

	GUID::GUID(const GUID & copy)
		: GUID(copy.id)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline GUID::operator uint64_t() const
	{
		return id;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline void GUID::serialize(std::ostream & out) const
	{
		out << id;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline bool GUID::equals(const uint64_t & value) const
	{
		return ((*this) == value);
	}

	inline bool GUID::equals(const GUID & value) const
	{
		return (id == value.id);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline bool GUID::lessThan(const uint64_t & value) const
	{
		return ((*this) < value);
	}

	inline bool GUID::lessThan(const GUID & value) const
	{
		return (id < value.id);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}