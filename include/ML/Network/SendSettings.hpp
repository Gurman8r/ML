#ifndef _ML_SEND_SETTINGS_HPP_
#define _ML_SEND_SETTINGS_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/INewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum Priority : uint8_t
	{
		Immediate,
		High,
		Medium,
		Low,

		MAX_PRIORITY
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum Reliability : uint8_t
	{
		Unreliable,
		UnreliableSequenced,
		Reliable,
		ReliableOrdered,
		ReliableSequenced,
		UnreliableWithAckReceipt,
		ReliableWithAckReceipt,
		ReliableOrderedWithAckReceipt,

		MAX_RELIABILITY
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API SendSettings final
		: public INewable
	{
		Priority	priority;
		Reliability reliability;
		char		ordering;
		bool		broadcast;
		uint32_t	receiptNumber;

		SendSettings(const SendSettings & copy) : SendSettings(
			copy.priority,
			copy.reliability,
			copy.ordering,
			copy.broadcast,
			copy.receiptNumber)
		{
		}

		SendSettings(Priority priority, Reliability reliability, char ordering, bool broadcast, uint32_t receiptNumber)
			: priority(priority)
			, reliability(reliability)
			, ordering(ordering)
			, broadcast(broadcast)
			, receiptNumber(receiptNumber)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SEND_SETTINGS_HPP_