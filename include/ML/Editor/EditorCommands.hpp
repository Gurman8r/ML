#ifndef _ML_EDITOR_COMMANDS_HPP_
#define _ML_EDITOR_COMMANDS_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Script/Args.hpp>
#include <ML/Script/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorCommands final
	{
		static void install();

		static Var cmd_clear	(Args & args); // Clear Terminal
		static Var cmd_history	(Args & args); // Display Terminal History
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_COMMANDS_HPP_