# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_config   as config
import memelib_io       as io
import memelib_plugins  as plugins

# Debug Message
if config.is_debug:
    io.printf("[ {0} | {1} | {2} | {3} | {4} ]\n", [ 
        config.project_name, 
        config.project_ver,
        config.project_date,
        config.project_time,
        config.project_auth
        ])

# Load Plugins
if (config.system_name == "Windows"):
    plugins.load_all([
 	    "Noobs_$(Configuration)_$(PlatformTarget).dll",
 	    "CommandSuite_$(Configuration)_$(PlatformTarget).dll",
 	    "TestPlugin_$(Configuration)_$(PlatformTarget).dll",
 	    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #