# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_config   as config
import memelib_io       as io
import memelib_plugins  as plugins
import memelib_prefs    as prefs

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# Modify Preferences
#prefs.set("Noobs", "demo_material", "advanced")
#prefs.set("Noobs", "demo_model", "sphere32x24")

# Debug Message
if config.IS_DEBUG:
    io.printf("{0} {1} | ({2}/{3}) | by {4} | {5} {6}\n", [ 
        config.PROJECT_NAME, 
        config.PROJECT_VER,
        config.CONFIGURATION,
        config.PLATFORM_TARGET,
        config.PROJECT_AUTH,
        config.PROJECT_DATE,
        config.PROJECT_TIME
        ])

# Load Plugins
if (config.SYSTEM_NAME == "Windows"):
    plugins.load_all([
 	    "Noobs_$(Configuration)_$(PlatformTarget).dll",
 	    "CommandSuite_$(Configuration)_$(PlatformTarget).dll",
 	    "TestPlugin_$(Configuration)_$(PlatformTarget).dll",
 	    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #