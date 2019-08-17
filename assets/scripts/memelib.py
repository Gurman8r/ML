# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_config
class ML_Config:
    architecture    = int   (   memelib_config.architecture     )
    compiler_name   = str   (   memelib_config.compiler_name    )
    compiler_ver    = int   (   memelib_config.compiler_ver     )
    configuration   = str   (   memelib_config.configuration    )
    is_debug        = bool  (   memelib_config.is_debug         )
    platform_target = str   (   memelib_config.platform_target  )
    project_url     = str   (   memelib_config.project_url      )
    project_ver     = str   (   memelib_config.project_ver      )
config = ML_Config()


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_io
class ML_IO:
    def command(self, cmd):         return memelib_io.command(cmd)
    def pause(self):                return memelib_io.pause()
    def print(self, value):         return memelib_io.print(str(value))
    def printl(self, value):        return memelib_io.printl(str(value))
    def format(self, fmt, args):    return memelib_io.format(str(fmt), list(args))
    def printf(self, fmt, args):    return self.print(self.format(fmt, list(args)))
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
    def is_open(self):          return memelib_window.is_open()
    def is_focused(self):       return memelib_window.is_focused()
    def get_key(self, vk):      return memelib_window.get_key(int(vk))
    def get_title(self):        return memelib_window.get_title()
    def get_w(self):            return memelib_window.get_width()
    def get_h(self):            return memelib_window.get_height()
    def get_x(self):            return memelib_window.get_x()
    def get_y(self):            return memelib_window.get_y()
    def get_cx(self):           return memelib_window.get_cursor_x()
    def get_cy(self):           return memelib_window.get_cursor_y()
window = ML_Window()


# Plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_plugins
class ML_Plugins:
    def load(self, value):
        if isinstance(value, str):
            memelib_plugins.load(value)
        elif isinstance(value, list):
            for file in value:
                self.load(file)
plugins = ML_Plugins()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #