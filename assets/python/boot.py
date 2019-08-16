import memelib as ml

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    if ml.config.is_debug:
        ml.io.printl("Hello from Python!")

    ml.plugins.load("Noobs_$(Configuration)_$(PlatformTarget).dll")
    ml.plugins.load("CommandSuite_$(Configuration)_$(PlatformTarget).dll")
    ml.plugins.load("TestPlugin_$(Configuration)_$(PlatformTarget).dll")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #