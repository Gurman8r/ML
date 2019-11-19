#ifndef _ML_GUID_HPP_
#define _ML_GUID_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct GUID final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint64_t id;

		constexpr GUID(uint64_t id) : id { id } {}

		constexpr GUID(GUID const & copy) : GUID { copy.id } {}

		constexpr GUID() : GUID { 0 } {}

		constexpr operator uint64_t() const { return this->id; }

		inline friend ML_SERIALIZE(std::ostream & out, GUID const & value)
		{
			return out << value.id;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUID_HPP_