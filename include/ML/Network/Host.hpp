#ifndef _ML_HOST_HPP_
#define _ML_HOST_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/StringView.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Host final
	{
		StringView	addr;
		uint16_t	port;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Host(StringView const & addr, uint16_t port) : addr { addr }, port { port } {}

		constexpr Host(StringView const & addr) : Host { addr, 0 } {}

		constexpr Host(Host const & copy) : Host { copy.addr, copy.port } {}

		constexpr Host() : Host { "", 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const { return (this->addr && this->port); }

		inline friend ML_SERIALIZE(std::ostream & out, Host const & value)
		{
			return out << value.addr;
		}

		constexpr bool operator==(Host const & other)
		{
			return !((*this) < other) && !(other < (*this));
		}

		constexpr bool operator!=(Host const & other)
		{
			return !((*this) == other);
		}

		constexpr bool operator<(Host const & other)
		{
			return (this->port < other.port) && (this->addr < other.addr);
		}

		constexpr bool operator>(Host const & other)
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(Host const & other)
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(Host const & other)
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HOST_HPP_