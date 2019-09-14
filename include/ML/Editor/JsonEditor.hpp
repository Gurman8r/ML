#ifndef _ML_JSON_EDITOR_HPP_
#define _ML_JSON_EDITOR_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	// WIP
	struct ML_EDITOR_API JsonEditor final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		JsonEditor();
		~JsonEditor();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	
}

#endif // !_ML_JSON_EDITOR_HPP_