#ifndef _ML_SEND_SETTINGS_HPP_
#define _ML_SEND_SETTINGS_HPP_

#include <ML/Network/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum Priority : uint8_t
	{
		Immediate,
		High,
		Medium,
		Low,

		MAX_PRIORITY
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct SendSettings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Priority	priority;
		Reliability reliability;
		char		ordering;
		bool		broadcast;
		uint32_t	receiptNumber;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const SendSettings & value)
		{
			return out
				<< value.priority << " "
				<< value.reliability << " "
				<< value.ordering << " "
				<< value.broadcast << " "
				<< value.receiptNumber << " ";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const SendSettings & other)
		{
			return
				this->priority == other.priority &&
				this->reliability == other.reliability &&
				this->ordering == other.ordering &&
				this->broadcast == other.broadcast &&
				this->receiptNumber == other.receiptNumber;
		}

		constexpr bool operator!=(const SendSettings & other)
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const SendSettings & other)
		{
			return
				this->priority < other.priority &&
				this->reliability < other.reliability &&
				this->ordering < other.ordering &&
				this->broadcast < other.broadcast &&
				this->receiptNumber < other.receiptNumber;
		}

		constexpr bool operator>(const SendSettings & other)
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(const SendSettings & other)
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(const SendSettings & other)
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SEND_SETTINGS_HPP_