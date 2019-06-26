#ifndef _ML_BUILTIN_HPP_
#define _ML_BUILTIN_HPP_

#include <ML/Script/Export.hpp>
#include <ML/Script/Args.hpp>
#include <ML/Script/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API DefaultCommands final
	{
		static Var cmd_cat		(Arguments & args); // Display File
		static Var cmd_cd		(Arguments & args); // Change Directory
		static Var cmd_cwd		(Arguments & args); // Print Working Directory
		static Var cmd_exec		(Arguments & args); // Execute File
		static Var cmd_exists	(Arguments & args); // Check Exists (File/Var)
		static Var cmd_get		(Arguments & args); // Get Variable
		static Var cmd_getcwd	(Arguments & args); // Return Working Directory
		static Var cmd_help		(Arguments & args); // Display Installed Commands
		static Var cmd_log		(Arguments & args); // Debug Log (msg/wrn/err)
		static Var cmd_ls		(Arguments & args); // Display a Directory
		static Var cmd_pause	(Arguments & args); // System Pause
		static Var cmd_os		(Arguments & args); // Execute OS Command
		static Var cmd_read		(Arguments & args); // Return File/Dir Contents
		static Var cmd_run		(Arguments & args); // Run Script
		static Var cmd_set		(Arguments & args); // Set Variable
		static Var cmd_system	(Arguments & args); // System Call
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUILTIN_HPP_