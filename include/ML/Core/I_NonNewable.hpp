#ifndef _ML_I_NON_NEWABLE_HPP_
#define _ML_I_NON_NEWABLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Base class for anything you REALLY don't want to be dynamically allocated.
	// Use in conjunction with I_Newable to help ensure memory safety.
	struct I_NonNewable
	{
		inline const std::type_info & get_type_info() const
		{
			return typeid(*this);
		}

		inline friend ML_SERIALIZE(std::ostream & out, const I_NonNewable & value)
		{
			return out << value.get_type_info().name();
		}

	private:
		inline void * operator	new		 (size_t)	{ return nullptr; }
		inline void * operator	new[]	 (size_t)	{ return nullptr; }
		inline void	  operator	delete	 (void *)	{ return; }
		inline void	  operator	delete[] (void *)	{ return; }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NON_NEWABLE_HPP_