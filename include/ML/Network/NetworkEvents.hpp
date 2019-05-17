#ifndef _ML_NETWORK_EVENTS_HPP_
#define _ML_NETWORK_EVENTS_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetworkEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_NETWORK_EVENT = IEvent::EV_NETWORK + 1,

			EV_ServerRecievePacket,
			EV_ClientRecievePacket,

			MAX_NETWORK_EVENT
		};
		
		NetworkEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API ServerRecievePacketEvent final : public NetworkEvent
	{
		String data;

		ServerRecievePacketEvent(const String & data)
			: NetworkEvent(EV_ServerRecievePacket)
			, data(data)
		{
		}
		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << data;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API ClientRecievePacketEvent final : public NetworkEvent
	{
		String data;

		ClientRecievePacketEvent(const String & data)
			: NetworkEvent(EV_ClientRecievePacket)
			, data(data)
		{
		}
		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " | " << data;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_EVENTS_HPP_