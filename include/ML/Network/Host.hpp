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
		C_String	addr;
		uint16_t	port;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr Host(C_String addr, uint16_t port)
			: addr { addr } 
			, port { port } 
		{
		}

		constexpr Host(C_String addr)
			: Host { addr, 0 }
		{
		}

		constexpr Host(const Host & copy)
			: Host { copy.addr, copy.port }
		{
		}

		constexpr Host()
			: Host { "", 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const 
		{ 
			return (addr && port); 
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Host & value)
	{
		return out << value.addr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const Host & lhs, const Host & rhs)
	{
		return
			(lhs.port == rhs.port) &&
			((StringView)lhs.addr == (StringView)rhs.addr);
	}

	constexpr bool operator!=(const Host & lhs, const Host & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const Host & lhs, const Host & rhs)
	{
		return
			(lhs.port < rhs.port) &&
			((StringView)lhs.addr < (StringView)rhs.addr);
	}

	constexpr bool operator>(const Host & lhs, const Host & rhs)
	{
		return !(lhs < rhs);
	}

	constexpr bool operator<=(const Host & lhs, const Host & rhs)
	{
		return (lhs == rhs) || (lhs < rhs);
	}

	constexpr bool operator>=(const Host & lhs, const Host & rhs)
	{
		return (lhs == rhs) || (lhs > rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HOST_HPP_