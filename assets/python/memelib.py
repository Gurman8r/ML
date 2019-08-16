import memelib_config
import memelib_content
import memelib_io
import memelib_window
import memelib_plugins

# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
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
class ML_IO:
    def pause(self):
        return memelib_io.pause()
    def print(self, value): 
        return memelib_io.print(value)
    def printl(self, value): 
        return memelib_io.printl(value)
io = ML_IO()


# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
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
class ML_Window:
    def is_open(): 
        return memelib_window.is_open()
window = ML_Window()


# Plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Plugins:
    def load(self, filename): 
        return memelib_plugins.load(filename)
plugins = ML_Plugins()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #