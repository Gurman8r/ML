# C++ Interface
import memelib_cplusplus as cpp

# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_IO:
    def pause(self): return cpp.pause()
    def print(self, value): return cpp.print(value)
    def printl(self, value): return cpp.printl(value)

io = ML_IO()

# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Content:
    def load(self, type, name, data):
        if isinstance(data, dict):
            data["type"] = type
            data["name"] = name
            if not cpp.load(data):
                io.printl("Failed loading: " + str(data))

content = ML_Content()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #