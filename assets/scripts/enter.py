# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
#
#   This script is called once when the program starts, before the window is created.
#
#   For more information see:
#   https://github.com/Gurman8r/ML/blob/master/src/ML/Engine/Python.cpp
#
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import MEMELIB as ml

# Debug Mode
if (ml.config.is_debug()): 
    ml.io.printf("{0}/{1} | {2}/{3} | {4}/{5} \n", [
        ml.config.project_name(),
        ml.config.project_version(),
        ml.config.configuration(),
        ml.config.platform_target(),
        ml.config.project_date(),
        ml.config.project_time()
        ])
    ml.prefs.set("Editor", "style_config", "../../../assets/styles/obsidian.style")
    #ml.io.pause()
    #ml.io.exit()

# Load Plugins
if (ml.config.system_name() == "Windows"):
    ml.plugins.load_all([
 	    "Noobs_$(Configuration)_$(PlatformTarget).dll",
 	    "CommandSuite_$(Configuration)_$(PlatformTarget).dll",
 	    "TestPlugin_$(Configuration)_$(PlatformTarget).dll",
 	    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #