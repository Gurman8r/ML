#ifndef _ML_HOST_HPP_
#define _ML_HOST_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/StringView.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Host final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Addr = typename StringView;
		using Port = typename uint16_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Addr addr;
		Port port;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Host()
			: Host{ "", 0 }
		{
		}

		constexpr Host(Host const & copy)
			: Host{ copy.addr, copy.port }
		{
		}

		constexpr Host(Addr const & addr) 
			: Host { addr, 0 } 
		{
		}

		constexpr Host(Addr const & addr, Port port)
			: addr{ addr }, port{ port }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const { return (this->addr && this->port); }

		inline friend ML_SERIALIZE(std::ostream & out, Host const & value)
		{
			return out << value.addr << ':' << value.port;
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