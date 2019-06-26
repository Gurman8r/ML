#ifndef _ML_NETWORK_EVENTS_HPP_
#define _ML_NETWORK_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct NetworkEvent final
	{
		enum : int32_t
		{
			MIN_NETWORK_EVENT = Event::EV_NETWORK + 1,

			EV_ServerRecievePacket,
			EV_ClientRecievePacket,

			MAX_NETWORK_EVENT
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ServerRecievePacketEvent final : public I_Event<NetworkEvent::EV_ServerRecievePacket>
	{
		C_String data;
		constexpr ServerRecievePacketEvent(C_String data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ClientRecievePacketEvent final : public I_Event<NetworkEvent::EV_ClientRecievePacket>
	{
		C_String data;
		constexpr ClientRecievePacketEvent(C_String data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_EVENTS_HPP_