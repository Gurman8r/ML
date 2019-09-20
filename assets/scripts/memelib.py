# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_config
class ML_Config:
    architecture    = int   (   memelib_config.architecture     ) #
    build_date      = str   (   memelib_config.build_date       ) #
    build_time      = str   (   memelib_config.build_time       ) #
    compiler_name   = str   (   memelib_config.compiler_name    ) #
    compiler_ver    = int   (   memelib_config.compiler_ver     ) #
    configuration   = str   (   memelib_config.configuration    ) #
    cplusplus_ver   = int   (   memelib_config.cplusplus_ver    ) #
    is_debug        = bool  (   memelib_config.is_debug         ) #
    platform_target = str   (   memelib_config.platform_target  ) #
    project_url     = str   (   memelib_config.project_url      ) #
    project_ver     = str   (   memelib_config.project_ver      ) #
    system_name     = str   (   memelib_config.system_name      ) #
config = ML_Config()


# Prefrences
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_prefs
class ML_Prefs:
    def load(self, filename):
        """Load settings from INI file """
        return memelib_prefs.load(filename)

    def save(self, filename):
        """ Save settings to INI file """
        return memelib_prefs.save(filename)
    
    def set(self, section, name, value):
        """ Overwrite a value in preferences """
        return memelib_prefs.set(str(section), str(name), str(value))

    def get(self, section, name, value): 
        """ Retreive a value from preferences """
        return memelib_prefs.get(str(section), str(name), str(value))
prefs = ML_Prefs()


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_io
class ML_IO:
    def clear(self):                return memelib_io.clear()
    def command(self, cmd):         return memelib_io.command(str(cmd))
    def pause(self):                return memelib_io.pause()
    def print(self, value):         return memelib_io.print(str(value))
    def printl(self, value):        return memelib_io.printl(str(value))
    def format(self, fmt, args):    return memelib_io.format(str(fmt), list(args))
    def printf(self, fmt, args):    return self.print(self.format(fmt, list(args)))
    def system(self, command):      return memelib_io.system(str(command))
io = ML_IO()


# Window
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_window
class ML_Window:
    def close(self):                return memelib_window.close()
    def destroy(self):              return memelib_window.destroy()
    def iconify(self):              return memelib_window.iconify()
    def make_context_current(self): return memelib_window.context_current()
    def maximize(self):             return memelib_window.maximize()
    def poll_events(self):          return memelib_window.poll_events()
    def restore(self):              return memelib_window.restore()
    def swap_buffers(self):         return memelib_window.swap_buffers()
    def terminate(self):            return memelib_window.terminate()
    def is_open(self):              return memelib_window.is_open()
    def is_focused(self):           return memelib_window.is_focused()
    def get_cx(self):               return memelib_window.get_cx()
    def get_cy(self):               return memelib_window.get_cy()
    def get_key(self, k):           return memelib_window.get_key(int(k))
    def get_title(self):            return memelib_window.get_title()
    def get_w(self):                return memelib_window.get_w()
    def get_h(self):                return memelib_window.get_h()
    def get_x(self):                return memelib_window.get_x()
    def get_y(self):                return memelib_window.get_y()
    def set_clipboard(self, s):     return memelib_window.set_clipboard(str(s))
    def set_cursor(self, c):        return memelib_window.set_cursor(int(c))
    def set_pos(self, x, y):        return memelib_window.set_pos(int(x), int(y))
    def set_size(self, w, h):       return memelib_window.set_size(int(w), int(h))
    def set_swap_interval(self, x): return memelib_window.swap_interval(int(x))
    def set_title(self, s):         return memelib_window.set_title(str(s))
window = ML_Window()


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
    def create(self, type, name):
        return memelib_content.create(str(type), str(name))
    def destroy(self, type, name):
        return memelib_content.destroy(str(type), str(name))
content = ML_Content()

class Entity:
    def __init__(self, name):
        self.name = str(name)
    def create(self):
        return content.create("Entity", self.name)
    def attach(self, type):
        return memelib_content.add_component(self.name, str(type))

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