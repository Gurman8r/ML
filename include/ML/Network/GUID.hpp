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

		using type = typename uint64_t;

		static constexpr auto Invalid{ static_cast<type>(-1) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		type id;

		constexpr GUID(type id) : id { id } {}

		constexpr GUID(GUID const & copy) : GUID { copy.id } {}

		constexpr GUID() : GUID { GUID::Invalid } {}

		constexpr operator type const &() const { return this->id; }

		inline friend ML_SERIALIZE(std::ostream & out, GUID const & value)
		{
			return out << value.id;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUID_HPP_