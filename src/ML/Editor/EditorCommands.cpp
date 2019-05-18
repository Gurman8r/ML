#include <ML/Editor/EditorCommands.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void EditorCommands::install()
	{
		ML_Interpreter.install({ "clear",	EditorCommands::cmd_clear	});
		ML_Interpreter.install({ "history",	EditorCommands::cmd_history });
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var EditorCommands::cmd_clear(Args & args)
	{
		ML_Editor.terminal.clear();
		return Var().voidValue();
	}

	Var EditorCommands::cmd_history(Args & args)
	{
		for (CString h : ML_Editor.terminal.history())
		{
			ml::cout << h << ml::endl;
		}
		return Var().boolValue(true);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}