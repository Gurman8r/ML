import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type): 
        self.type = str(type)

class Camera(Component):
    def __init__(self):
        Component.__init__(self, "struct ml::Camera")

class Light(Component):
    def __init__(self):
        Component.__init__(self, "struct ml::Light")

class Renderer(Component):
    def __init__(self):
        Component.__init__(self, "struct ml::Renderer")

class Transform(Component):
    def __init__(self):
        Component.__init__(self, "struct ml::Transform")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, name):
        self.type = "struct ml::Entity"
        self.name = str(name)
        self.components = []

    def attach(self, value):
        if isinstance(value, Component) and ecs.add_component(self.name, value.type):
            self.components.append(value)
            return value
        return None

    def create(self):
        return bool(content.create(self.type, self.name))

    def destroy(self):
        return bool(content.destroy(self.type, self.name))

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

e = Entity("MyEntity")
if e.create():
    io.printf("Create {0}: \'{1}\'\n", [ e.type, e.name ])
    r = e.attach(Renderer())
    if (r != None): io.printf("Attached: {0}\n", [ r.type ])
else:
    io.printf("Entity \'{0}\' already exists.", [ e.name ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #