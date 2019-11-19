#ifndef _ML_NETWORK_INTERFACE_HPP_
#define _ML_NETWORK_INTERFACE_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/NonCopyable.hpp>
#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>
#include <ML/Network/Packet.hpp>
#include <ML/Network/SendSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetInterface : public EventListener, public Disposable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Host LocalHost{ "127.0.0.1" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NetInterface();
		virtual ~NetInterface();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setup();
		
		bool dispose() override;
		
		void poll();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t send(const GUID & guid, const String & data, const SendSettings & settings);
		
		uint32_t send(const Host & host, const String & data, const SendSettings & settings);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onEvent(const Event & value) override = 0;
		
		virtual void onPacket(const Packet & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GUID getMyGUID() const;
		
		GUID getGUIDFromAddress(const Host & value) const;
		
		Host getMyAddress() const;
		
		Host getAddressFromGUID(const GUID & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * m_peer;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_INTERFACE_HPP_