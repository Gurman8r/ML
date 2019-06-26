#ifndef _ML_SEND_SETTINGS_HPP_
#define _ML_SEND_SETTINGS_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/I_Newable.hpp>

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

	struct ML_NETWORK_API SendSettings final : public I_NonNewable
	{
		Priority	priority;
		Reliability reliability;
		char		ordering;
		bool		broadcast;
		uint32_t	receiptNumber;

		constexpr SendSettings(const SendSettings & copy) : SendSettings(
			copy.priority,
			copy.reliability,
			copy.ordering,
			copy.broadcast,
			copy.receiptNumber)
		{
		}

		constexpr SendSettings(Priority priority, Reliability reliability, char ordering, bool broadcast, uint32_t receiptNumber)
			: priority(priority)
			, reliability(reliability)
			, ordering(ordering)
			, broadcast(broadcast)
			, receiptNumber(receiptNumber)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const SendSettings & value)
	{
		return out
			<< value.priority		<< " "
			<< value.reliability	<< " "
			<< value.ordering		<< " "
			<< value.broadcast		<< " "
			<< value.receiptNumber	<< " ";
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const SendSettings & lhs, const SendSettings & rhs)
	{
		return
			lhs.priority		== rhs.priority &&
			lhs.reliability		== rhs.reliability &&
			lhs.ordering		== rhs.ordering &&
			lhs.broadcast		== rhs.broadcast &&
			lhs.receiptNumber	== rhs.receiptNumber;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator<(const SendSettings & lhs, const SendSettings & rhs)
	{
		return
			lhs.priority		< rhs.priority &&
			lhs.reliability		< rhs.reliability &&
			lhs.ordering		< rhs.ordering &&
			lhs.broadcast		< rhs.broadcast &&
			lhs.receiptNumber	< rhs.receiptNumber;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SEND_SETTINGS_HPP_