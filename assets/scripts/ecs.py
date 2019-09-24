# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type_name): 
        self.type_name = str(type_name)

class Camera(Component):
    def __init__(self): Component.__init__(self, "ml::Camera")

class Light(Component):
    def __init__(self): Component.__init__(self, "ml::Light")

class Renderer(Component):
    def __init__(self): Component.__init__(self, "ml::Renderer")

class Transform(Component):
    def __init__(self): Component.__init__(self, "ml::Transform")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, item_name):
        self.type_name = "ml::Entity"
        self.item_name = str(item_name)
        self.components = []

    def attach(self, value):
        if isinstance(value, Component):
            return self.attach(value.type_name)
        if isinstance(value, str):
            if ecs.add_component(self.item_name, value):
                return value
        return None

    def create(self):
        return content.create(self.type_name, self.item_name)

    def destroy(self):
        return content.destroy(self.type_name, self.item_name)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

e = Entity("MyEntity")
if e.create():
    io.printf("Create {0}: \'{1}\'\n", [ e.type_name, e.item_name ])
    if (e.attach(Renderer()) != None): 
        io.printf("Attached: {0}\n", [ r.type_name ])
else:
    io.printf("Entity \'{0}\' already exists.", [ e.item_name ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #