#ifndef _ML_CLIENT_HPP_
#define _ML_CLIENT_HPP_

#include <ML/Core/ISingleton.hpp>
#include <ML/Network/NetInterface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetClient final
		: public ITrackable
		, public NetInterface
	{
	public:
		enum { ID = 2 };

	public:
		explicit NetClient(EventSystem & eventSystem);
		~NetClient();

	public:
		void onEvent(const Event * value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const IpAddress & addr, const String & pass = String());

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_HPP_