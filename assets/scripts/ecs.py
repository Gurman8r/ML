# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type_name): 
        self.type_name = str(type_name)

class Camera(Component):
    def __init__(self): Component.__init__(self, "struct ml::Camera")

class Light(Component):
    def __init__(self): Component.__init__(self, "struct ml::Light")

class Renderer(Component):
    def __init__(self): Component.__init__(self, "struct ml::Renderer")

class Transform(Component):
    def __init__(self): Component.__init__(self, "struct ml::Transform")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, item_name):
        self.type_name = "struct ml::Entity"
        self.item_name = str(item_name)

    def create(self):
        return content.create(self.type_name, self.item_name)

    def destroy(self):
        return content.destroy(self.type_name, self.item_name)

    def attach(self, value):
        if isinstance(value, Component): return self.attach(value.type_name)
        elif isinstance(value, str): return ecs.add_component(self.item_name, value)
        else: return False

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

e = Entity("MyEntity")
if e.create():
    io.printf("Create {0}: \'{1}\'\n", [ e.type_name, e.item_name ])
    if e.attach(Renderer()): 
        io.printf("Attached: {0}\n", [ r.type_name ])
    else:
        io.printl("Failed attaching Renderer")
else:
    io.printf("Entity \'{0}\' already exists.", [ e.item_name ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #