#ifndef _ML_NETWORK_EVENTS_HPP_
#define _ML_NETWORK_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class NetworkEventType
	{
		MIN_NETWORK_EVENT = Event::EV_NETWORK + 1,

		EV_ServerRecievePacket,
		EV_ClientRecievePacket,

		MAX_NETWORK_EVENT
	};

	static_assert(
		(int32_t)NetworkEventType::MAX_NETWORK_EVENT < 
		(int32_t)NetworkEventType::MIN_NETWORK_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <NetworkEventType ID> struct NetworkEvent : public T_Event<NetworkEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ServerRecievePacketEvent final : public NetworkEvent<NetworkEventType::EV_ServerRecievePacket>
	{
		const C_String data;
		constexpr ServerRecievePacketEvent(C_String data) noexcept
			: data{ data }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ClientRecievePacketEvent final : public NetworkEvent<NetworkEventType::EV_ClientRecievePacket>
	{
		const C_String data;
		constexpr ClientRecievePacketEvent(C_String data) noexcept
			: data{ data }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_EVENTS_HPP_