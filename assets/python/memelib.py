# C++ Interface
import memelib_cplusplus as cpp


# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Config:
    architecture    = cpp.architecture
    compiler_name   = cpp.compiler_name
    configuration   = cpp.configuration
    platform_target = cpp.platform_target

config = ML_Config()


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_IO:
    def pause(self):
        return cpp.pause()

    def print(self, value): 
        return cpp.print(value)

    def printl(self, value): 
        return cpp.printl(value)

io = ML_IO()


# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Content:
    def load_md(self, data): 
        return cpp.load(data)

    def load(self, type, name, data):
        data["type"] = type
        data["name"] = name
        return self.load_md(data)

content = ML_Content()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #