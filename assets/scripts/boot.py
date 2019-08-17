# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    import memelib as ml
    
    # Debug Message
    if ml.config.is_debug:
        ml.io.printf("[ MemeLib | {0} | {1} ]\n", [ 
            ml.config.project_ver, 
            ml.config.project_url 
            ])

    # Load Plugins
    ml.plugins.load([
		"Noobs_$(Configuration)_$(PlatformTarget).dll",
		"CommandSuite_$(Configuration)_$(PlatformTarget).dll",
		"TestPlugin_$(Configuration)_$(PlatformTarget).dll",
		])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #