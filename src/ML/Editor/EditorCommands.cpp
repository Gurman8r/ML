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
				<< "# Fonts" << endl << ML_Resources.fonts
				<< "# Images" << endl << ML_Resources.images
				<< "# Meshes" << endl << ML_Resources.meshes
				<< "# Models" << endl << ML_Resources.models
				<< "# Scripts" << endl << ML_Resources.scripts
				<< "# Shaders" << endl << ML_Resources.shaders
				<< "# Skyboxes" << endl << ML_Resources.skyboxes
				<< "# Sounds" << endl << ML_Resources.sounds
				<< "# Sprites" << endl << ML_Resources.sprites
				<< "# Textures" << endl << ML_Resources.textures
				<< endl;
		}
		else if (type == "fonts") { cout << ML_Resources.fonts << endl; }
		else if (type == "images") { cout << ML_Resources.images << endl; }
		else if (type == "meshes") { cout << ML_Resources.meshes << endl; }
		else if (type == "models") { cout << ML_Resources.models << endl; }
		else if (type == "scripts") { cout << ML_Resources.scripts << endl; }
		else if (type == "shaders") { cout << ML_Resources.shaders << endl; }
		else if (type == "skyboxes") { cout << ML_Resources.skyboxes << endl; }
		else if (type == "sounds") { cout << ML_Resources.sounds << endl; }
		else if (type == "sprites") { cout << ML_Resources.sprites << endl; }
		else if (type == "textures") { cout << ML_Resources.textures << endl; }
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
			return Var().intValue((int32_t)ML_Resources.reloadAll());
		}
		else if (type == "fonts") { return Var().intValue((int32_t)ML_Resources.fonts.reload()); }
		else if (type == "images") { return Var().intValue((int32_t)ML_Resources.images.reload()); }
		else if (type == "meshes") { return Var().intValue((int32_t)ML_Resources.meshes.reload()); }
		else if (type == "models") { return Var().intValue((int32_t)ML_Resources.models.reload()); }
		else if (type == "scripts") { return Var().intValue((int32_t)ML_Resources.scripts.reload()); }
		else if (type == "shaders") { return Var().intValue((int32_t)ML_Resources.shaders.reload()); }
		else if (type == "skyboxes") { return Var().intValue((int32_t)ML_Resources.skyboxes.reload()); }
		else if (type == "sounds") { return Var().intValue((int32_t)ML_Resources.sounds.reload()); }
		else if (type == "sprites") { return Var().intValue((int32_t)ML_Resources.sprites.reload()); }
		else if (type == "textures") { return Var().intValue((int32_t)ML_Resources.textures.reload()); }
		else
		{
			return Var().errorValue("Type not found: {0}", type);
		}
	}

	Var EditorCommands::cmd_run(Args & args)
	{
		bool flag_rebuild = args.find_and_erase("-r");

		const String name = args.pop();
		if (Script * scr = ML_Resources.scripts.get(name))
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