#ifndef _ML_HOST_HPP_
#define _ML_HOST_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/NonNewable.hpp>
#include <ML/Core/StringView.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Host final : public NonNewable
	{
		StringView	addr;
		uint16_t	port;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Host(const StringView & addr, uint16_t port) : addr { addr }, port { port } {}

		constexpr Host(const StringView & addr) : Host { addr, 0 } {}

		constexpr Host(const Host & copy) : Host { copy.addr, copy.port } {}

		constexpr Host() : Host { "", 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const { return (this->addr && this->port); }

		inline friend ML_SERIALIZE(std::ostream & out, const Host & value)
		{
			return out << value.addr;
		}

		constexpr bool operator==(const Host & other)
		{
			return !((*this) < other) && !(other < (*this));
		}

		constexpr bool operator!=(const Host & other)
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const Host & other)
		{
			return (this->port < other.port) && (this->addr < other.addr);
		}

		constexpr bool operator>(const Host & other)
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(const Host & other)
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(const Host & other)
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HOST_HPP_