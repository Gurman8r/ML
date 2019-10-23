#ifndef _ML_NETWORK_EVENTS_HPP_
#define _ML_NETWORK_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class NetworkEvents
	{
		MIN_NETWORK_EVENT = Event::EV_NETWORK + 1,

		EV_ServerRecievePacket,
		EV_ClientRecievePacket,

		MAX_NETWORK_EVENT
	};

	static_assert(
		(int32_t)NetworkEvents::MAX_NETWORK_EVENT < 
		(int32_t)NetworkEvents::MIN_NETWORK_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <NetworkEvents ID> struct NetworkEvent : public T_Event<NetworkEvents, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ServerRecievePacketEvent final : public NetworkEvent<NetworkEvents::EV_ServerRecievePacket>
	{
		C_String data;
		constexpr ServerRecievePacketEvent(C_String data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ClientRecievePacketEvent final : public NetworkEvent<NetworkEvents::EV_ClientRecievePacket>
	{
		C_String data;
		constexpr ClientRecievePacketEvent(C_String data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_EVENTS_HPP_