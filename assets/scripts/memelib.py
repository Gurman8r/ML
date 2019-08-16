# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_config
class ML_Config:
    architecture    = memelib_config.architecture
    compiler_name   = memelib_config.compiler_name
    compiler_ver    = memelib_config.compiler_ver
    configuration   = memelib_config.configuration
    is_debug        = memelib_config.is_debug
    platform_target = memelib_config.platform_target
    project_url     = memelib_config.project_url
    project_ver     = memelib_config.project_ver
config = ML_Config()


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_io
class ML_IO:
    def pause(self):
        return memelib_io.pause()
    def print(self, value): 
        return memelib_io.print(str(value))
    def printl(self, value): 
        return memelib_io.printl(str(value))
io = ML_IO()


# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_content
class ML_Content:
    def load(self, data):
        if isinstance(data, dict):
            memelib_content.load(data)
        elif isinstance(data, list):
            for elem in data:
                self.load(elem)
content = ML_Content()


# Window
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_window
class ML_Window:
    def is_open(): 
        return memelib_window.is_open()
window = ML_Window()


# Plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_plugins
class ML_Plugins:
    def load(self, filename): 
        return memelib_plugins.load(filename)
plugins = ML_Plugins()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #