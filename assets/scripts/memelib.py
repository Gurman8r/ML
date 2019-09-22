# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_config
class ML_Config:
    ARCHITECTURE    = int   (   memelib_config.ARCHITECTURE     ) #
    COMPILER_NAME   = str   (   memelib_config.COMPILER_NAME    ) #
    COMPILER_VER    = int   (   memelib_config.COMPILER_VER     ) #
    CONFIGURATION   = str   (   memelib_config.CONFIGURATION    ) #
    CPLUSPLUS_VER   = int   (   memelib_config.CPLUSPLUS_VER    ) #
    IS_DEBUG        = bool  (   memelib_config.IS_DEBUG         ) #
    PLATFORM_TARGET = str   (   memelib_config.PLATFORM_TARGET  ) #
    PROJECT_AUTH    = str   (   memelib_config.PROJECT_AUTH     ) #
    PROJECT_DATE    = str   (   memelib_config.PROJECT_DATE     ) #
    PROJECT_NAME    = str   (   memelib_config.PROJECT_NAME     ) #
    PROJECT_TIME    = str   (   memelib_config.PROJECT_TIME     ) #
    PROJECT_URL     = str   (   memelib_config.PROJECT_URL      ) #
    PROJECT_VER     = str   (   memelib_config.PROJECT_VER      ) #
    SYSTEM_NAME     = str   (   memelib_config.SYSTEM_NAME      ) #


# Prefrences
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_prefs
class ML_Prefs:
    def load(self, filename):
        return memelib_prefs.load(filename)
    def save(self, filename):
        return memelib_prefs.save(filename)
    def set(self, section, name, value):
        return memelib_prefs.set(str(section), str(name), str(value))
    def get(self, section, name, value): 
        return memelib_prefs.get(str(section), str(name), str(value))


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_io
class ML_IO:
    def clear(self):                return memelib_io.clear()
    def command(self, cmd):         return memelib_io.command(str(cmd))
    def exit(self):                 return memelib_io.exit()
    def fatal(self, msg):           return memelib_io.fatal(str(msg))
    def pause(self):                return memelib_io.pause()
    def print(self, value):         return memelib_io.print(str(value))
    def printl(self, value):        return memelib_io.printl(str(value))
    def printf(self, fmt, args):    return memelib_io.printf(str(fmt), list(args))
    def system(self, command):      return memelib_io.system(str(command))


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
    def get_cursor(self):           return memelib_window.get_cursor()
    def get_cx(self):               return memelib_window.get_cx()
    def get_cy(self):               return memelib_window.get_cy()
    def get_key(self, k):           return memelib_window.get_key(int(k))
    def get_title(self):            return memelib_window.get_title()
    def get_size(self):             return memelib_window.get_size()
    def get_w(self):                return memelib_window.get_w()
    def get_h(self):                return memelib_window.get_h()
    def get_position(self):         return memelib_window.get_position()
    def get_x(self):                return memelib_window.get_x()
    def get_y(self):                return memelib_window.get_y()
    def get_time(self):             return memelib_window.get_time()
    def set_clipboard(self, s):     return memelib_window.set_clipboard(str(s))
    def set_cursor(self, c):        return memelib_window.set_cursor(int(c))
    def set_position(self, v):      return memelib_window.set_position(list(v))
    def set_size(self, w, h):       return memelib_window.set_size(int(w), int(h))
    def set_swap_interval(self, x): return memelib_window.swap_interval(int(x))
    def set_title(self, s):         return memelib_window.set_title(str(s))


# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_content
class ML_Content:
    def load(self, data):
        if isinstance(data, dict): return memelib_content.load(data)
        elif isinstance(data, list): return memelib_content.load_all(data)
        else: return False
    
    def create(self, type, name):
        return memelib_content.create(str(type), str(name))
    
    def destroy(self, type, name):
        return memelib_content.destroy(str(type), str(name))
    
    def exists(self, type, name):
        return memelib_content.exists(str(type), str(name))

# Plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_plugins
class ML_Plugins:
    def load(self, value):
        if isinstance(value, str): return memelib_plugins.load(value)
        elif isinstance(value, list): return memelib_plugins.load_all(value)
        else: return False


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #