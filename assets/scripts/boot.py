import memelib as ml

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    if ml.config.is_debug:
        ml.io.printl("MemeLib " + ml.config.project_ver + " (" + ml.config.project_url + ")")

    ml.plugins.load("Noobs_$(Configuration)_$(PlatformTarget).dll")
    ml.plugins.load("CommandSuite_$(Configuration)_$(PlatformTarget).dll")
    ml.plugins.load("TestPlugin_$(Configuration)_$(PlatformTarget).dll")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #