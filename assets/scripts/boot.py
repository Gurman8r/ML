# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    import memelib as ml
    
    if ml.config.is_debug:
        ml.io.printf("ML | {0} | {1}\n", [ ml.config.project_ver, ml.config.project_url ])

    ml.plugins.load([
		"Noobs_$(Configuration)_$(PlatformTarget).dll",
		"CommandSuite_$(Configuration)_$(PlatformTarget).dll",
		"TestPlugin_$(Configuration)_$(PlatformTarget).dll",
		])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #