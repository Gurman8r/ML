#ifndef _ML_CLIENT_HPP_
#define _ML_CLIENT_HPP_

#include <ML/Core/Singleton.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Network/NetInterface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetClient final : public Newable, public NetInterface
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { ID = 2 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NetClient();
		~NetClient();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(const Event & value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const Host & host, const String & pass = String());

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool connected() const { return m_connected; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool m_connected;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_HPP_