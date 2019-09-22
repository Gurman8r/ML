# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_config   as config
import memelib_io       as io
import memelib_plugins  as plugins

# Debug Message
if (config.CONFIGURATION == "Debug"):
    io.printf("{0} {1} | by {2} | {3} - {4}\n", [ 
        config.PROJECT_NAME, 
        config.PROJECT_VER,
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