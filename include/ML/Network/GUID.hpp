#ifndef _ML_GUID_HPP_
#define _ML_GUID_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API GUID final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		uint64_t id;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr GUID(const uint64_t id)
			: id { id }
		{
		}

		constexpr GUID(const GUID & copy)
			: GUID { copy.id }
		{
		}

		constexpr GUID()
			: GUID { 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr operator uint64_t() const
		{ 
			return this->id; 
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const GUID & value)
	{
		return out << value.id;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const GUID & lhs, const uint64_t & rhs)
	{
		return lhs.id == rhs;
	}

	constexpr bool operator!=(const GUID & lhs, const uint64_t & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const GUID & lhs, const uint64_t & rhs)
	{
		return lhs.id < rhs;
	}

	constexpr bool operator>(const GUID & lhs, const uint64_t & rhs)
	{
		return !(lhs < rhs);
	}

	constexpr bool operator<=(const GUID & lhs, const uint64_t & rhs)
	{
		return (lhs == rhs) || (lhs < rhs);
	}

	constexpr bool operator>=(const GUID & lhs, const uint64_t & rhs)
	{
		return (lhs == rhs) || (lhs > rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const GUID & lhs, const GUID & rhs)
	{
		return lhs == rhs.id;
	}

	constexpr bool operator!=(const GUID & lhs, const GUID & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const GUID & lhs, const GUID & rhs)
	{
		return lhs < rhs.id;
	}

	constexpr bool operator>(const GUID & lhs, const GUID & rhs)
	{
		return !(lhs < rhs);
	}

	constexpr bool operator<=(const GUID & lhs, const GUID & rhs)
	{
		return (lhs == rhs) || (lhs < rhs);
	}

	constexpr bool operator>=(const GUID & lhs, const GUID & rhs)
	{
		return (lhs == rhs) || (lhs > rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUID_HPP_