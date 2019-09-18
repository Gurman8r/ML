# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    import memelib as ml
    
    # Debug Message
    if ml.config.is_debug:
        ml.io.printf("[ MemeLib {0} | {1} | {2} | {3} ]\n", [ 
            ml.config.project_ver, 
            ml.config.build_date,
            ml.config.build_time,
            ml.config.project_url
            ])

    # Load Plugins
    if ml.config.system_name == "Windows":
        ml.plugins.load([
	    	"Noobs_$(Configuration)_$(PlatformTarget).dll",
	    	"CommandSuite_$(Configuration)_$(PlatformTarget).dll",
	    	"TestPlugin_$(Configuration)_$(PlatformTarget).dll",
	    	])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #