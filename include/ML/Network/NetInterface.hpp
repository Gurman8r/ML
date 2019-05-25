#ifndef _ML_NETWORK_INTERFACE_HPP_
#define _ML_NETWORK_INTERFACE_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>
#include <ML/Network/Packet.hpp>
#include <ML/Network/SendSettings.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_LOCALHOST	"127.0.0.1"
#define ML_DEFAULT_PORT	60000
#define ML_MAX_CLIENTS	10

/* * * * * * * * * * * * * * * * * * * * */

#define ML_USER_PACKET 134 // ID_USER_PACKET_ENUM
#define ML_SERVER_RECIEVE (ML_USER_PACKET + 1)
#define ML_CLIENT_RECIEVE (ML_USER_PACKET + 2)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetInterface
		: public EventListener
		, public IDisposable
		, public INonCopyable
	{
	public:
		explicit NetInterface(EventSystem & eventSystem);
		virtual ~NetInterface();

	public:
		bool setup();
		bool dispose() override;
		void poll();

	public:
		uint32_t send(const GUID & guid, const String & data, const SendSettings & settings);
		uint32_t send(const Host & address, const String & data, const SendSettings & settings);

	public:
		virtual void onEvent(const Event * ev) override = 0;
		virtual void onPacket(const Packet & value) = 0;

	public:
		GUID	getMyGUID() const;
		GUID	getGUIDFromAddress(const Host & value) const;
		Host	getMyAddress() const;
		Host	getAddressFromGUID(const GUID & value) const;

	protected:
		void * m_peer;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_INTERFACE_HPP_