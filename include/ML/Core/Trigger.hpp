#ifndef _ML_TRIGGER_HPP_
#define _ML_TRIGGER_HPP_

#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// A 'consumable' bool which can be used to do something once.
	// If armed, consuming trigger will reset its value and return true.
	struct Trigger final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Trigger() : m_value(false) {}

		Trigger(const Trigger & copy) : m_value(copy.m_value) {}

		~Trigger() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool arm()			{ return (m_value = true); }
		inline bool consume()		{ return peek() && disarm(); }
		inline bool disarm()		{ return !(m_value = false); }
		inline bool peek() const	{ return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: bool m_value;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRIGGER_HPP_