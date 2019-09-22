import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type): 
        self.type = str(type)

class Renderer(Component):
    def __init__(self): 
        Component.__init__(self, "struct ml::Renderer")
    def set_model(self, value):
        return
    def set_material(self, value):
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, name):
        self.type = "struct ml::Entity"
        self.name = str(name)
        self.components = []

    def create(self):
        return bool(content.create(self.type, self.name))

    def destroy(self):
        return bool(content.destroy(self.type, self.name))

    def attach(self, value):
        if isinstance(value, Component) and ecs.add_component(self.name, value.type):
            self.components.append(value)
            return value
        return None

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

e = Entity("MyEntity")
if e.create():
    io.printl("Created Entity: \'" + e.name + "\'")
    r = e.attach(Renderer())
    if r != None:
        io.printl("Attached Renderer")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #