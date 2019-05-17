#include <ML/Editor/EditorCommands.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	// Install
	/* * * * * * * * * * * * * * * * * * * * */

	void EditorCommands::install()
	{
		ML_Interpreter.install({ "clear",	EditorCommands::cmd_clear	});
		ML_Interpreter.install({ "history",	EditorCommands::cmd_history });
		ML_Interpreter.install({ "list",	EditorCommands::cmd_list	});
		ML_Interpreter.install({ "reload",	EditorCommands::cmd_reload	});
		ML_Interpreter.install({ "run",		EditorCommands::cmd_run		});
	}


	// Inspector
	/* * * * * * * * * * * * * * * * * * * * */

	Var EditorCommands::cmd_clear(Args & args)
	{
		ML_Terminal.clear();
		return Var().voidValue();
	}

	Var EditorCommands::cmd_history(Args & args)
	{
		for (CString h : ML_Terminal.history())
		{
			ml::cout << h << ml::endl;
		}
		return Var().boolValue(true);
	}

	Var EditorCommands::cmd_list(Args & args)
	{
		const String type = args.pop();
		if (type == "all")
		{
			cout
				<< "# Fonts" << endl << ML_Res.fonts
				<< "# Images" << endl << ML_Res.images
				<< "# Meshes" << endl << ML_Res.meshes
				<< "# Models" << endl << ML_Res.models
				<< "# Scripts" << endl << ML_Res.scripts
				<< "# Shaders" << endl << ML_Res.shaders
				<< "# Skyboxes" << endl << ML_Res.skyboxes
				<< "# Sounds" << endl << ML_Res.sounds
				<< "# Sprites" << endl << ML_Res.sprites
				<< "# Textures" << endl << ML_Res.textures
				<< endl;
		}
		else if (type == "fonts") { cout << ML_Res.fonts << endl; }
		else if (type == "images") { cout << ML_Res.images << endl; }
		else if (type == "meshes") { cout << ML_Res.meshes << endl; }
		else if (type == "models") { cout << ML_Res.models << endl; }
		else if (type == "scripts") { cout << ML_Res.scripts << endl; }
		else if (type == "shaders") { cout << ML_Res.shaders << endl; }
		else if (type == "skyboxes") { cout << ML_Res.skyboxes << endl; }
		else if (type == "sounds") { cout << ML_Res.sounds << endl; }
		else if (type == "sprites") { cout << ML_Res.sprites << endl; }
		else if (type == "textures") { cout << ML_Res.textures << endl; }
		else
		{
			return Var().errorValue("Type not found: {0}", type);
		}
		return Var().boolValue(true);
	}

	Var EditorCommands::cmd_reload(Args & args)
	{
		const String type = args.pop();
		if (type == "all")
		{
			return Var().intValue((int32_t)ML_Res.reloadAll());
		}
		else if (type == "fonts") { return Var().intValue((int32_t)ML_Res.fonts.reload()); }
		else if (type == "images") { return Var().intValue((int32_t)ML_Res.images.reload()); }
		else if (type == "meshes") { return Var().intValue((int32_t)ML_Res.meshes.reload()); }
		else if (type == "models") { return Var().intValue((int32_t)ML_Res.models.reload()); }
		else if (type == "scripts") { return Var().intValue((int32_t)ML_Res.scripts.reload()); }
		else if (type == "shaders") { return Var().intValue((int32_t)ML_Res.shaders.reload()); }
		else if (type == "skyboxes") { return Var().intValue((int32_t)ML_Res.skyboxes.reload()); }
		else if (type == "sounds") { return Var().intValue((int32_t)ML_Res.sounds.reload()); }
		else if (type == "sprites") { return Var().intValue((int32_t)ML_Res.sprites.reload()); }
		else if (type == "textures") { return Var().intValue((int32_t)ML_Res.textures.reload()); }
		else
		{
			return Var().errorValue("Type not found: {0}", type);
		}
	}

	Var EditorCommands::cmd_run(Args & args)
	{
		bool flag_rebuild = args.find_and_erase("-r");

		const String name = args.pop();
		if (Script * scr = ML_Res.scripts.get(name))
		{
			auto build_fun = ((flag_rebuild)
				? (&Script::rebuild)
				: (&Script::build)
			);

			args.pop_front();

			if ((scr->*build_fun)(args))
			{
				if (scr->run())
				{
					return scr->retv();
				}
			}
			return Var().stringValue(name);
		}
		return Var().errorValue("Script not found: {0}", name);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}