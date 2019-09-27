# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import ml_config
import ml_content
import ml_ecs
import ml_io
import ml_plugins
import ml_prefs
import ml_window

# Debug Message
if (ml_config.IS_DEBUG): 
    ml_io.printf("{0}-{1} ({2}/{3}) ({4} {5}) \n", [
        ml_config.PROJECT_NAME,
        ml_config.PROJECT_VER,
        ml_config.CONFIGURATION,
        ml_config.PLATFORM_TARGET,
        ml_config.PROJECT_DATE,
        ml_config.PROJECT_TIME
        ])

# Load Plugins
if (ml_config.SYSTEM_NAME == "Windows"):
    ml_plugins.load_all([
 	    "Noobs_$(Configuration)_$(PlatformTarget).dll",
 	    "CommandSuite_$(Configuration)_$(PlatformTarget).dll",
 	    #"TestPlugin_$(Configuration)_$(PlatformTarget).dll",
 	    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #