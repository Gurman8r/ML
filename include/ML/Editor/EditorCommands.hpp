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
		static Var cmd_list		(Args & args); // Display Resources
		static Var cmd_reload	(Args & args); // Reload Resources
		static Var cmd_run		(Args & args); // Run Script
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_COMMANDS_HPP_