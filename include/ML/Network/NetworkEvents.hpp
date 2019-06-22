#ifndef _ML_NETWORK_EVENTS_HPP_
#define _ML_NETWORK_EVENTS_HPP_

#include <ML/Network/Export.hpp>
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

	struct ML_NETWORK_API ServerRecievePacketEvent final : public IEvent<NetworkEvent::EV_ServerRecievePacket>
	{
		CString data;
		constexpr ServerRecievePacketEvent(CString data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API ClientRecievePacketEvent final : public IEvent<NetworkEvent::EV_ClientRecievePacket>
	{
		CString data;
		constexpr ClientRecievePacketEvent(CString data)
			: data(data)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_EVENTS_HPP_