# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_config   as config
import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io
import memelib_plugins  as plugins
import memelib_prefs    as prefs
import memelib_window   as window

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# Print Version
if (config.IS_DEBUG):
    io.printl(config.MEMELIB_VERSION)

# Load Plugins
if (config.SYSTEM_NAME == "Windows"):
    plugins.load_all([
 	    "Noobs_$(Configuration)_$(PlatformTarget).dll",
 	    "CommandSuite_$(Configuration)_$(PlatformTarget).dll",
 	    #"TestPlugin_$(Configuration)_$(PlatformTarget).dll",
 	    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #