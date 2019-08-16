# C++
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import memelib_cplusplus as cpp


# Config
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Config:
    architecture    = cpp.architecture
    compiler_name   = cpp.compiler_name
    compiler_ver    = cpp.compiler_ver
    configuration   = cpp.configuration
    platform_target = cpp.platform_target
    project_url     = cpp.project_url
    project_ver     = cpp.project_ver

config = ML_Config()


# IO
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_IO:
    def pause(self): return cpp.pause()
    def print(self, value): return cpp.print(value)
    def printl(self, value): return cpp.printl(value)

io = ML_IO()


# Content
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ML_Content:
    def load(self, data):
        if isinstance(data, dict):
            cpp.load(data)
        elif isinstance(data, list):
            for elem in data:
                self.load(elem)

content = ML_Content()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #