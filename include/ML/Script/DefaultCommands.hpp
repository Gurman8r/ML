#ifndef _ML_BUILTIN_HPP_
#define _ML_BUILTIN_HPP_

#include <ML/Script/Export.hpp>
#include <ML/Script/Args.hpp>
#include <ML/Script/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Interpreter;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API DefaultCommands final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void install(Interpreter & interpreter);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static Var cmd_cat		(Args & args); // Display File
		static Var cmd_cd		(Args & args); // Change Directory
		static Var cmd_cwd		(Args & args); // Print Working Directory
		static Var cmd_exec		(Args & args); // Execute File
		static Var cmd_exists	(Args & args); // Check Exists (File/Var)
		static Var cmd_get		(Args & args); // Get Variable
		static Var cmd_getcwd	(Args & args); // Return Working Directory
		static Var cmd_help		(Args & args); // Display Installed Commands
		static Var cmd_log		(Args & args); // Debug Log (msg/wrn/err)
		static Var cmd_ls		(Args & args); // Display a Directory
		static Var cmd_pause	(Args & args); // System Pause
		static Var cmd_os		(Args & args); // Execute OS Command
		static Var cmd_read		(Args & args); // Return File/Dir Contents
		static Var cmd_run		(Args & args); // Run Script
		static Var cmd_set		(Args & args); // Set Variable
		static Var cmd_system	(Args & args); // System Call

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUILTIN_HPP_