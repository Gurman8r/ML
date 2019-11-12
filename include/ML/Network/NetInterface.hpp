#ifndef _ML_NETWORK_INTERFACE_HPP_
#define _ML_NETWORK_INTERFACE_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>
#include <ML/Network/Packet.hpp>
#include <ML/Network/SendSettings.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_LOCALHOST	"127.0.0.1"
#define ML_DEFAULT_PORT	60000
#define ML_MAX_CLIENTS	10

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_USER_PACKET 134 // ID_USER_PACKET_ENUM
#define ML_SERVER_RECIEVE (ML_USER_PACKET + 1)
#define ML_CLIENT_RECIEVE (ML_USER_PACKET + 2)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr Host LocalHost { "127.0.0.1" };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetInterface
		: public EventListener
		, public Disposable
		, public NonCopyable
	{
	public:
		NetInterface();
		virtual ~NetInterface();

	public:
		bool setup();
		bool dispose() override;
		void poll();

	public:
		uint32_t send(const GUID & guid, const String & data, const SendSettings & settings);
		uint32_t send(const Host & host, const String & data, const SendSettings & settings);

	public:
		virtual void onEvent(const Event & value) override = 0;
		virtual void onPacket(const Packet & value) = 0;

	public:
		GUID	getMyGUID() const;
		GUID	getGUIDFromAddress(const Host & value) const;
		Host	getMyAddress() const;
		Host	getAddressFromGUID(const GUID & value) const;

	protected:
		ptr_t<void> m_peer;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_INTERFACE_HPP_