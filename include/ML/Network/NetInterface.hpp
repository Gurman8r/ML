#ifndef _ML_NETWORK_INTERFACE_HPP_
#define _ML_NETWORK_INTERFACE_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/NonCopyable.hpp>
#include <ML/Network/GUID.hpp>
#include <ML/Network/Host.hpp>
#include <ML/Network/Packet.hpp>
#include <ML/Network/SendSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetInterface : public EventListener, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Host LocalHost{ "127.0.0.1" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NetInterface();
		virtual ~NetInterface() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setup();
		
		bool dispose();
		
		void poll();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t send(GUID const & guid, String const & data, SendSettings const & settings);
		
		uint32_t send(Host const & host, String const & data, SendSettings const & settings);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onEvent(Event const & value) override = 0;
		
		virtual void onPacket(Packet const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		GUID getMyGUID() const;
		
		GUID getGUIDFromAddress(Host const & value) const;
		
		Host getMyAddress() const;
		
		Host getAddressFromGUID(GUID const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * m_peer;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_INTERFACE_HPP_