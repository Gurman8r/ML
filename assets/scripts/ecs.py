import memelib_content
import memelib_ecs
import memelib_io

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type): 
        self.type = str(type)

class Renderer(Component):
    def __init__(self): 
        Component.__init__(self, "struct ml::Renderer")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, name):
        self.type = "struct ml::Entity"
        self.name = str(name)

    def create(self):
        return bool(memelib_content.create(self.type, self.name))

    def destroy(self):
        return bool(memelib_content.destroy(self.type, self.name))

    def attach(self, value):
        if not isinstance(value, Component): return False
        return bool(memelib_ecs.add_component(self.name, value.type))

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    e = Entity("MyEntity")
    if e.create():
        memelib_io.printl("Created Entity: \'" + e.name + "\'")
        if e.attach(Renderer()):
            memelib_io.printl("Attached Renderer")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #