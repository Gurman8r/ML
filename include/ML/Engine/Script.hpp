#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Placeholder, moved here from old scripting lib.
	struct ML_ENGINE_API Script
		: public I_Newable
		, public I_Readable
	{
		Script() {}

		virtual ~Script() {}

		inline bool loadFromFile(const String & filename) override
		{ 
			return false; 
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_