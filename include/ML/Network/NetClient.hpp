#ifndef _ML_CLIENT_HPP_
#define _ML_CLIENT_HPP_

#include <ML/Core/I_Singleton.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Network/NetInterface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetClient final
		: public I_Newable
		, public NetInterface
	{
	public:
		enum { ID = 2 };

	public:
		NetClient();
		~NetClient();

	public:
		void onEvent(const Event & value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const Host & host, const String & pass = String());

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_HPP_